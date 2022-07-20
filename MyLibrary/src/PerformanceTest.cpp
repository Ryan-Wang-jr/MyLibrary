#include <cstdio>
#include <forward_list>

#include "SinglyLinkedList.h"
#include "Vec3.h"
#include "Timer.h"
#include "PerformanceTest.h"

static void helper(const int count, const char* info, void (*Func1)(), void (*Func2)()) {
	static const char* line    = "___________________________________\n";
	static const char* test    = "[Test%d] : %s\n";
	static const char* lan_imp = "LAN implementation : ";
	static const char* std_imp = "STD implementation : ";
	std::printf(line);
	std::printf(test, count, info);
	std::printf(lan_imp);
	Func1();
	std::printf(std_imp);
	Func2();
}

void performanceTest() {

	helper(1, "Fill Constructor", [] {
			Timer Time;
			for (int i = 0; i < 100; ++i) {
				lan::SinglyLinkedList<int> Container(100, 1);
			}
		}, [] {
			Timer Time;
			for (int i = 0; i < 100; ++i) {
				std::forward_list<int> Container(100, 1);
			}
		});

	helper(2, "Iterator Traversal", [] {
			lan::SinglyLinkedList<int> Container(100, 1);
			Timer Time;
			for (int i = 0; i < 100; ++i) {
				auto begin = Container.begin();
				auto end = Container.end();
				for (; begin != end; ++begin) {
					++(*begin);
				}
			}
		}, [] {
			std::forward_list<int> Container(100, 1);
			Timer Time;
			for (int i = 0; i < 100; ++i) {
				auto begin = Container.begin();
				auto end = Container.end();
				for (; begin != end; ++begin) {
					++(*begin);
				}
			}
		});

}