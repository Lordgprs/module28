#pragma once
#include "BlockedQueue.h"
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

using FuncType = void (*)(int, int);
using task_type = std::function<void()>;

class ThreadPool {
public:
  ThreadPool();
  void start();
  void stop();
  void pushTask(FuncType f, int id, int arg);
  void threadFunc(int qindex);

private:
  unsigned _thread_count;
  std::vector<std::thread> _threads;
  std::vector<BlockedQueue<task_type>> _thread_queues;
  int _index;
};
