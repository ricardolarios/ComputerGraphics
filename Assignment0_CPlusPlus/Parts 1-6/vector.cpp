#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	// Create vector to store data.
	std::vector<unsigned char> my_data;

	// Add data.
	for(unsigned int i = 65; i < 91; i++)
	{
		my_data.push_back(i);
	}

	for (unsigned int i = 0; i < my_data.size(); i++)
	{
		std::cout << my_data[i];
	}

	return 0;
}