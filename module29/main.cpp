#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>

template <class T> class CVQueue {
public:
  void push(const T &value);
  void pop();
  size_t size() const;

private:
  std::queue<T> internal_;
  mutable std::shared_mutex mtx_;
  std::condition_variable_any data_cond_;
};

template <class T> void CVQueue<T>::push(const T &value) {
  std::unique_lock ul{mtx_};
  internal_.push(value);
  data_cond_.notify_one();
}

template <class T> void CVQueue<T>::pop() {
  std::unique_lock<std::shared_mutex> lock{mtx_};
  data_cond_.wait(lock, [this]() { return !internal_.empty(); });
  internal_.pop();
}

template <class T> size_t CVQueue<T>::size() const {
  std::shared_lock sl{mtx_};
  return internal_.size();
}

int main() {
  CVQueue<int> mq;
  mq.push(1);
  mq.push(2);
  mq.push(3);
  std::cout << "Queue size: " << mq.size() << std::endl;
  mq.pop();
  std::cout << "Queue size: " << mq.size() << std::endl;
  return EXIT_SUCCESS;
}
