#pragma once
#include "ThreadPool.h"

class RequestHandler {
public:
  RequestHandler();
  ~RequestHandler();
  void pushRequest(FuncType f, int id, int arg);

private:
  ThreadPool _pool;
};
