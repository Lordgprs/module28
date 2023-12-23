#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>

template <class T> class MutexQueue {
public:
  void push(const T &);
  void pop();
  size_t size() const;

private:
  std::queue<T> internal_;
  mutable std::shared_mutex mtx_;
  std::condition_variable_any data_cond;
};

template <class T> void MutexQueue<T>::push(const T &value) {
  std::scoped_lock<std::shared_mutex> lg{mtx_};
  internal_.push(value);
}

template <class T> void MutexQueue<T>::pop() {
  std::scoped_lock<std::shared_mutex> lg{mtx_};
  if (!internal_.empty()) {
    internal_.pop();
  }
}

template <class T> size_t MutexQueue<T>::size() const {
  size_t result{0};
  {
    std::shared_lock<std::shared_mutex> lg{mtx_};
    result = internal_.size();
  }
  return result;
}

int main() {
  MutexQueue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  std::cout << "Queue size: " << q.size() << std::endl;
  q.pop();
  std::cout << "Queue size: " << q.size() << std::endl;

  return EXIT_SUCCESS;
}
