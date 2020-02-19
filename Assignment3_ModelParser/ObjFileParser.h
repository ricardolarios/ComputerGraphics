#pragma once
#include <vector>
#include <string>
#include <QtOpenGL>

/**
* This class is used to parse an Obj file and store the vertices, normals and faces.
* Can be extended later to parse different types of lines.
*/
class ObjFileParser
{
	private:
		std::string filepath_;
		bool has_setup_buffers_;

		// Raw vertex/index data
		GLfloat* vertices_;
		GLfloat* normals_;
		GLuint* vertices_idx_; // Indices of vertices to use
		GLuint* normals_idx_; // Indices of normals to use

		// Sizes for the above raw data
		size_t vertices_length_;
		size_t normals_length_;
		size_t vertices_idx_length_; 
		size_t normals_idx_length_;

		// Buffers
		QOpenGLBuffer vbo_;
		QOpenGLBuffer nbo_; // for normals
		QOpenGLBuffer ibo_;
		QOpenGLBuffer nibo_; // normal indices buffer

		QOpenGLVertexArrayObject vao_;

		// This is where the actual file parsing will happen, instead of in the constructor.
		void parse_file(std::string filepath);

		// Helpers that set the GL arrays using the temp vectors created from parse_file.
		void set_vertices(std::vector<float> v);
		void set_normals(std::vector<float> n);
		void set_v_indices(std::vector<unsigned int> v_idx);
		void set_n_indices(std::vector<unsigned int> n_idx);

	public:
		ObjFileParser();
		// Constructor that takes in filepath to a .obj file.
		ObjFileParser(std::string filepath);
		~ObjFileParser();

		void setup_buffers();

		size_t vertices_length();
		size_t normals_length();
		size_t vertex_indices_length();
		size_t normal_indices_length();

		QOpenGLBuffer get_vbo();
		QOpenGLBuffer get_nbo(); 
		QOpenGLBuffer get_ibo();
		QOpenGLBuffer get_nibo(); 
		QOpenGLVertexArrayObject& get_vao();

		// Determines if the two parsers are the same. For this Assignment, we'll check if the filepaths they read from are the same.
		bool equals(ObjFileParser& other);

		bool has_setup_buffers();
};