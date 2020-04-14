#pragma once
#include <string>
#include <QtOpenGL>

class MtlFileParser
{
	private:
		std::string diffuse_ppm;
		void parse(std::string mtlfile);

	public:
		MtlFileParser(std::string path);

		QString get_diffuse_ppm();
};