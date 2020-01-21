#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
	// input stream
	std::ifstream in_file;

	// This is going to read a file name from the command line args
	// So yeah.

	// 2 because it counts the program name as the first one
	if (argc != 2)
	{
		std::cout << "Oops, exactly two arguments needed." << std::endl;
		std::cout << "ProgramName file_path.\n";
		return 0;
	}

	// Use arg[1] to open file
	in_file.open(argv[1]);

	if (in_file.is_open())
	{
		std::string line;
		while (getline(in_file, line))
		{
			std::cout << line << std::endl;
		}
	}

	in_file.close();
	return 0;
}