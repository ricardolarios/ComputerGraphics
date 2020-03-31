#pragma once
#include "MtlFileParser.h"
#include "helper.h"
#include <fstream>
#include <iostream>

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
			// TODO: I have NO idea why im getting multiple definitions errors but shrug ill rewrite it for now.
			size_t first_letter = cur_line.find_first_not_of(" ");
			std::string trimmed_line = (first_letter == std::string::npos) ? "" : cur_line.substr(first_letter);

			size_t end = trimmed_line.find_first_of(" ");
			std::string first_word = (end == std::string::npos) ? trimmed_line : trimmed_line.substr(0, end);

			// For now we just wanna get the path for the diffuse
			if (first_word == "map_Kd")
			{
				// TODO: I have no idea how to get paths properly with this. Maybe have the path from the std input stick through.
				size_t end = trimmed_line.find_first_of(" ");
				std::string path = (end == std::string::npos) ? trimmed_line : trimmed_line.substr(0, end);
				this->diffuse_path = "../objects/house/" + path;
			}
		}
	}
}

////////// Public

MtlFileParser::MtlFileParser(std::string path)
{
	this->parse(path);
}

QString MtlFileParser::get_diffuse_ppm()
{
	return QString::fromStdString(this->diffuse_path);
}