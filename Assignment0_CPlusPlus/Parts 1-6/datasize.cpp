#include <iostream>
#include <fstream>

int main()
{
	std::cout << "Data size of int in bytes: " << sizeof(int) << std::endl;
	std::cout << "Data size of char in bytes: " << sizeof(char) << std::endl;
	std::cout << "Data size of float in bytes: " << sizeof(float) << std::endl;
	std::cout << "Data size of double in bytes: " << sizeof(double) << std::endl;

	std::cout << "Hello" << std::endl;
	std::cout << "How old are you?" << std::endl;

	int age;
	std::cin >> age;

	std::cout << "Cool, you're old: " << age << std::endl;

	return 0;
}