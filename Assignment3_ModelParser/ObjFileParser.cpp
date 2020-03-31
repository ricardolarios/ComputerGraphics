#include "ObjFileParser.h"
#include <iostream>
#include <fstream>

//////////////////// Helpers /////////////////////

// Trims the whitespace from the left side of a string
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
	std::vector<float> temp_normals;
	std::vector<unsigned int> temp_v_indices;
	std::vector<unsigned int> temp_n_indices;

	if (ifile.is_open())
	{
		while (std::getline(ifile, cur_line))
		{
			std::string trimmed_line = ltrim(cur_line);
			std::string first_word = get_next_string_by_delimiter(trimmed_line);

			// Per the assignment, we only care if the line starts with `v`, `vn`, or `,f`
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
				temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);
				temp_vertices.push_back(1.0f);
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
			else if (first_word == "f")
			{
				// parse as face line
				size_t current_index = 0;
				trimmed_line = trimmed_line.substr(first_word.size()); // remove string so we can check string properly.

				while (!trimmed_line.empty())
				{
					trimmed_line = ltrim(trimmed_line);
					std::string temp = get_next_string_by_delimiter(trimmed_line);
					
					// Can get temp and then parse while `//`
					
					size_t end_of_first_val = temp.find_first_of('/');
					size_t beginning_of_last_val = temp.find_last_of('/') + 1;

					std::string first_val = temp.substr(0, end_of_first_val);
					std::string last_val = temp.substr(beginning_of_last_val);
					
					// Because obj files are 1 indexed, subtract 1.
					size_t vertex_index = std::stoul(first_val) - 1;
					size_t normal_index = std::stoul(last_val) - 1;

					temp_v_indices.push_back(vertex_index);
					temp_n_indices.push_back(normal_index);

					// Get rid of data from line so that we don't double read/loop on the same thing.
					trimmed_line = trimmed_line.substr(temp.size()); // Get rid of stuff we already went through in string.
					trimmed_line = ltrim(trimmed_line);
				}
			}

		}

	}

	// Now set everything from the temp vectors to the GLfloat* and GLuint*
	this->set_vertices(temp_vertices);
	this->set_normals(temp_normals);
	this->set_v_indices(temp_v_indices);
	this->set_n_indices(temp_n_indices);
	ifile.close();
}

// Setup the buffers if they have not already been setup. If they have, simply return.
void ObjFileParser::setup_buffers()
{
	if (this->has_setup_buffers_) return;

	// vbo will store vertex and color info.
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.create();
	vbo_.bind();
	vbo_.allocate(this->vertices_, this->vertices_length_ * sizeof(GL_FLOAT));

	nbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	nbo_.create();
	nbo_.bind();
	nbo_.allocate(this->normals_, this->normals_length_ * sizeof(GL_FLOAT));

	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.create();
	ibo_.bind();
	ibo_.allocate(this->vertices_idx_, this->vertices_idx_length_ * sizeof(GL_UNSIGNED_INT));

	nibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	nibo_.create();
	nibo_.bind();
	nibo_.allocate(this->normals_idx_, this->normals_idx_length_ * sizeof(GL_UNSIGNED_INT));

	this->has_setup_buffers_ = true;
}

/////////////
// Setters

void ObjFileParser::set_vertices(std::vector<float> v)
{
	size_t length = v.size();
	this->vertices_ = new GLfloat[length];

	for (size_t i = 0; i < length; i++)
	{
		this->vertices_[i] = v[i];
	}

	this->vertices_length_ = length;
}

void ObjFileParser::set_normals(std::vector<float> n)
{
	size_t length = n.size();
	this->normals_ = new GLfloat[length];

	for (size_t i = 0; i < length; i++)
	{
		this->normals_[i] = n[i];
	}

	this->normals_length_ = length;
}

void ObjFileParser::set_v_indices(std::vector<unsigned int> v_idx)
{
	size_t length = v_idx.size();
	this->vertices_idx_ = new GLuint[length];

	for (size_t i = 0; i < length; i++)
	{
		this->vertices_idx_[i] = v_idx[i];
	}

	this->vertices_idx_length_ = length;
}

void ObjFileParser::set_n_indices(std::vector<unsigned int> n_idx)
{
	size_t length = n_idx.size();
	this->normals_idx_ = new GLuint[length];

	for (size_t i = 0; i < length; i++)
	{
		this->normals_idx_[i] = n_idx[i];
	}

	this->normals_idx_length_ = length;
}

////////////////////////////////////////////////////////////////////////////
// PUBLIC

ObjFileParser::ObjFileParser() : vbo_(QOpenGLBuffer::VertexBuffer), nbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), nibo_(QOpenGLBuffer::IndexBuffer)
{
	this->filepath_ = "";
	this->has_setup_buffers_ = false;

	this->vertices_ = nullptr;
	this->normals_ = nullptr;
	this->vertices_idx_ = nullptr;
	this->normals_idx_ = nullptr;

	this->vertices_length_ = 0;
	this->normals_length_ = 0;
	this->vertices_idx_length_ = 0;
	this->normals_idx_length_ = 0;
}

ObjFileParser::ObjFileParser(std::string filepath) : vbo_(QOpenGLBuffer::VertexBuffer), nbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), nibo_(QOpenGLBuffer::IndexBuffer)
{
	this->parse_file(filepath);
	this->filepath_ = filepath;
	this->has_setup_buffers_ = false;
}

ObjFileParser::~ObjFileParser()
{
	delete[] this->vertices_;
	delete[] this->normals_;
	delete[] this->vertices_idx_;
	delete[] this->normals_idx_;

	// Release vertex buffers
	this->vbo_.release();
	this->vbo_.destroy();
	this->nbo_.release();
	this->nbo_.destroy();
	this->ibo_.release();
	this->ibo_.destroy();
	this->nibo_.release();
	this->nibo_.destroy();
	this->vao_.release();
	this->vao_.destroy();
}

size_t ObjFileParser::vertices_length()
{
	return this->vertices_length_;
}

size_t ObjFileParser::normals_length()
{
	return this->normals_length_;
}
size_t ObjFileParser::vertex_indices_length()
{
	return this->vertices_idx_length_;
}

size_t ObjFileParser::normal_indices_length()
{
	return this->normals_idx_length_;
}

QOpenGLBuffer ObjFileParser::get_vbo()
{
	return this->vbo_;
}
QOpenGLBuffer ObjFileParser::get_nbo()
{
	return this->nbo_;
}
QOpenGLBuffer ObjFileParser::get_ibo()
{
	return this->ibo_;
}
QOpenGLBuffer ObjFileParser::get_nibo()
{
	return this->nibo_;
}

QOpenGLVertexArrayObject& ObjFileParser::get_vao()
{
	return this->vao_;
}

bool ObjFileParser::equals(ObjFileParser& other)
{
	return this->filepath_.compare(other.filepath_) == 0;
}

bool ObjFileParser::has_setup_buffers()
{
	return this->has_setup_buffers_;
}