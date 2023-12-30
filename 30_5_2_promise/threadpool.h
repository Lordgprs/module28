#pragma once

#include "BlockedQueue.h"
#include "PromisedFunction.h"
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using FuncType = void (*)(int, int);
using task_type = PromisedFunction<void>;
using res_type = std::future<void>;

class ThreadPool {
public:
  ThreadPool();
  void start();
  void stop();
  res_type push_task(FuncType f, int id, int arg);
  void threadFunc(int qindex);

private:
  unsigned int _thread_count;
  std::vector<std::thread> _threads;
  std::vector<BlockedQueue<task_type>> _thread_queues;
  int _index;
};
