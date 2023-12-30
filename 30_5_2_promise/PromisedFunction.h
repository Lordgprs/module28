#pragma once

#include <functional>
#include <future>

template <typename T> class PromisedFunction {
public:
  PromisedFunction() = default;
  PromisedFunction(std::function<T()> pfun);
  bool valid();
  void operator()();
  std::future<T> get_future();

private:
  std::function<T()> _function;
  std::promise<T> _promise;
};

template <typename T>
PromisedFunction<T>::PromisedFunction(std::function<T()> pfun)
    : _function{pfun} {}

template <typename T> bool PromisedFunction<T>::valid() {
  return static_cast<bool>(_function);
}

template <typename T> void PromisedFunction<T>::operator()() {
  _function();
  _promise.set_value();
}

template <typename T> std::future<T> PromisedFunction<T>::get_future() {
  return _promise.get_future();
}
