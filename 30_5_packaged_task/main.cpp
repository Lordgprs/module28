#include "requesthandler.h"
#include <chrono>
#include <iostream>

std::mutex mtx_cout;

void taskFunc(int id, int delay) {
  std::this_thread::sleep_for(std::chrono::seconds(delay));
  std::lock_guard<std::mutex> lg{mtx_cout};
  std::cout << "task " << id << " made by thread_id "
            << std::this_thread::get_id() << std::endl;
}

int main() {
  srand(static_cast<unsigned>(time(nullptr)));
  RequestHandler rh;
  std::vector<res_type> results;

  for (int i = 0; i < 20; ++i) {
    results.push_back(rh.pushRequest(taskFunc, i, 1 + rand() % 9));
  }
  for (auto &result : results) {
    result.wait();
  }

  return EXIT_SUCCESS;
}
