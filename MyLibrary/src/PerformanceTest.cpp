#include <cstdio>
#include <forward_list>

#include "SinglyLinkedList.h"
#include "Vec3.h"
#include "Timer.h"
#include "PerformanceTest.h"

template <class Functor1, class Functor2>
static void helper(const int count, const char* info, Functor1 Func1, Functor2 Func2) {
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

template <class Container>
struct fillConstructor {
	void operator()() {
		Timer Time;
		for (int i = 0; i < 100; ++i) {
			Container Cont(1000, 1);
		}
	}
};

void performanceTest() {

	helper(1, "Fill Constructor",
		fillConstructor<std::forward_list<int>>(),
		fillConstructor<lan::SinglyLinkedList<int>>());

}
