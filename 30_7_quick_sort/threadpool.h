#pragma once

#include "BlockedQueue.h"
#include "PromisedFunction.h"
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class RequestHandler;

using FuncType = void (*)(RequestHandler &, int *, size_t, size_t);
using task_type = PromisedFunction<void>;

class ThreadPool {
public:
  ThreadPool();
  void start();
  void stop();
  unsigned int getThreadCount() const;
  unsigned int getActiveThreadCount() const;
  unsigned int getFreeThreadCount() const;
  std::future<void> push_task(FuncType f, RequestHandler &, int *array,
                              size_t left, size_t right);
  void threadFunc(int qindex);

private:
  unsigned int _thread_count;
  std::vector<std::thread> _threads;
  std::vector<BlockedQueue<task_type>> _thread_queues;
  int _index;
  unsigned int _active_thread_count{0};
};
