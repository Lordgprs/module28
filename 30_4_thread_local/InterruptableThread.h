#pragma once
#include "ThreadPool.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

class ThreadPool;

struct thread_interrupt_struct {
  thread_interrupt_struct() : thread_interrupt_flag{false}, cv{nullptr} {}
  std::mutex _defender;
  bool thread_interrupt_flag;
  std::condition_variable *cv;
};

class InterruptableThread {
public:
  InterruptableThread(ThreadPool *pool, int qIndex);
  ~InterruptableThread();
  void interruptableWait(std::condition_variable &cv,
                         std::unique_lock<std::mutex> &ul,
                         std::function<bool()> predicate);
  void interrupt();
  void startFunc(ThreadPool *pool, int qIndex);
  static bool checkInterrupted();

private:
  thread_interrupt_struct *_pStopFlag;
  std::thread _thread;
  std::mutex _mtx;
};
