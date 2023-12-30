#pragma once
#include "ThreadPool.h"

class RequestHandler {
public:
  RequestHandler();
  ~RequestHandler();
  void pushRequest(FuncType f, int id, int arg);
  void interruptPool();

private:
  ThreadPool _pool;
};
