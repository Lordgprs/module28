#include "requesthandler.h"

RequestHandler::RequestHandler() { _pool.start(); }

RequestHandler::~RequestHandler() { _pool.stop(); }

void RequestHandler::pushRequest(FuncType f, int id, int arg) {
  _pool.push_task(f, id, arg);
}
