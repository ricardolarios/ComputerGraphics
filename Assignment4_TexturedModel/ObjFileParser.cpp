#pragma once
#include "ObjFileParser.h"
#include <iostream>
#include <fstream>
#include "helper.h"

///////////////////// ObjFileParser methods ///////////////////////

////////////////////////////////////////////////////////////////////////////
// PUBLIC

ObjFileParser::ObjFileParser() : texture_(QOpenGLTexture::Target2D)
{
	this->filepath_ = "";
}

ObjFileParser::ObjFileParser(std::string filepath) : texture_(QOpenGLTexture::Target2D)
{
	// Store the path up to the file so that we can use it later for consistency.
	size_t last_slash = filepath.find_last_of('/');
	assert(last_slash != std::string::npos);

	this->directory_path_ = QString::fromStdString(filepath.substr(0, last_slash + 1));

	bool was_parse_successful = this->parse_file(filepath);
	if (!was_parse_successful)
	{
		std::cout << "Parse was very unsuccessful" << std::endl;
	}
	this->filepath_ = filepath;
}

ObjFileParser::~ObjFileParser()
{
}


bool ObjFileParser::equals(ObjFileParser& other)
{
	return this->filepath_.compare(other.filepath_) == 0;
}

QString ObjFileParser::get_texture_file()
{
	return this->directory_path_ + this->mtl->get_diffuse_ppm();
}

//QVector<float> ObjFileParser::get_verts()
//{
//	return this->compact_verts_;
//}

QVector<VertexData> ObjFileParser::get_compact_vert()
{
	return this->compact_verts_;
}

QVector<unsigned int> ObjFileParser::get_indices()
{
	return this->indices_;
}

unsigned int ObjFileParser::get_vertex_size()
{
	return this->vert_size_;
}

// Returns true if file is loaded correctly, otherwise returns false if something occurs.
bool ObjFileParser::parse_file(std::string path)
{
	// TODO: Add check if its even an obj file?
	std::ifstream file(path);

	if (!file.is_open()) return false;

	// Get our vectors and such ready
	QVector<QVector3D> positions;
	QVector<QVector3D> normals;
	QVector<QVector2D> texCoords;
	QVector<QVector3D> faces;

	std::string curline;
	while (std::getline(file, curline))
	{
		std::string token = Helper::get_first_token(curline);
		// Check for the mtl file
		if (token == "mtllib")
		{
			// TODO: have to update the filepath to be correct. Store first part of path and pass it perhaps?
			std::string mtlfile = this->directory_path_.toStdString() + Helper::get_all_but_first_token(curline);
			//std::cout << mtlfile << std::endl;
			this->mtl = new MtlFileParser(mtlfile);

			//qDebug() << "PPM: "; qDebug() << this->mtl->get_diffuse_ppm();
		}
		// v line
		if (token == "v")
		{
			std::vector<std::string> vert_list = Helper::split(Helper::get_all_but_first_token(curline), " ");
			QVector3D vert_pos;

			vert_pos.setX(std::stof(vert_list[0]));
			vert_pos.setY(std::stof(vert_list[1]));
			vert_pos.setZ(std::stof(vert_list[2]));

			positions.push_back(vert_pos);

		}
		// vn line
		else if (token == "vn")
		{
			std::vector<std::string> norm_list = Helper::split(Helper::get_all_but_first_token(curline), " ");
			QVector3D vert_norm;

			vert_norm.setX(std::stof(norm_list[0]));
			vert_norm.setY(std::stof(norm_list[1]));
			vert_norm.setZ(std::stof(norm_list[2]));

			normals.push_back(vert_norm);
		}
		// vt line
		else if (token == "vt")
		{
			std::vector<std::string> tex_list = Helper::split(Helper::get_all_but_first_token(curline), " ");
			QVector2D vert_tex;

			vert_tex.setX(std::stof(tex_list[0]));
			vert_tex.setY(std::stof(tex_list[1]));

			texCoords.push_back(vert_tex);
		}
		// f line
		else if (token == "f")
		{
			std::vector<std::string> face_list = Helper::split(Helper::get_all_but_first_token(curline), " ");
	

			for (int i = 0; i < face_list.size(); i++)
			{
				std::vector<std::string> curr_face = Helper::split(face_list[i], "/");

				/*std::cout << "Curr face 0: " << curr_face[0] << std::endl;
				std::cout << curr_face[1] << std::endl;
				std::cout << curr_face[2] << std::endl;*/
				// TODO: Later, get this to work with multiple types of faces, and no textures, etc.
				QVector3D face;
				face.setX(std::stoul(curr_face[0]) - 1);
				face.setY(std::stoul(curr_face[1]) - 1);
				face.setZ(std::stoul(curr_face[2]) - 1);

				faces.push_back(face);
			}
		}
	}


	//for (int i = 0; i < positions.size(); i++)
	//{
	//	//std::cout << "Positions: (" << positions[i].x() << ", " << positions[i].y() << ", " << positions[i].z() << ")" << std::endl;
	//}

	// Abstract out to handle normals and stuff.
	QVector<VertexData> compact_verts;
	QVector<unsigned int> compact_indices;

	for (int i = 0; i < faces.size(); i++)
	{
		VertexData curr;
		unsigned int pos_index = faces[i].x();
		unsigned int tex_index = faces[i].y();

		curr.x = positions[pos_index].x();
		curr.y = positions[pos_index].y();
		curr.z = positions[pos_index].z();
		curr.s = texCoords[tex_index].x();
		curr.t = texCoords[tex_index].y();

		int contains_curr = compact_verts.indexOf(curr);
		if (contains_curr == -1)
		{
			compact_indices.push_back(compact_verts.size());
			compact_verts.push_back(curr);
		}
		else
		{
			compact_indices.push_back(contains_curr);
		}
	}

	// Store the information.
	// POSSIBLY REDO LATER JUST TO STORE THE NORMALS/POSITIONS/ETC AND LET RENDERABLE HANDLE THIS.
	this->compact_verts_ = compact_verts;
	this->indices_ = compact_indices;

	file.close();
}
