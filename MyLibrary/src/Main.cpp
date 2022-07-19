#include <iostream>
#include <forward_list>

#include "SinglyLinkedList.h"
#include "Vec3.h"
#include "Timer.h"

int main() {
	std::cout << "Welcome to my library!\n";

	std::cout << "___________________________________\n";
	std::cout << "[Test1] : fill constructor\n";
	std::cout << "LAN implementation : ";
	{
		Timer Test1;
		for (int i = 0; i < 100; ++i) {
			lan::SinglyLinkedList<int> Container(100, 1);
		}
	}
	std::cout << "STD implementation : ";
	{
		Timer Test1;
		for (int i = 0; i < 100; ++i) {
			std::forward_list<int> Container(100, 1);
		}
	}

	std::cout << "___________________________________\n";
	std::cout << "[Test2] : iterator traversal\n";
	std::cout << "LAN implementation : ";
	{
		lan::SinglyLinkedList<int> Container(100, 1);
		Timer Time;
		for (int i = 0; i < 100; ++i) {
			auto begin = Container.begin();
			auto end = Container.end();
			for (; begin != end; ++begin) {
				++(*begin);
			}
		}
	}
	std::cout << "STD implementation : ";
	{
		std::forward_list<int> Container(100, 1);
		Timer Time;
		for (int i = 0; i < 100; ++i) {
			auto begin = Container.begin();
			auto end = Container.end();
			for (; begin != end; ++begin) {
				++(*begin);
			}
		}
	}
	return 0;
}