#include <iostream>
#include <future>
#include <thread>
#include <chrono>

void threadFunc(std::packaged_task<int()> &pt) {
	std::cout << "int threadFunc" << std::endl;
	pt();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "threadFunc finished" << std::endl;
}

int main() {
	std::packaged_task<int()> task([]() {
		std::cout << "Packaged task finished work" << std::endl;
		return 10;
	});

	std::future<int> f = task.get_future();
	std::thread t{ threadFunc, std::ref(task) };

	try {
		std::cout << "Future result: " << f.get() << std::endl;
	}
	catch (...) {
		std::cout << "Some error in thread" << std::endl;
	}

	t.join();
}


