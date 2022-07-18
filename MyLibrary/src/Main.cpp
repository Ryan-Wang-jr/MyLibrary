#include <iostream>
#include <vector>

#include "SinglyLinkedList.h"
#include "Vec3.h"

int main() {
	std::cout << "Welcome to my library!\n";


	lan::SinglyLinkedList<int> Test(2, 1);

	// std::vector<int> mid(3, 2);
	lan::SinglyLinkedList<int> mid(2, 2);

	Test.insertAfter(Test.cBegin(), mid.begin(), mid.end());

	for (const auto& element : Test) {
		std::cout << element << ", ";
	}
	std::cout << '\n';

	return 0;
}