#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <iomanip>
#include <stdexcept>

std::atomic_int max_threads{ 1 };
std::atomic_int thread_count{ 1 };

std::vector<int> merge(std::vector<int>::iterator begin, std::vector<int>::iterator mid, std::vector<int>::iterator end) {
	std::vector<int> result;;
	result.reserve(std::distance(begin, end));
	auto left = begin;
	auto right = mid;

	while (left != mid || right != end) {
		if (left == mid) {
			result.push_back(*right);
			++right;
		}
		else if (right == end) {
			result.push_back(*left);
			++left;
		}
		else if (*left < *right) {
			result.push_back(*left);
			++left;
		}
		else {
			result.push_back(*right);
			++right;
		}
	}

	return result;
}

bool mergeSort(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
	size_t size = std::distance(begin, end);
	if (size <= 1) {
		return true;
	}
	size_t mid = size / 2;
	std::vector<int>::iterator mid_iterator = begin + mid;
	auto left_launch_mode { std::launch::deferred }; // always do this job in current thread
	std::future<bool> future_left = std::async(left_launch_mode, [&]() {
		return mergeSort(mid_iterator, end);
	});
	auto right_launch_mode { std::launch::deferred };
	if (thread_count < max_threads && std::distance(mid_iterator, end) > 1000) {
		right_launch_mode = std::launch::async;
	}
	std::future<bool> future_right = std::async(right_launch_mode, [&](){
		if (right_launch_mode == std::launch::async) {
			++thread_count;
		}
		return mergeSort(mid_iterator, end);
		if (right_launch_mode == std::launch::async) {
			--thread_count;
		}
	});
	if (future_left.get() && future_right.get()) {
		std::vector<int> result = merge(begin, mid_iterator, end);
		std::copy (result.cbegin(), result.cend(), begin);
	}
	return true;
}

void printArray(const std::vector<int> &v) {
	for (auto elem: v) {
		std::cout << std::setw(8) << elem;
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <max threads, 1..64>" << std::endl;
		return EXIT_FAILURE;
	}

	try { 
		max_threads = std::stoi(argv[1]);
		if (max_threads < 1 || max_threads > 64) {
			throw std::invalid_argument{ std::string{ "invalid value " } + argv[1] };
		}
	}
	catch (const std::invalid_argument &e) {
		std::cerr << "Invalid thread count: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	auto unix_timestamp = std::chrono::seconds(std::time(nullptr));
	srand(std::chrono::seconds(unix_timestamp).count());
	std::vector<int> v(2000000);

	std::cout << "Filling an array..." << std::endl;
	std::fill(v.begin(), v.end(), rand() % 1000);

	std::cout << "Sorting of the array started..." << std::endl;
	auto before = std::chrono::high_resolution_clock::now();
	mergeSort(v.begin(), v.end());
	auto after = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration{ after - before };

	std::cout << "Sorting finished in " <<
		std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
		<< " ms..." << std::endl;

	if (std::is_sorted(v.cbegin(), v.cend())) {
		std::cout << "Array has been successfully sorted!" << std::endl;
	}
	else {
		std::cout << "There were an error while sorting the array!" << std::endl;
	}

	return EXIT_SUCCESS;
}

