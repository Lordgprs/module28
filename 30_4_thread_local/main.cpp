#include "RequestHandler.h"
#include <chrono>
#include <iostream>

std::mutex mtx_cout;

void taskFunc(int id, int delay) {
  for (int i = 0; i < delay; ++i) {
    if (InterruptableThread::checkInterrupted()) {
      std::unique_lock<std::mutex> ul{mtx_cout};
      std::cout << "task " << id << " was interrupted" << std::endl;
      return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::unique_lock<std::mutex> ul{mtx_cout};
  std::cout << "task " << id << " made by thread_id "
            << std::this_thread::get_id() << std::endl;
}

int main() {
  srand(static_cast<unsigned>(time(nullptr)));
  RequestHandler rh;

  for (int i = 1; i <= 20; ++i) {
    rh.pushRequest(taskFunc, i, 1 + rand() % 9);
  }
  std::this_thread::sleep_for(std::chrono::seconds(4));
  rh.interruptPool();

  return EXIT_SUCCESS;
}
