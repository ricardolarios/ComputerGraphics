#pragma once
#include "MtlFileParser.h"
#include <fstream>
#include <iostream>
#include "helper.h"

//////////// Private
void MtlFileParser::parse(std::string mtlfile)
{
	std::ifstream ifile(mtlfile);
	std::string cur_line;

	if (ifile.is_open())
	{
		// Parse the line until we get what we want. For now we only care about diffuse.
		while (std::getline(ifile, cur_line))
		{
			std::string first_token = Helper::get_first_token(cur_line);

			// For now we just wanna get the path for the diffuse
			if (first_token == "map_Kd")
			{
				std::cout << "hi" << std::endl;
				// TODO: I have no idea how to get paths properly with this. Maybe have the path from the std input stick through.
				std::string path = Helper::get_all_but_first_token(cur_line);
				this->diffuse_ppm = path;
			}
		}
	}

	ifile.close();
}

////////// Public

MtlFileParser::MtlFileParser(std::string path)
{
	this->diffuse_ppm = "";
	this->parse(path);
}

QString MtlFileParser::get_diffuse_ppm()
{
	return QString::fromStdString(this->diffuse_ppm);
}