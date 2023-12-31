#include "requesthandler.h"

RequestHandler::RequestHandler() { _pool.start(); }

RequestHandler::~RequestHandler() { _pool.stop(); }

unsigned int RequestHandler::getThreadCount() const {
  return _pool.getThreadCount();
}

unsigned int RequestHandler::getActiveThreadCount() const {
  return _pool.getActiveThreadCount();
}

unsigned int RequestHandler::getFreeThreadCount() const {
  return _pool.getFreeThreadCount();
}

std::future<void> RequestHandler::pushRequest(FuncType f, RequestHandler &rh,
                                              int *array, size_t left,
                                              size_t right) {
  return _pool.push_task(f, rh, array, left, right);
}
