#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using FuncType = void (*)(int, int);
using task_type = std::function<void()>;

class ThreadPool {
public:
  ThreadPool() = default;
  void start();
  void stop();
  void push_task(FuncType f, int id, int arg);
  void threadFunc();

private:
  std::vector<std::thread> _threads;
  std::mutex _mtx;
  std::queue<task_type> _task_queue;
  std::condition_variable _event_holder;
  volatile std::atomic_bool _is_working{false};
};
