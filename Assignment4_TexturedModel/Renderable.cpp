#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>
#include "ObjFileParser.h"
#include <iostream>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25), origin_(QVector3D(0, 0, 0))
{
	rotationAngle_ = 0.0;
}

// Uses the given path to read data about an obj file, uses that to initialize data.
Renderable::Renderable(std::string path) : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25), origin_(QVector3D(0, 0, 0))
{
	std::cout << "before parser" << std::endl;
	ObjFileParser* parser = new ObjFileParser(path);

	this->init(parser->get_verts(), parser->get_indices(), parser->get_vertex_size(), parser->get_texture_file());
	rotationAngle_ = 0.0;

	delete parser;
}

Renderable::~Renderable()
{
	if (texture_.isCreated())
	{
		texture_.destroy();
	}
	if (vbo_.isCreated())
	{
		vbo_.destroy();
	}
	if (ibo_.isCreated())
	{
		ibo_.destroy();
	}
	if (vao_.isCreated())
	{
		vao_.destroy();
	}
}

void Renderable::createShaders()
{
	QString vertexFilename = "../../vert.glsl";
	bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
	if (!ok)
	{
		qDebug() << shader_.log();
	}
	QString fragmentFilename = "../../frag.glsl";
	ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
	if (!ok)
	{
		qDebug() << shader_.log();
	}
	ok = shader_.link();
	if (!ok)
	{
		qDebug() << shader_.log();
	}
}

// TODO: Still need to generalize this more, allow for multiple types of renderables.... maybe something other than init.
void Renderable::init(const QVector<float> vert_info, const QVector<unsigned int> indices, const size_t vertex_size, const QString& textureFile)
{
	modelMatrix_.setToIdentity();

	// Load texture
	texture_.setData(QImage(textureFile));

	// Number of triangles to draw
	numTris_ = indices.size() / 3;
	std::cout << "got before buffers shit" << std::endl;

	int numVBOEntries = vert_info.size();

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();
	std::cout << "got before buffers shit" << std::endl;

	// Create a temporary data array
	float* data = new float[numVBOEntries];
	for (int i = 0; i < numVBOEntries; ++i)
	{
		std::cout << vert_info.at(i) << std::endl;
		data[i] = vert_info.at(i);
	}
	vbo_.allocate(data, numVBOEntries * sizeof(float));
	delete[] data;

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// create a temporary array for our indexes
	unsigned int* idxAr = new unsigned int[indices.size()];
	for (int i = 0; i < indices.size(); ++i)
	{
		idxAr[i] = indices.at(i);
	}
	ibo_.allocate(idxAr, indices.size() * sizeof(unsigned int));
	delete[] idxAr;

	std::cout << "got past buffers shit" << std::endl;
	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();
	vbo_.release();
	ibo_.release();
}

// Old init, keeping just in case.
void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<QVector2D>& texCoords, const QVector<unsigned int>& indexes, const QString& textureFile)
{
	// NOTE:  We do not currently do anything with normals -- we just
	// have it here for a later implementation!
	// We need to make sure our sizes all work out ok.
	if (positions.size() != texCoords.size() ||
		positions.size() != normals.size())
	{
		qDebug() << "[Renderable]::init() -- positions size mismatch with normals/texture coordinates";
		return;
	}


	// Set our model matrix to identity
	modelMatrix_.setToIdentity();
	//modelMatrix_ = modelMatrix_ * QMatrix4x4(
	//	-1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, -1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//);
	// Load our texture.
	texture_.setData(QImage(textureFile));

	// set our number of trianges.
	numTris_ = indexes.size() / 3;

	// num verts (used to size our vbo)
	int numVerts = positions.size();
	vertexSize_ = 3 + 2;  // Position + texCoord
	int numVBOEntries = numVerts * vertexSize_;

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();
	// Create a temporary data array
	float* data = new float[numVBOEntries];
	for (int i = 0; i < numVerts; ++i)
	{
		data[i * vertexSize_ + 0] = positions.at(i).x();
		data[i * vertexSize_ + 1] = positions.at(i).y();
		data[i * vertexSize_ + 2] = positions.at(i).z();
		data[i * vertexSize_ + 3] = texCoords.at(i).x();
		data[i * vertexSize_ + 4] = texCoords.at(i).y();
	}
	vbo_.allocate(data, numVBOEntries * sizeof(float));
	delete[] data;

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// create a temporary array for our indexes
	unsigned int* idxAr = new unsigned int[indexes.size()];
	for (int i = 0; i < indexes.size(); ++i)
	{
		idxAr[i] = indexes.at(i);
	}
	ibo_.allocate(idxAr, indexes.size() * sizeof(unsigned int));
	delete[] idxAr;

	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();
	vbo_.release();
	ibo_.release();
}

void Renderable::update(const qint64 msSinceLastFrame)
{
	// For this lab, we want our polygon to rotate. 
	float sec = msSinceLastFrame / 1000.0f;
	float anglePart = sec * rotationSpeed_ * 360.f;
	rotationAngle_ += anglePart;
	while (rotationAngle_ >= 360.0)
	{
		rotationAngle_ -= 360.0;
	}
}

void Renderable::draw(const QMatrix4x4& view, const QMatrix4x4& projection)
{
	// Handles translations, such as if the renderable isn't drawn in the center.
	QMatrix4x4 translationMatrix;
	translationMatrix.setToIdentity();
	translationMatrix.translate(this->origin_);

	// Create our model matrix.
	QMatrix4x4 rotMatrix;
	rotMatrix.setToIdentity();
	rotMatrix.rotate(rotationAngle_, rotationAxis_);

	QMatrix4x4 modelMat = modelMatrix_ * translationMatrix * rotMatrix;
	// Make sure our state is what we want
	shader_.bind();
	// Set our matrix uniforms!
	QMatrix4x4 id;
	id.setToIdentity();
	shader_.setUniformValue("modelMatrix", modelMat);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	vao_.bind();
	texture_.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	texture_.release();
	vao_.release();
	shader_.release();
}

void Renderable::setModelMatrix(const QMatrix4x4& transform)
{
	modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D& axis)
{
	rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
	rotationSpeed_ = speed;
}

void Renderable::setOrigin(const QVector3D& origin)
{
	this->origin_ = origin;
}