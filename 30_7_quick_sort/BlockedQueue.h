#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <class T> class BlockedQueue {
public:
  void push(T &item);
  void pop(T &item);
  bool fast_pop(T &item);

private:
  std::mutex _mtx;
  std::queue<T> _task_queue;
  std::condition_variable _notifier;
};

template <class T> void BlockedQueue<T>::push(T &item) {
  std::lock_guard<std::mutex> lg{_mtx};
  _task_queue.push(std::move(item));
  _notifier.notify_one();
}

template <class T> void BlockedQueue<T>::pop(T &item) {
  std::unique_lock<std::mutex> ul{_mtx};
  if (_task_queue.empty()) {
    _notifier.wait(ul, [this] { return !_task_queue.empty(); });
  }
  item = std::move(_task_queue.front());
  _task_queue.pop();
}

template <class T> bool BlockedQueue<T>::fast_pop(T &item) {
  std::lock_guard<std::mutex> lg{_mtx};
  if (_task_queue.empty()) {
    return false;
  }
  item = std::move(_task_queue.front());
  _task_queue.pop();
  return true;
}
