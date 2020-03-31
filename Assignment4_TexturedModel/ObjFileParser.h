#pragma once
#include <vector>
#include <string>
#include <QtOpenGL>
#include "MtlFileParser.h"

/**
* This class is used to parse an Obj file and store the vertices, normals and faces.
* Can be extended later to parse different types of lines.
*/
class ObjFileParser
{
	private:
		std::string filepath_;
		bool has_setup_buffers_;

		MtlFileParser* mtl;

		// Stores vertex, texture, other information eventually.
		QVector<float> compact_verts_;
		QVector<unsigned int> indices_;
		unsigned int vert_size_; // size of each vertex (right now 3 vertx, 2 textures)

		// For now, we have only one texture per object
		QOpenGLTexture texture_;

		// This is where the actual file parsing will happen, instead of in the constructor.
		void parse_file(std::string filepath);

	public:
		ObjFileParser();
		// Constructor that takes in filepath to a .obj file.
		ObjFileParser(std::string filepath);
		~ObjFileParser();

		QVector<float> get_verts();
		QVector<unsigned int> get_indices();
		unsigned int get_vertex_size();
		QString get_texture_file();


		// Determines if the two parsers are the same. For this Assignment, we'll check if the filepaths they read from are the same.
		bool equals(ObjFileParser& other);

};