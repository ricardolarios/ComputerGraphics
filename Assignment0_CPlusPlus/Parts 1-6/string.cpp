#include <iostream>
#include <string>

int main()
{
	std::string full_name = "Rikki Larios";
	std::string first_name = full_name.substr(0, 5);
	std::string last_name = full_name.substr(6);

	std::cout << "First: " << first_name << std::endl;
	std::cout << "Last: " << last_name << std::endl;

	unsigned int i = 0;
	while (i < full_name.length())
	{
		if (full_name.at(i) == ' ')
		{
			//break from loop
			break;
		}
		std::cout << full_name[i];
		++i;
	}

	return 0;
}