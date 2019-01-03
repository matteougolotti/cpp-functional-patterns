/* 

MIT License

Copyright (c) 2018 Matteo Ugolotti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <thread>
#include <type_traits>
#include <vector>

namespace fp
{

// Maximum number of threads for concurrent functions
static const int kMaxThreads = 4;

// A collection of objects supporting functional patterns
template <typename T>
class collection
{
  private:
    std::vector<T> _values;

  public:
    // Constructor for epty collection
    collection<T>() :
      _values{} {
    }

    // Empty collection of given size
    collection<T>(int size) :
	  _values{size} {
	}

	collection<T>(std::initializer_list<T> values) :
	  _values{values} {

	}

	// Builds a collection from iterators
  template <class Iterator>
	collection<T>(Iterator begin,
	              Iterator end) :
	  _values{begin, end} {
	}

	// Vector constructor
	collection<T>(std::vector<T> const& v) :
	  _values{v} {
	}

	// List constructor
	collection<T>(std::list<T> const& v) :
	  _values{v.begin(), v.end()} {
	}

	// C-style array constructor
	collection<T>(T d[], int len) {
	  _values.assign(d, d + len);
	}

	// Overload operator []
	T operator[](const int index) const {
	  return _values[index];
	}

	// Overload operator ==
	bool operator==(collection<T> const& other) const {
	  return _values == other._values;
	};

	// Return collection as a std::vector
	std::vector<T> vector() const {
	  return _values;
	};

	// Return the collection as a std::list
	std::list<T> list() const {
	  return std::list<T>{_values.begin(), _values.end()};
	}

	// Overload the << operator
	friend std::ostream &operator<<(std::ostream &stream, collection<T> const& f) {
	  stream << "[";

	  for (int i = 0; i < f._values.size() - 1; i++) {
	    stream << f._values[i] << ",";
	  }

	  stream << f._values[f._values.size() - 1];
	  stream << "]";

	  return stream;
	}

	// Returns the size of the collection
	int size() const;

	// Returns only the first element of the Collection
	// Throws if the collection is empty
	T head() const;

	// Returns a copy of the Collection, except the first element
	collection<T> tail() const;

	// Applies a function to each element of the collection
	void each(std::function<void(T)> f);

	// Returns a subset of the collection, filtered by the given predicate
	collection<T> filter(std::function<bool(T)> f) const;

	// Returns the [begin, end) subset of the collection
	collection<T> slice(int begin, int end) const;

	// Returns the number of elements for which the given predicate evaluates to true
	int count(std::function<bool(T)> f) const;

	// Returns a copy of the Collection, sorted according to the given predicate
	collection<T> sort(std::function<bool(T, T)> f) const;

	// Returns a new collection, as the result of the application of the given function
	// to each element of the initial collection
	template <typename Func>
	collection<typename std::result_of<Func(T)>::type>
	map(Func f) const;

	// A concurrent implementation of map
	template <typename Function>
	collection<typename std::result_of<Function(T)>::type>
	pmap(Function func, const unsigned long threads = kMaxThreads) const;

	// Returns the result of the application of the binary operator on the Collection
	// starting from the first element
	// Throws if the collection is empty
	T reduce(std::function<T(T, T)> f) const;

	// Returns the result of the application of the binary operator on the Collection
	// starting from the last element
	// Throws if the Collection is empty
	T rightreduce(std::function<T(T, T)> f) const;

	// Returns the result of the application of a binary operator on
	// all elements in the Collection from a given initial value, starting
	// from the first element
	// Throws if the Collection is empty
	template <typename Function, typename I>
	typename std::result_of<Function(I, T)>::type
	fold(Function f, I init) const;

	// Returns the result of the application of a binary operator on
	// all elements in the Collection from a given initial value, starting
	// from the last element
	// Throws if the Collection is empty
	template <typename Function, typename I>
	typename std::result_of<Function(I, T)>::type
	foldr(Function f, I init) const;

  collection<T> concat(const collection<T>&) const;
};

template <typename T>
int collection<T>::size() const
{
  return _values.size();
}

template <typename T>
T collection<T>::head() const
{
  return (_values.size() > 0) ? _values[0]
                              : throw std::runtime_error("Empty collection");
}

template <typename T>
collection<T> collection<T>::tail() const
{
  return (_values.size() > 0) ? collection{_values.begin() + 1, _values.end()}
                              : collection{};
}

template <typename T>
void collection<T>::each(std::function<void(T)> f)
{
  for (auto const& value : _values) {
    f(value);
  }
}

template <typename T>
collection<T> collection<T>::filter(std::function<bool(T)> f) const
{
  std::vector<T> values;
  for (auto const& value : _values) {
    if (f(value)) {
      values.push_back(value);
    }
  }

  return collection<T>{values};
}

template <typename T>
collection<T> collection<T>::slice(int begin, int end) const
{
  std::vector<T> values;
  values.resize(end - begin);

  for (int i = 0; i + begin < end; ++i) {
    values[i] = _values[i + begin];
  }

  return collection<T>(values);
}

template <typename T>
int collection<T>::count(std::function<bool(T)> f) const {
  int count {0};

  for (auto const& value : _values) {
    if (f(value)) {
      ++count;
    }
  }

  return count;
}

template <typename T>
collection<T>  collection<T>::sort(std::function<bool(T, T)> f) const {
  std::vector<T> sorted{_values};

  std::sort(sorted.begin(), sorted.end(), f);

  return collection<T>{sorted};
}

template <typename T>
template <typename Function>
collection<typename std::result_of<Function(T)>::type> collection<T>::map(Function f) const {
  using return_type = typename std::result_of<Function(T)>::type;
  std::vector<return_type> values;
  values.resize(_values.size());

  for (int i = 0; i < _values.size(); ++i) {
    values[i] = f(_values[i]);
  }

  return collection<return_type>(values);
}

template<typename T, typename Function>
void pmap_thread(int begin, int end, Function func, std::vector<T>& values) {
  for (int i = begin; i < end && i < values.size(); ++i) {
    values[i] = func(values[i]);
  }
}

template <typename T>
template <typename Function>
collection<typename std::result_of<Function(T)>::type>
collection<T>::pmap(Function func, const unsigned long threads) const
{
  std::vector<std::thread> thread_pool(threads);
  const unsigned long chunk = _values.size() / threads;
  const unsigned long extra = _values.size() - chunk*threads;
  std::vector<int> indices(extra, chunk + 1);
  std::vector<int> normal(threads - extra, chunk);
  indices.insert(indices.end(), normal.begin(), normal.end());
  std::vector<T> values { _values };

  int start = 0;

  for (int i = 0; i < threads; ++i) {
    int end = start + indices[i];

    thread_pool[i] = std::thread ([=, &values]() {
      pmap_thread(start, end, func, values);
    });

    start = end;
  }

  for (int i = 0; i < threads; ++i) {
    thread_pool[i].join();
  }

  return collection<T>{ values };
}

template <typename T>
T collection<T>::reduce(std::function<T(T, T)> f) const
{
  if (_values.empty()) {
    throw std::runtime_error("Empty collection");
  }

  if (_values.size() == 1) {
    return _values[0];
  }

  T value { f(_values[0], _values[1]) };
  for (int i = 2; i < _values.size(); ++i) {
    value = f(value, _values[i]);
  }

  return value;
}

template <typename T>
T collection<T>::rightreduce(std::function<T(T, T)> f) const {
  if (_values.empty()) {
    throw std::runtime_error("Empty collection");
  }

  if (_values.size() == 1) {
    return _values[0];
  }

  T value { f(_values[_values.size() - 1], _values[_values.size() - 2]) };
  for (int i = _values.size() - 3; i >= 0; --i) {
    value = f(value, _values[i]);
  }

  return value;
}

template <typename T>
template <typename Function, typename I>
typename std::result_of<Function(I, T)>::type
collection<T>::fold(Function f, I init) const {
  using return_type = typename std::result_of<Function(I, T)>::type;
  static_assert(std::is_same<return_type, I>::value,
      "Initial value and return value do not match");

  if (_values.empty()) {
    throw std::runtime_error("Collection is empty");
  }

  if (_values.size() == 1) {
    return _values[0];
  }

  return_type val = f(init, _values[0]);
  for (int i = 1; i < _values.size(); ++i) {
    val = f(val, _values[i]);
  }

  return val;
}

template <typename T>
template <typename Function, typename I>
typename std::result_of<Function(I, T)>::type
collection<T>::foldr(Function f, I init) const {
  using return_type = typename std::result_of<Function(I, T)>::type;
  static_assert(std::is_same<return_type, I>::value,
      "Initial value and return value do not match");

  if (_values.empty()) {
    throw std::runtime_error("Collection is empty");
  }

  if (_values.size() == 1) {
    return _values[0];
  }

  return_type value = f(init, _values[_values.size() - 1]);
  for (int i = _values.size() - 2; i >= 0; i--) {
    value = f(value, _values[i]);
  }

  return value;
}

template <typename T>
collection<T>
collection<T>::concat(const collection<T>& c) const {
  const auto firstSize = _values.size();
  const auto secondSize = c.size();
  const auto totalSize = firstSize + secondSize;

  std::vector<T> values(totalSize);

  for (auto i{0}; i < firstSize; ++i) {
    values[i] = _values[i];
  }

  for (auto i{0}; i < secondSize; ++i) {
    values[firstSize + i] = c[i];
  }

  return collection<T>(values);
}

}
