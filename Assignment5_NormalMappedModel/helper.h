#pragma once
#include <string>
#include <vector>

class Helper
{
	public:
		// Get first token in given line. Delimiter is a space.
		static std::string get_first_token(std::string& given)
		{
			if (!given.empty())
			{
				size_t start = given.find_first_not_of(" "); // find the first actual char
				size_t end = given.find_first_of(" ", start); // first space after the start

				if (start != std::string::npos && end != std::string::npos)
				{
					return given.substr(start, end - start); // start at start, go for end - start chars
				}
				else if (start != std::string::npos)
				{
					return given.substr(start); // if only the end is npos, then just get the rest of the line.
				}
			}
			return "";
		}

		// Split based on a delimiter and put it in a vector or something.
		static std::vector<std::string> split(std::string& given, std::string delim)
		{
			std::vector<std::string> result;
			std::string buff;

			for (int i = 0; i < given.size(); i++)
			{
				std::string curr = given.substr(i, delim.size()); // check up to the size because the delim could be any length 

				if (curr == delim)
				{
					// Push back onto result probably, then increase our counter proportionately to delim size.
					if (buff.empty())
					{
						result.push_back(""); // just push an empty. maybe push nothing later, not sure.
					}
					else
					{
						// not empty. so push it, clear the buffer, increment counter
						result.push_back(buff);
						buff.clear();
						i += delim.size() - 1; // - 1 because it will automatically increment by 1 anyway.
					}
				}
				else if (i + delim.size() >= given.size())
				{
					// Append the rest of the line and push back
					buff += given.substr(i); // add everything up to the end of the string
					result.push_back(buff);
					break;
				}
				else
				{
					buff += given[i];
				}
			}

			return result;
		}

		// Get everything besides the first token.
		static std::string get_all_but_first_token(std::string& given)
		{
			// Look for the first token. Find first that isn't a space.
			size_t first_start = given.find_first_not_of(" ");
			// Find the first space after the first token (starting at first_start)
			size_t first_space = given.find_first_of(" ", first_start);
			// Now find the first non-space *after* that. This is the start of the end result.
			size_t start_result = given.find_first_not_of(" ", first_space);
			size_t end = given.find_last_not_of(" "); // no trailing whitespace

			if (start_result != std::string::npos && end != std::string::npos)
			{
				return given.substr(start_result, end - start_result + 1);
			}
			else if (start_result != std::string::npos)
			{
				return given.substr(start_result);
			}
			else
			{
				return "";
			}
		}
};