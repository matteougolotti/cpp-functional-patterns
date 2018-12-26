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

#include <functional>
#include <memory>

namespace fp {

template <typename InT, typename OutT> class Match;
template <typename InT, typename OutT> class MatchExpression;
template <typename InT, typename OutT> class DefaultMatch;
template <typename InT, typename OutT> class DefaultExpression;

template <typename InT, typename OutT>
class Match {

  private:
    InT const _input;
    bool _isMatched;
    std::unique_ptr<OutT> _result;
    std::unique_ptr<std::function<OutT(InT)>> _resultFunction;

  public:
    Match<InT, OutT>(InT const input, bool isMatched) :
      _input { input },
      _isMatched { isMatched } {
    }

    Match<InT, OutT>(InT const input, bool isMatched, OutT const result) :
      _input { input },
      _isMatched { isMatched } {
        _result = std::make_unique<OutT>(result);
    }

    Match<InT, OutT>(InT const input, bool isMatched, std::function<OutT(InT)> resultFunction) :
      _input { input },
      _isMatched { isMatched } {
        _resultFunction = std::make_unique<std::function<OutT(InT)>>(resultFunction);
    }

    MatchExpression<InT, OutT> operator>=(InT match) {
      if (_isMatched and _result) {
        return MatchExpression<InT, OutT> { _input, true, *_result };
      }

      if (_isMatched and _resultFunction) {
        return MatchExpression<InT, OutT> { _input, true, *_resultFunction };
      }

      if (match == _input) {
        return MatchExpression<InT, OutT> { _input, true };
      }

      return MatchExpression<InT, OutT> { _input, false };
    }

    OutT operator|(OutT const& result) {
      if (_isMatched and _result) {
        return *_result;
      }

      if (_isMatched and _resultFunction) {
        return (*_resultFunction)(_input);
      }

      return result;
    }

    OutT operator|(std::function<OutT(InT)> resultFunction) {
      if (_isMatched and _result) {
        return *_result;
      }

      if (_isMatched and _resultFunction) {
        return (*_resultFunction)(_input);
      }

      return resultFunction(_input);
    }
};

template <typename InT, typename OutT>
class MatchExpression {

  private:
    InT const _input;
    bool _isMatched;
    std::unique_ptr<OutT> _result;
    std::unique_ptr<std::function<OutT(InT)>> _resultFunction;

  public:
    MatchExpression<InT, OutT>(InT const input, bool isMatched) :
      _input { input },
      _isMatched { isMatched } {
    }

    MatchExpression<InT, OutT>(InT const input, bool isMatched, OutT const result) :
      _input { input },
      _isMatched { isMatched } {
        _result = std::make_unique<OutT>(result);
    }

    MatchExpression<InT, OutT>(InT const input, bool isMatched, std::function<OutT(InT)> resultFunction) :
      _input { input },
      _isMatched { isMatched } {
        _resultFunction = std::make_unique<std::function<OutT(InT)>>(resultFunction);
    }

    Match<InT, OutT> operator>(OutT result) {
      if (_isMatched and _result) {
        return Match<InT, OutT> { _input, true, *_result };
      }

      if (_isMatched and _resultFunction) {
        return Match<InT, OutT> { _input, true, *_resultFunction };
      }

      if (_isMatched and (not _result) and (not _resultFunction)) {
        return Match<InT, OutT> { _input, true, result };
      }

      return Match<InT, OutT> { _input, false };
    }

    Match<InT, OutT> operator>(std::function<OutT(InT)> resultFunction) {
      if (_isMatched and _result) {
        return Match<InT, OutT> { _input, true, *_result };
      }

      if (_isMatched and _resultFunction) {
        return Match<InT, OutT> { _input, true, *_resultFunction };
      }

      if (_isMatched and (not _result) and (not _resultFunction)) {
        return Match<InT, OutT> { _input, true, resultFunction };
      }

      return Match<InT, OutT> { _input, false };
    }
};

template <typename InT, typename OutT>
Match<InT, OutT> match(InT input) {
  return Match<InT, OutT> { input, false };
}

}
