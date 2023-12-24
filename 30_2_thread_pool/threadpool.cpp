#include "threadpool.h"

void ThreadPool::start() {
  _is_working = true;
  for (int i = 0; i < 4; ++i) {
    _threads.emplace_back(&ThreadPool::threadFunc, this);
  }
}

void ThreadPool::stop() {
  _is_working = false;
  _event_holder.notify_all();
  for (auto &thread : _threads) {
    thread.join();
  }
}

void ThreadPool::push_task(FuncType f, int id, int arg) {
  std::lock_guard<std::mutex> lg{_mtx};
  task_type new_task{[=] { f(id, arg); }};
  _task_queue.push(new_task);
  _event_holder.notify_one();
}

void ThreadPool::threadFunc() {
  while (true) {
    task_type task_to_do;
    {
      std::unique_lock<std::mutex> ul{_mtx};
      if (_task_queue.empty() && !_is_working) {
        return;
      }
      if (_task_queue.empty()) {
        _event_holder.wait(
            ul, [&]() { return !_task_queue.empty() || !_is_working; });
      }
      if (!_task_queue.empty()) {
        task_to_do = _task_queue.front();
        _task_queue.pop();
      }
    }
    if (task_to_do) {
      task_to_do();
    }
  }
}
