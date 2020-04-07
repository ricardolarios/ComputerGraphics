#pragma once
#include "ObjFileParser.h"
#include <iostream>
#include <fstream>
#include "helper.h"

// Vertex Texture struct for compact vbo usage.
struct VertexData
{
	float x;
	float y;
	float z;
	float s;
	float t;
};

//////////////////// Helpers /////////////////////

std::string ltrim(const std::string& string)
{
	size_t first = string.find_first_not_of(" ");
	return (first == std::string::npos) ? "" : string.substr(first);
}

std::string get_next_string_by_delimiter(const std::string& string, size_t start = 0, char delimiter = ' ')
{
	std::string trimmed = ltrim(string);
	size_t end = string.find_first_of(delimiter);

	return (end == std::string::npos) ? string : string.substr(0, end);
}


/**
  Determines if the given VerTex is already stored in the given vector of vertices/textures.
  If found, sets result as the found index and returns true. Else, returns false.
  This will be a slow version. Will eventually refactor to be a faster version.
  */
bool get_same_vertex_index(const QVector<float>& verts, const VertexData& given, size_t& result)
{
	size_t curr_index = 0;

	if (verts[curr_index] == given.x
		&& verts[curr_index + 1] == given.y
		&& verts[curr_index + 2] == given.z
		&& verts[curr_index + 3] == given.s
		&& verts[curr_index + 4] == given.t
		)
	{
		result = curr_index;
		return true;
	}
	else
	{
		return false;
	}
}

///////////////////// ObjFileParser methods ///////////////////////

///////////////////////////////////////////////////////////////////
// PRIVATE

void ObjFileParser::parse_file(std::string filepath)
{
	std::ifstream ifile(filepath);
	std::string cur_line;

	// Setup temp vectors, and then we will store everything as GLfloat*, etc. 
	// Storing using GLfloat* since I couldn't figure out how to use std::vector with QTOpenGL
	std::vector<float> temp_vertices;
	std::vector<float> temp_textures;
	std::vector<float> temp_normals;
	std::vector<unsigned int> temp_v_indices;
	std::vector<unsigned int> temp_n_indices;

	std::string mtlfile;

	if (ifile.is_open())
	{
		while (std::getline(ifile, cur_line))
		{
			std::string trimmed_line = ltrim(cur_line);
			std::string first_word = get_next_string_by_delimiter(trimmed_line);

			// Get the info from mtllib
			if (first_word == "mtllib")
			{
				trimmed_line = trimmed_line.substr(first_word.size());

				// TODO: have to update the filepath to be correct. Store first part of path and pass it perhaps?
				mtlfile = "../objects/house/"  + get_next_string_by_delimiter(trimmed_line);
				this->mtl = new MtlFileParser(mtlfile);
			}
			// Per the assignment, we only care if the line starts with `v`, `vn`, 'f' or 'vt' (don't use vn yet).
			if (first_word == "v")
			{
				size_t current_index = 0;
				trimmed_line = trimmed_line.substr(first_word.size()); // trim string so we can check  properly.

				while (current_index < trimmed_line.size())
				{
					float temp = std::stof(trimmed_line, &current_index);
					// Multiply by .5 so that I can see the entire thing on the screen.
					temp_vertices.push_back(temp * .5f);

					trimmed_line = trimmed_line.substr(current_index); // Get rid of stuff we already went through in string.
					current_index = 0; // reset index
				}

				// Add color data at the end. I'll just set all colors to white for this assignment.
				/*temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);*/
			}
			else if (first_word == "vn")
			{
				size_t current_index = 0;
				trimmed_line = trimmed_line.substr(first_word.size()); // remove string so we can check properly.

				while (current_index < trimmed_line.size())
				{
					float temp = std::stof(trimmed_line, &current_index);
					temp_normals.push_back(temp);

					trimmed_line = trimmed_line.substr(current_index); // Get rid of stuff we already went through in string.
					current_index = 0; // reset index
				}
			}
			else if (first_word == "vt")
			{
				size_t current_index = 0;
				trimmed_line = trimmed_line.substr(first_word.size()); // remove string so we can check properly.

				while (current_index < trimmed_line.size())
				{
					float temp = std::stof(trimmed_line, &current_index);
					temp_textures.push_back(temp);

					trimmed_line = trimmed_line.substr(current_index); // Get rid of stuff we already went through in string.
					current_index = 0; // reset index
				}
			}
			else if (first_word == "f")
			{
				//std::cout << cur_line << std::endl;
				// parse as face line
				size_t current_index = 0;	
				trimmed_line = trimmed_line.substr(first_word.size()); // remove string so we can check string properly.

				while (!trimmed_line.empty())
				{
					trimmed_line = ltrim(trimmed_line);
					size_t end_of_curr_face = trimmed_line.find_first_of(" ");
					std::string curr_face = trimmed_line.substr(0, end_of_curr_face);

					// Temporary design choice to count the face values, stops after 2 because we dont care about normals rn.
					// For later abstractions, could make a vector or array of the vectors of indices and use counter as index for that.
					int counter = 0;
					while (counter < 2) //
					{
						size_t end_of_first_val = curr_face.find_first_of('/');
						std::string curr_num = curr_face.substr(0, end_of_first_val);
						curr_face = curr_face.substr(end_of_first_val + 1); // shift it over for next iteration
						//std::cout << "curr_num: " << curr_num << std::endl;
						
						size_t curr_face_index = std::stoul(curr_num) - 1;


						counter == 0 ? 
							temp_v_indices.push_back(curr_face_index) : 
							temp_textures.push_back(curr_face_index);
						counter++;
					}

					if(end_of_curr_face == std::string::npos) break;
					// Get rid of data from line so that we don't double read/loop on the same thing.
					trimmed_line = trimmed_line.substr(end_of_curr_face + 1); // Get rid of stuff we already went through in string.
					//std::cout << "after after, trimmed line: " << trimmed_line << std::endl;

				}
			}

		}

	}


	// Now we have to do the work to compact the vertex data.

	// This is where we will store everything as one array (vert coordinates, texture coords, etc).	
	QVector<float> compacted_verts;
	QVector<unsigned int> compacted_indices;
	for (size_t i = 0; i < temp_v_indices.size(); i++)
	{
		size_t curr_vert_idx = temp_v_indices[i];
		size_t curr_tex_idx = temp_textures[i];

		size_t compacted_index;

		VertexData curr_compact =
		{
				temp_vertices[curr_vert_idx * 3],
				temp_vertices[curr_vert_idx * 3 + 1],
				temp_vertices[curr_vert_idx * 3 + 2],
				temp_textures[curr_vert_idx * 2],
				temp_textures[curr_vert_idx * 2 + 1],
		};

		bool result = get_same_vertex_index(compacted_verts, curr_compact, compacted_index);

		// We haven't seen a similar vertex yet, push back the needed info and set the new index
		if (!result)
		{
			compacted_verts.push_back(curr_compact.x);
			compacted_verts.push_back(curr_compact.y);
			compacted_verts.push_back(curr_compact.z);
			compacted_verts.push_back(curr_compact.s);
			compacted_verts.push_back(curr_compact.t);

			// We added the new info, so to get the index we divide size of each vertex (currently 5) and subtract 1 for 0 indexing.
			compacted_index = (unsigned int) (compacted_verts.size() / 5) - 1;
			compacted_indices.push_back(compacted_index);
		}

		compacted_indices.push_back(compacted_index);

	}
	std::cout << "got here" << std::endl;


	// Now set everything from the temp vectors to the GLfloat* and GLuint*

	this->compact_verts_ = compacted_verts;
	this->indices_ = compacted_indices;
	this->vert_size_ = 5;
	ifile.close();
}


////////////////////////////////////////////////////////////////////////////
// PUBLIC

ObjFileParser::ObjFileParser() : texture_(QOpenGLTexture::Target2D)
{
	this->filepath_ = "";
}

ObjFileParser::ObjFileParser(std::string filepath) : texture_(QOpenGLTexture::Target2D)
{
	this->parse_file(filepath);
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
	return this->mtl->get_diffuse_ppm();
}

QVector<float> ObjFileParser::get_verts()
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
