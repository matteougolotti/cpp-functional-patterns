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
class Collection
{
  private:
    std::vector<T> _values;

  public:
    // Constructor for epty collection
    Collection<T>() :
      _values{} {
    }

    // Empty collection of given size
    Collection<T>(int size) :
	  _values{size} {
	}

	// Builds a collection from iterators
	Collection<T>(std::iterator<std::input_iterator_tag, int> const& begin,
	              std::iterator<std::input_iterator_tag, int> const& end) :
	  _values{begin, end} {
	}

	// Vector constructor
	Collection<T>(std::vector<T> const& v) :
	  _values{v} {
	}

	// List constructor
	Collection<T>(std::list<T> const& v) :
	  _values{v.begin(), v.end()} {
	}

	// C-style array constructor
	Collection<T>(T d[], int len) {
	  _values.assign(d, d + len);
	}

	// Overload operator []
	T operator[](const int index) const {
	  return _values[index];
	}

	// Overload operator ==
	bool operator==(Collection<T> const& other) const {
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
	friend std::ostream &operator<<(std::ostream &stream, Collection<T> const& f) {
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
	Collection<T> tail() const;

	// Applies a function to each element of the collection
	void each(std::function<void(T)> f);

	// Returns a subset of the collection, filtered by the given predicate
	Collection<T> filter(std::function<bool(T)> f) const;

	// Returns the [begin, end) subset of the collection
	Collection<T> slice(int begin, int end) const;

	// Returns the number of elements for which the given predicate evaluates to true
	int count(std::function<bool(T)> f) const;

	// Returns a copy of the Collection, sorted according to the given predicate
	Collection<T> sort(std::function<bool(T, T)> f) const;

	// Returns a new collection, as the result of the application of the given function
	// to each element of the initial collection
	template <typename Func>
	Collection<typename std::result_of<Func(T)>::type>
	map(Func f) const;

	// A concurrent implementation of map
	template <typename Function>
	Collection<typename std::result_of<Function(T)>::type>
	tmap(Function func, const int threads = kMaxThreads) const;

	// Returns the result of the application of the binary operator on the Collection
	// starting from the first element
	// Throws if the collection is empty
	T reducel(std::function<T(T, T)> f) const;

	// Returns the result of the application of the binary operator on the Collection
	// starting from the last element
	// Throws if the Collection is empty
	T reducer(std::function<T(T, T)> f) const;

	// A concurrent implementation of reducel
	// Operator f must be commutative for the result to be correct
	// Throws if the Collection is empty
	T treduce(std::function<T(T, T)> f, const int threads = kMaxThreads) const;

	// Returns the result of the application of a binary operator on
	// all elements in the Collection from a given initial value, starting
	// from the first element
	// Throws if the Collection is empty
	template <typename Function, typename I>
	typename std::result_of<Function(I, T)>::type
	foldl(Function f, I init) const;

	// Returns the result of the application of a binary operator on
	// all elements in the Collection from a given initial value, starting
	// from the last element
	// Throws if the Collection is empty
	template <typename Function, typename I>
	typename std::result_of<Function(I, T)>::type
	foldr(Function f, I init) const;
};

template <typename T>
int Collection<T>::size() const
{
  return _values.size();
}

template <typename T>
T Collection<T>::head() const
{
  return (_values.size() > 0) ? _values[0]
                              : throw std::runtime_error("Empty collection");
}

template <typename T>
Collection<T> Collection<T>::tail() const
{
  return (_values.size() > 0) ? Collection{_values.begin() + 1, _values.end()}
                              : Collection{};
}

template <typename T>
void Collection<T>::each(std::function<void(T)> f)
{
  for (auto const& value : _values) {
    f(value);
  }
}

template <typename T>
Collection<T> Collection<T>::filter(std::function<bool(T)> f) const
{
  std::vector<T> values;
  for (auto const& value : _values) {
    if (f(value)) {
      values.push_back(value);
    }
  }

  return Collection<T>{values};
}

template <typename T>
Collection<T> Collection<T>::slice(int begin, int end) const
{
  std::vector<T> values{end - begin};
  for (int i = 0; i < end - begin; ++i) {
    values[i] = _values[i + end];
  }

  return Collection<T>(values);
}

template <typename T>
int Collection<T>::count(std::function<bool(T)> f) const {
  int count {0};

  for (auto const& value : _values) {
    if (f(value)) {
      ++count;
    }
  }

  return count;
}

template <typename T>
Collection<T>  Collection<T>::sort(std::function<bool(T, T)> f) const {
  std::vector<T> sorted{_values};

  std::sort(sorted.begin(), sorted.end(), f);

  return Collection<T>{sorted};
}

template <typename T>
template <typename Function>
Collection<typename std::result_of<Function(T)>::type> Collection<T>::map(Function f) const {
  using return_type = typename std::result_of<Function(T)>::type;
  std::vector<return_type> values{_values.size()};

  for (int i = 0; i < _values.size(); ++i) {
    values[i] = f(_values[i]);
  }

  return Collection<return_type>(values);
}

template<typename T, typename Function>
void tmap_thread(int begin, int end, Function func, std::vector<T>& values) {
  for (int i = begin; i < end && i < values.size(); ++i) {
    values[i] = func(values[i]);
  }
}

template <typename T>
template <typename Function>
Collection<typename std::result_of<Function(T)>::type>
Collection<T>::tmap(Function func, const int threads) const
{
  std::vector<std::thread> thread_pool{ threads };
  const int chunk = _values.size() / threads;
  const int extra = _values.size() - chunk*threads;
  std::vector<int> indices{ extra, chunk+1 };
  std::vector<int> normal{ threads-extra, chunk };
  indices.insert(indices.end(), normal.begin(), normal.end());
  std::vector<T> values { _values };

  int start = 0;

  for (int i = 0; i < threads; ++i) {
    int end = start + indices[i];

    thread_pool[i] = std::thread ([=, &values]() {
      tmap_thread(start, end, func, values);
    });

    start = end;
  }

  for (int i = 0; i < threads; ++i) {
    thread_pool[i].join();
  }

  return Collection<T>{ values };
}

template <typename T>
T Collection<T>::reducel(std::function<T(T, T)> f) const
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
T Collection<T>::reducer(std::function<T(T, T)> f) const {
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

template<typename T>
void treduce_thread(int tid, int begin, int end, std::function<T(T, T)> f,
                    std::vector<T>& values, std::vector<T>& results) {

    T value = f(values[begin], values[begin + 1]);

    for (int i = begin + 2; i < end && i < values.size(); ++i) {
        value = f(value, values[i]);
    }

    results[tid] = value;
}

template <typename T>
T Collection<T>::treduce(std::function<T(T, T)> f, const int threads) const {

  if (_values.empty()) {
    throw std::runtime_error("Collection is empty");
  }

  std::vector<std::thread> thread_pool{ threads };
  std::vector<T> results{ threads };

  const int chunk = _values.size() / threads;
  const int extra = _values.size() - chunk*threads;
  std::vector<int> indices{ extra, chunk+1 };
  std::vector<int> normal{ threads-extra, chunk };

  indices.insert(indices.end(), normal.begin(), normal.end());

  int start = 0;

  for (int i = 0; i < threads; ++i) {
    int end{ start + indices[i] };

    thread_pool[i] = std::thread ([=, &results]() {
      treduce_thread(i, start, end, f, _values, results);
    });

    start = end;
  }

  for (int i = 0; i < thread_pool.size(); ++i) {
    thread_pool[i].join();
  }

  T value{ results[0] };
  if (results.size() > 1) {
    T value = f(results[0], results[1]);
    for (int i = 2; i < results.size(); ++i) {
      value = f(value, results[i]);
    }
  }

  return value;
}

template <typename T>
template <typename Function, typename I>
typename std::result_of<Function(I, T)>::type
Collection<T>::foldl(Function f, I init) const {
  using return_type = typename std::result_of<Function(I, T)>::type;
  static_assert(std::is_same<return_type, I>::value,
      "Initial value and return value do not match");

  if (_values.empty()) {
    throw std::runtime_error("Collection is empty");
  }

  if (_values.size() == 1) {
    return _values[0];
  }

  return_type val = func(init, _values[0]);
  for (int i = 1; i < _values.size(); ++i) {
    val = f(val, _values[i]);
  }

  return val;
}

template <typename T>
template <typename Function, typename I>
typename std::result_of<Function(I, T)>::type
Collection<T>::foldr(Function f, I init) const {
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

template<typename T>
void concat(std::vector<T>& values, Collection<T>& other, int& index) {
  for (int i = 0; i < other.size(); ++i) {
    values[++index] = other[i];
  }
}

// Concatenates several Collections
template<typename T, typename ...Collections>
Collection<T> concat(Collection<T>& first, Collections... others) {

  int size{ first.size() };
  int get_size[]{0, (size += others.size(), 0)...};
  std::vector<T> values(size);

  int index = 0;
  concat(values, first, index);
  int concatenate[]{0, (concat(values, others, index), 0)...};

  return Collection<T>(values);
}

}
