#include <cstdio>
#include <forward_list>
#include <utility>

#include <windows.h>

#include "SinglyLinkedList.h"
#include "Vec3.h"
#include "Timer.h"
#include "PerformanceTest.h"
#include "PerformanceTestObject.h"

static std::size_t memory_usage = 0;

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(std::size_t _Size) {
	memory_usage += _Size;
    if (_Size == 0)
        ++_Size;
 
    if (void *ptr = std::malloc(_Size))
        return ptr;
 
    throw std::bad_alloc{};
}
 
void __CRTDECL operator delete(void* _Block) noexcept {
    std::free(_Block);
}

static HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);

template <class Functor1, class Functor2>
static void helper(const int count, const char* info, Functor1 Func1, Functor2 Func2) {
	static const char* test    = "\n[Test%d] %s\n";
	static const char* lan_imp = "LAN implementation\n";
	static const char* std_imp = "STD implementation\n";
	static const char* time    = "- Time : ";
	static const char* memory  = "- Memory : %zu bytes\n";

	SetConsoleTextAttribute(Console,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::printf(test, count, info);

	SetConsoleTextAttribute(Console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::printf(lan_imp);
	std::printf(time);
	Func1();
	std::printf(memory, memory_usage);
	memory_usage = 0;

	SetConsoleTextAttribute(Console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::printf(std_imp);
	std::printf(time);
	Func2();
	std::printf(memory, memory_usage);
	memory_usage = 0;
}

void sllPerformanceTest() {
	std::printf("SinglyLinkedList and std::forward_list performance test start...\n");
	int count = 0;
	
	helper(++count, "Fill Construct",
		fillConstruct<lan::SinglyLinkedList<std::string>>(),
		fillConstruct<std::forward_list<std::string>>());

	helper(++count, "Range Construct",
		rangeConstruct<lan::SinglyLinkedList<std::string>>(),
		rangeConstruct<std::forward_list<std::string>>());

	helper(++count, "Copy Construct",
		copyConstruct<lan::SinglyLinkedList<std::string>>(),
		copyConstruct<std::forward_list<std::string>>());

	helper(++count, "Move Construct",
		moveConstruct<lan::SinglyLinkedList<std::string>>(),
		moveConstruct<std::forward_list<std::string>>());

	helper(++count, "Destruct",
		destruct<lan::SinglyLinkedList<std::string>>(),
		destruct<std::forward_list<std::string>>());
	
	helper(++count, "Iterate",
		iterate<lan::SinglyLinkedList<std::string>>(),
		iterate<std::forward_list<std::string>>());

	helper(++count, "Emplace Front",
		emplaceFront<lan::SinglyLinkedList<std::string>>(),
		emplaceFront<std::forward_list<std::string>>());
	
	helper(++count, "Push Front Copy",
		pushFrontCopy<lan::SinglyLinkedList<std::string>>(),
		pushFrontCopy<std::forward_list<std::string>>());
	
	helper(++count, "Push Front Move",
		pushFrontMove<lan::SinglyLinkedList<std::string>>(),
		pushFrontMove<std::forward_list<std::string>>());

	helper(++count, "Popfront",
		popFront<lan::SinglyLinkedList<std::string>>(),
		popFront<std::forward_list<std::string>>());
	
	helper(++count, "Emplace After",
		emplaceAfter<lan::SinglyLinkedList<std::string>>(),
		emplaceAfter<std::forward_list<std::string>>());

	helper(++count, "Insert After Copy",
		insertAfterCopy<lan::SinglyLinkedList<std::string>>(),
		insertAfterCopy<std::forward_list<std::string>>());

	helper(++count, "Insert After Move",
		insertAfterMove<lan::SinglyLinkedList<std::string>>(),
		insertAfterMove<std::forward_list<std::string>>());

	helper(++count, "Erase After",
		eraseAfter<lan::SinglyLinkedList<std::string>>(),
		eraseAfter<std::forward_list<std::string>>());

	helper(++count, "Erase After Range",
		eraseAfterRange<lan::SinglyLinkedList<std::string>>(),
		eraseAfterRange<std::forward_list<std::string>>());
	
	helper(++count, "Resize Expand",
		resizeExpand<lan::SinglyLinkedList<std::string>>(),
		resizeExpand<std::forward_list<std::string>>());

	helper(++count, "Resize Shrink",
		resizeShrink<lan::SinglyLinkedList<std::string>>(),
		resizeShrink<std::forward_list<std::string>>());
}
