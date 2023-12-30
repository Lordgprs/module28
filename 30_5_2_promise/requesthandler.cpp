#include "requesthandler.h"

RequestHandler::RequestHandler() { _pool.start(); }

RequestHandler::~RequestHandler() { _pool.stop(); }

res_type RequestHandler::pushRequest(FuncType f, int id, int arg) {
  return _pool.push_task(f, id, arg);
}
