#include "requesthandler.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <string>

std::mutex mtx_cout;
std::atomic_bool make_thread{false};
const constexpr size_t ARRAY_SIZE = 50000;

void debug(const std::string &str) {
  std::lock_guard<std::mutex> lg{mtx_cout};
  std::cout << str << std::endl;
}

void taskFunc(int id, int delay) {
  std::this_thread::sleep_for(std::chrono::seconds(delay));
  std::lock_guard<std::mutex> lg{mtx_cout};
  std::cout << "task " << id << " made by thread_id "
            << std::this_thread::get_id() << std::endl;
}

void quicksort(RequestHandler &rh, int *array, size_t left, size_t right) {
  if (left >= right)
    return;
  size_t left_bound = left;
  size_t right_bound = right;

  size_t pivot_index{left_bound + (right_bound - left_bound) / 2};
  int pivot = array[pivot_index];

  do {
    while (array[left_bound] < pivot) {
      left_bound++;
    }
    while (array[right_bound] > pivot) {
      right_bound--;
    }

    if (left_bound <= right_bound) {
      std::swap(array[left_bound], array[right_bound]);
      left_bound++;
      right_bound--;
    }
  } while (left_bound <= right_bound);

  //  auto active_thread_count = rh.getActiveThreadCount();
  //  auto thread_count = rh.getThreadCount();
  if (make_thread && rh.getActiveThreadCount() < 5 &&
      (right_bound - left > 10000)) {
    debug(std::string{"Thread count: "} + std::to_string(rh.getThreadCount()));
    debug(std::string{"Active thread count: "} +
          std::to_string(rh.getActiveThreadCount()));
    auto future = rh.pushRequest(quicksort, rh, array, left, right_bound);
    quicksort(rh, array, left_bound, right);
    future.wait();
  } else {
    quicksort(rh, array, left, right_bound);
    quicksort(rh, array, left_bound, right);
  }
}

int main() {
  srand(static_cast<unsigned>(time(nullptr)));
  RequestHandler rh;
  int *array = new int[ARRAY_SIZE];
  for (int *it = array; it != array + ARRAY_SIZE; ++it) {
    *it = rand() % 100;
  }
  // std::sort(array, array + ARRAY_SIZE);
  make_thread = true;
  quicksort(rh, array, 0, ARRAY_SIZE);

  if (std::is_sorted(array, array + ARRAY_SIZE)) {
    debug("The array is sorted");
  } else {
    debug("The array is not sorted");
  }

  // for (int i = 1; i <= 5; ++i) {
  //    rh.pushRequest(taskFunc, i, 1 + rand() % 4);
  //  }

  delete[] array;
  return EXIT_SUCCESS;
}
