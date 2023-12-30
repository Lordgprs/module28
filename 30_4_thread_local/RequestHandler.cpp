#include "RequestHandler.h"

RequestHandler::RequestHandler() { _pool.start(); }

RequestHandler::~RequestHandler() { _pool.stop(); }

void RequestHandler::pushRequest(FuncType f, int id, int arg) {
  _pool.pushTask(f, id, arg);
}

void RequestHandler::interruptPool() { _pool.interrupt(); }
