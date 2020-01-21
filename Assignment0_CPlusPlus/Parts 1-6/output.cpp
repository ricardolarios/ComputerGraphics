#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	std::ofstream out_file;

	out_file.open("temp.txt");

	out_file << "# Some image data" << std::endl;
	out_file << "P3" << std::endl;

	out_file.close();
	
	return 0;
}