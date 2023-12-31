#pragma once
#include "threadpool.h"
#include <future>

class RequestHandler {
public:
  RequestHandler();
  ~RequestHandler();
  unsigned int getThreadCount() const;
  unsigned int getActiveThreadCount() const;
  unsigned int getFreeThreadCount() const;
  std::future<void> pushRequest(FuncType f, RequestHandler &rh, int *array,
                                size_t left, size_t right);

private:
  ThreadPool _pool;
};
