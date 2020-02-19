#include "BasicWidget.h"
#include <iostream>

// I'm storing these here, because if I try to create them in the constructor or elsewhere, 
// it doesn't seem to create them properly, and can't parse it at all. Don't know why!
// Was originally planning on storing a map of these parsers and setting the curr_key to different ones based
// on key clicks (i.e. 1 would go to bunny, 2 to monkey, etc.).
ObjFileParser* bunny = new ObjFileParser("../objects/bunny.obj");
ObjFileParser* monkey = new ObjFileParser("../objects/monkey.obj");
ObjFileParser* cube = new ObjFileParser("../objects/cube.obj");

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    //this->curr_key_ = "bunny";
    //std::cout << "testing" << std::endl;
    //ObjFileParser defo("../objects/bunny.obj");
    

    //qDebug() << "In constructor for basicwidget: " << defo.vertices_length();
    this->is_wireframe_mode_ = false;
    this->curr_obj_ = bunny;
    setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    // Delete stuff in this->possible_obj_
    //vbo_.release();
    //vbo_.destroy();
    //nbo_.release();
    //nbo_.destroy();
    //ibo_.release();
    //ibo_.destroy();
    //nibo_.release();
    //nibo_.destroy();
    //vao_.release();
    //vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates

QString BasicWidget::vertexShaderString() const
{
    QString str =
        "#version 330\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 vertColor;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "  vertColor = color;\n"
        "}\n";
    return str;
}

QString BasicWidget::fragmentShaderString() const
{
    QString str =
        "#version 330\n"
        "in vec4 vertColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "  color = vertColor;\n"
        "}\n";
    return str;
}


void BasicWidget::createShader()
{
    QOpenGLShader vert(QOpenGLShader::Vertex);
    vert.compileSourceCode(vertexShaderString());
    QOpenGLShader frag(QOpenGLShader::Fragment);
    frag.compileSourceCode(fragmentShaderString());
    bool ok = shaderProgram_.addShader(&vert);
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.addShader(&frag);
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.link();
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO
  // Handle key events here.
    if (keyEvent->key() == Qt::Key_1)
    {
        // First solution
        //this->curr_key_ = "bunny";
        //if (!this->possible_obj_[this->curr_key_]->is_set())
        //{
        //    this->possible_obj_[this->curr_key_] = new ObjFileParser("../objects/bunny.obj");
        //}

        // Second
        this->curr_obj_ = bunny;
        this->initialize_parser();
        update();  // We call update after we handle a key press to trigger a redraw when we are ready
    }
    else if (keyEvent->key() == Qt::Key_2)
    {
        // first solution
        //this->curr_key_ = "monkey";
        //if (!this->possible_obj_[this->curr_key_]->is_set())
        //{
        //    this->possible_obj_[this->curr_key_] = new ObjFileParser("../objects/monkey.obj");
        //}

        // second solution
        this->curr_obj_ = monkey;
        this->initialize_parser();
        update();  // We call update after we handle a key press to trigger a redraw when we are ready
    }
    else if (keyEvent->key() == Qt::Key_3)
    {
        this->curr_obj_ = cube;
        this->initialize_parser();
        update();
    }
    else if (keyEvent->key() == Qt::Key_W)
    {
        // I'm choosing to have the W key toggle between wireframe and non-wireframe.
        this->is_wireframe_mode_ = !this->is_wireframe_mode_;
        update();
    }
    else if (keyEvent->key() == Qt::Key_Q)
    {
        qDebug() << "Q key hit, quitting program.";
        exit(0);
    }
    else
    {
        qDebug() << "You Pressed an unsupported Key!";
    }
  // ENDTODO
}
void BasicWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    QOpenGLContext* curContext = this->context();
    qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
    qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
    qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
    qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    createShader();

    this->initialize_parser();
  //shaderProgram_.bind();

  //this->curr_obj_->setup_buffers();

  //// vbo will store vertex and color info.
  //vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  //vbo_.create();
  //// Bind our vbo inside our vao
  //vbo_.bind();
  //GLfloat* vertices = curr_obj_.get_verts_gl();
  //vbo_.allocate(vertices, curr_obj_.get_vertices().size() * sizeof(GL_FLOAT));

  //nbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  //nbo_.create();
  //nbo_.bind();
  //GLfloat* normals = curr_obj_.get_norms_gl();
  //nbo_.allocate(normals, curr_obj_.get_normals().size() * sizeof(GL_FLOAT));

  //// TODO:  Generate our index buffers
  //ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  //ibo_.create();
  //ibo_.bind();
  //GLuint* v_indices = curr_obj_.get_v_idx_gl();

  //ibo_.allocate(v_indices, curr_obj_.get_v_indices().size() * sizeof(GL_UNSIGNED_INT));

  //nibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  //nibo_.create();
  //nibo_.bind();
  //GLuint* n_indices = curr_obj_.get_n_idx_gl();
  //nibo_.allocate(n_indices, curr_obj_.get_n_indices().size() * sizeof(GL_UNSIGNED_INT));
  // ENDTODO

  // Create a VAO to keep track of things for us.
  //QOpenGLVertexArrayObject& curr_vao = this->curr_obj_->get_vao();
  //curr_vao.create();
  //curr_vao.bind();
  //this->curr_obj_->get_vbo().bind();
  //// verts
  //shaderProgram_.enableAttributeArray(0);
  //shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 7 * sizeof(GL_FLOAT));
  //  
  //// colors
  //shaderProgram_.enableAttributeArray(1);
  //shaderProgram_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GL_FLOAT), 4, 7 * sizeof(GL_FLOAT));
  //this->curr_obj_->get_nbo().bind();
  //this->curr_obj_->get_ibo().bind();
  //this->curr_obj_->get_nibo().bind();

  //// Releae the vao THEN the vbo
  //curr_vao.release();
  //shaderProgram_.release();

  glViewport(0, 0, width(), height());
}

void BasicWidget::initialize_parser()
{
    // If we already set up the current ObjFileParser, don't do anything.
    if (this->curr_obj_->has_setup_buffers()) return;

    makeCurrent();

    shaderProgram_.bind();

    this->curr_obj_->setup_buffers();

    QOpenGLVertexArrayObject& curr_vao = this->curr_obj_->get_vao();
    curr_vao.create();
    curr_vao.bind();
    this->curr_obj_->get_vbo().bind();
    // verts
    shaderProgram_.enableAttributeArray(0);
    shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 7 * sizeof(GL_FLOAT));

    // colors
    shaderProgram_.enableAttributeArray(1);
    shaderProgram_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GL_FLOAT), 4, 7 * sizeof(GL_FLOAT));
    this->curr_obj_->get_nbo().bind();
    this->curr_obj_->get_ibo().bind();
    this->curr_obj_->get_nibo().bind();

    // Releae the vao THEN the vbo
    curr_vao.release();
    shaderProgram_.release();
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO:  render.
  shaderProgram_.bind();
  //this->curr_obj_->equals(*bunny) 
  //    ? vao_.bind()
  //    : vao_m_.bind();
  this->curr_obj_->get_vao().bind();

  this->is_wireframe_mode_ 
      ? glDrawElements(GL_LINES, curr_obj_->vertex_indices_length(), GL_UNSIGNED_INT, 0)
      : glDrawElements(GL_TRIANGLES, curr_obj_->vertex_indices_length(), GL_UNSIGNED_INT, 0);

  //this->curr_obj_->equals(*bunny)
  //    ? vao_.release()
  //    : vao_m_.release();
  this->curr_obj_->get_vao().release();

  shaderProgram_.release();
}
