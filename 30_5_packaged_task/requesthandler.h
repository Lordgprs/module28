#pragma once
#include "threadpool.h"

class RequestHandler {
public:
  RequestHandler();
  ~RequestHandler();
  res_type pushRequest(FuncType f, int id, int arg);

private:
  ThreadPool _pool;
};
