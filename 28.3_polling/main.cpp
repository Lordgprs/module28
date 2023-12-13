#include <queue>
#include <list>
#include <mutex>
#include <thread>
#include <random>
#include <memory>
#include <string>
#include <iostream>
#include <atomic>

struct Part {
	int part_id;
	float volume;
	using PartPtr = std::shared_ptr<Part>;
};

std::atomic_bool done1{ false };
std::atomic_bool done2{ false };
std::queue<Part::PartPtr> shared_queue1;
std::mutex lock_queue1;
std::queue<Part::PartPtr> shared_queue2;
std::mutex lock_queue2;
std::mutex lock_cout;

void locked_output(const std::string &str) {
	std::lock_guard<std::mutex> raii(lock_cout);
	std::cout << str << std::endl;
}

void threadAWork(Part::PartPtr &part) {
	part->volume -= 2;
	std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 3000));

	locked_output("threadAWork finished with part " + std::to_string(part->part_id));
}

void threadBWork(Part::PartPtr &part) {
	if (!part) {
		return;
	}
	part->volume -= 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 3000));

	locked_output("threadBWork finished with part " + std::to_string(part->part_id));
}

void threadCWork(Part::PartPtr &part) {
	if (!part) {
		return;
	}
	part->volume -= 0.5;
	std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 3000));

	locked_output("threadCWork finished with part " + std::to_string(part->part_id));
}

void threadA(std::list<Part::PartPtr> &input) {
	srand(7777777);
	size_t size = input.size();
	for (size_t i = 0; i < size; ++i) {
		threadAWork(input.front());
		{
			std::lock_guard<std::mutex> raii(lock_queue1);
			shared_queue1.push(input.front());
			input.remove(input.front());
			locked_output("Part was added to queue");
		}
	}
	done1 = true;
}

void threadB() {
	srand(100000);
	while (true) {
		Part::PartPtr part_for_work;
		{
			lock_queue1.lock();
			if (shared_queue1.empty()) {
				lock_queue1.unlock();
				if (done1) {
					break;
				}
				locked_output("threadB useless check, queue is empty. Going to bed...");
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			else {
				part_for_work = shared_queue1.front();
				shared_queue1.pop();
				lock_queue1.unlock();
				locked_output("Part was removed from queue 1");
			}

		}
		threadBWork(part_for_work);
		{
			std::lock_guard<std::mutex> raii{ lock_queue2 };
			shared_queue2.push(part_for_work);
		}
	}
	done2 = true;
}

void threadC() {
	srand(121212);
	while(true) {
		Part::PartPtr part_for_work;
		lock_queue2.lock();
		if (shared_queue2.empty()) {
			lock_queue2.unlock();
			if (done2) {
				break;
			}
			locked_output("threadC useless check, queue is empty. Going to bed...");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else {
			part_for_work = shared_queue2.front();
			shared_queue2.pop();
			lock_queue2.unlock();
			locked_output("Part was removed from queue 2");
		}
		threadCWork(part_for_work);
	}
}

int main() {
	std::list<Part::PartPtr> spare_parts;
	for (int i = 0; i < 5; ++i) {
		spare_parts.push_back(Part::PartPtr(new Part{ i + 1, 10.0 }));
	}
	std::thread ta{ threadA, std::ref(spare_parts) };
	std::thread tb{ threadB };
	std::thread tc{ threadC };

	ta.join();
	tb.join();
	tc.join();

	locked_output("done");

	return EXIT_SUCCESS;
}
