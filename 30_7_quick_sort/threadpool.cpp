#include "threadpool.h"

ThreadPool::ThreadPool()
    : _thread_count{std::thread::hardware_concurrency() != 0
                        ? std::thread::hardware_concurrency()
                        : 4},
      _thread_queues{_thread_count} {}

void ThreadPool::start() {
  for (int i = 0; i < _thread_count; ++i) {
    _threads.emplace_back(&ThreadPool::threadFunc, this, i);
  }
}

void ThreadPool::stop() {
  for (int i = 0; i < _thread_count; ++i) {
    task_type empty_task;
    _thread_queues[i].push(empty_task);
  }
  for (auto &t : _threads) {
    t.join();
  }
}

unsigned int ThreadPool::getThreadCount() const { return _thread_count; }

unsigned int ThreadPool::getActiveThreadCount() const {
  return _active_thread_count;
}

unsigned int ThreadPool::getFreeThreadCount() const {
  return _thread_count - _active_thread_count;
}

std::future<void> ThreadPool::push_task(FuncType f, RequestHandler &rh,
                                        int *array, size_t left, size_t right) {
  int queue_to_push = _index++ % _thread_count;
  task_type task{[=, &rh] { f(rh, array, left, right); }};
  auto future = task.get_future();
  _thread_queues[queue_to_push].push(task);
  return future;
}

void ThreadPool::threadFunc(int qindex) {
  while (true) {
    task_type task_to_do;
    bool res{false};
    int i{0};
    for (; i < _thread_count; ++i) {
      if ((res = _thread_queues[(qindex + i) % _thread_count].fast_pop(
               task_to_do))) {
        break;
      }
    }
    if (!res) {
      _thread_queues[qindex].pop(task_to_do);
    } else if (!task_to_do.valid()) {
      _thread_queues[(qindex + i) % _thread_count].push(task_to_do);
    }
    if (!task_to_do.valid()) {
      return;
    }
    _active_thread_count++;
    task_to_do();
    _active_thread_count--;
  }
}
