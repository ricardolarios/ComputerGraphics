#pragma once
#include <string>
// File full of helper methods.

std::string ltrim(const std::string& string)
{
	size_t first = string.find_first_not_of(" ");
	return (first == std::string::npos) ? "" : string.substr(first);
}

std::string get_next_string_by_delimiter(const std::string& string, size_t start = 0, char delimiter = ' ')
{
	size_t end = string.find_first_of(delimiter);

	return (end == std::string::npos) ? string : string.substr(0, end);
}

