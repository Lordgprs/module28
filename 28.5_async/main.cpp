#include <queue>
#include <list>
#include <mutex>
#include <thread>
#include <random>
#include <memory>
#include <string>
#include <iostream>
#include <atomic>
#include <condition_variable>

struct Part {
	int part_id;
	float volume;
	using PartPtr = std::shared_ptr<Part>;
};

std::atomic_bool done1{ false };
std::atomic_bool done2{ false };
std::queue<Part::PartPtr> shared_queue1;
std::mutex lock_queue1;
std::condition_variable event_holder1;
std::queue<Part::PartPtr> shared_queue2;
std::mutex lock_queue2;
std::condition_variable event_holder2;
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
			locked_output("Part was added to queue 1");
			event_holder1.notify_one();
		}
	}
	done1 = true;
	event_holder1.notify_one();
}

void threadB() {
	srand(100000);
	while (true) {
		std::list<Part::PartPtr> parts_for_work;
		{
			std::unique_lock<std::mutex> m_holder1{ lock_queue1 };
			if (shared_queue1.empty() && done1) {
				break;
			}
			if (shared_queue1.empty()) {
				event_holder1.wait(m_holder1, [](){
					return !shared_queue1.empty() || done1;
				});
			}
			for (size_t i = 0; i < shared_queue1.size(); ++i) {
				parts_for_work.push_back(shared_queue1.front());
				shared_queue1.pop();
			}
			locked_output("Parts were removed from queue 1");

		}
		for (auto &p: parts_for_work) {
			threadBWork(p);
			{
				std::lock_guard<std::mutex> raii{ lock_queue2 };
				shared_queue2.push(p);
				locked_output("Part was added to queue 2");
				event_holder2.notify_one();
			}
		}
	}
	done2 = true;
}

void threadC() {
	srand(121212);
	while(true) {
		std::list<Part::PartPtr> parts_for_work;
		{
			std::unique_lock<std::mutex> m_holder2{ lock_queue2 };
			if (shared_queue2.empty() && done2) {
				break;
			}
			if (shared_queue2.empty()) {
				event_holder2.wait(m_holder2, [](){
					return !shared_queue2.empty() || done2;
				});
			}
			for (size_t i = 0; i < shared_queue2.size(); ++i) {
				parts_for_work.push_back(shared_queue2.front());
				shared_queue2.pop();
			}
			locked_output("Parts were removed from queue 2");
		}
		for (auto &p: parts_for_work) {
			threadCWork(p);
		}
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
