#include <iostream>
#include <vector>

#include "SinglyLinkedList.h"
#include "PerformanceTest.h"

void print(const lan::SinglyLinkedList<int>& Container) {
	for (const auto& element : Container) {
		std::cout << element << ", ";
	}
	std::cout << '\n';
}

int main() {
	std::cout << "Welcome to my library!\n";
	performanceTest();
	

	return 0;
}