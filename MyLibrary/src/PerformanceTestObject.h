#ifndef PERFORMANCETESTOBJECT_H_
#define PERFORMANCETESTOBJECT_H_

#include <array>
#include <vector>
#include <string>

#include "Timer.h"

static const char* Elem = "Yeeeeeeeeeeeeet";

template <class Container>
struct fillConstruct {
	void operator()() {
		std::array<Container, 1000> Target;
		{
			Timer Time;
			for (int i = 0; i < 1000; ++i) {
				Target[i].assign(1000, Elem);
			}
		}
	}
};

template <class Container>
struct rangeConstruct {
	void operator()() {
		std::vector<std::string> Input(1000, Elem);
		std::array<Container, 1000> Target;
		{
			Timer Time;
			for (int i = 0; i < 1000; ++i) {
				Target[i].assign(Input.begin(), Input.end());
			}
		}
	}
};

template <class Container>
struct copyConstruct {
	void operator()() {
		Container Other(1000, Elem);
		std::array<Container, 1000> Target;
		{
			Timer Time;
			for (int i = 0; i < 1000; ++i) {
				Target[i] = Other;
			}
		}
	}
};

template <class Container>
struct moveConstruct {
	void operator()() {
		std::array<Container, 1000> Other;
		Other.fill(Container(1000, Elem));
		std::array<Container, 1000> Target;
		{
			Timer Time;
			for (int i = 0; i < 1000; ++i) {
				Target[i] = std::move(Other[i]);
			}
		}
	}
};

template <class Container>
struct destruct {
	void operator()() {
		std::array<Container, 1000> Target;
		Target.fill(Container(1000, Elem));
		{
			Timer Time;
			for (int i = 0; i < 1000; ++i) {
				Target[i].clear();
			}
		}
	}
};

template <class Container>
struct iterate {
	void operator()() {
		Container Target(1000000, Elem);
		std::size_t temp = 0;
		{
			Timer Time;
			for (const auto& element : Target) {
				temp = element.capacity();
			}
		}
	}
};

template <class Container>
struct emplaceFront {
	void operator()() {
		Container Target;
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				Target.emplace_front(Elem);
			}
		}
	}
};

template <class Container>
struct pushFrontCopy {
	void operator()() {
		Container Target;
		std::string Val(Elem);
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				Target.push_front(Val);
			}
		}
	}
};

template <class Container>
struct pushFrontMove {
	void operator()() {
		Container Target;
		std::vector<std::string> Val(1000000, Elem);
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				Target.push_front(std::move(Val[i]));
			}
		}
	}
};

template <class Container>
struct popFront {
	void operator()() {
		Container Target(1000000, Elem);
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				Target.pop_front();
			}
		}
	}
};

template <class Container>
struct emplaceAfter {
	void operator()() {
		Container Target(1, Elem);
		typename Container::const_iterator it = Target.begin();
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				it = Target.emplace_after(it, Elem);
			}
		}
	}
};

template <class Container>
struct insertAfterCopy {
	void operator()() {
		Container Target(1, Elem);
		typename Container::const_iterator it = Target.begin();
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				it = Target.insert_after(it, Elem);
			}
		}
	}
};

template <class Container>
struct insertAfterMove {
	void operator()() {
		Container Target(1, Elem);
		typename Container::const_iterator it = Target.begin();
		std::vector<std::string> Val(1000000, Elem);
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				it = Target.insert_after(it, std::move(Val[i]));
			}
		}
	}
};

template <class Container>
struct eraseAfter {
	void operator()() {
		Container Target(1000000, Elem);
		typename Container::const_iterator it = Target.before_begin();
		{
			Timer Time;
			for (int i = 0; i < 1000000; ++i) {
				Target.erase_after(it);
			}
		}
	}
};

template <class Container>
struct eraseAfterRange {
	void operator()() {
		Container Target(1000000, Elem);
		{
			Timer Time;
			Target.erase_after(Target.before_begin(), Target.end());
		}
	}
};

template <class Container>
struct resizeExpand {
	void operator()() {
		Container Target(1, Elem);
		{
			Timer Time;
			Target.resize(10000001, Elem);
		}
	}
};

template <class Container>
struct resizeShrink {
	void operator()() {
		Container Target(10000001, Elem);
		{
			Timer Time;
			Target.resize(1);
		}
	}
};

#endif // !PERFORMANCETESTOBJECT_H_