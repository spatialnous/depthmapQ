// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "agltriangles.h"

#include <math.h>

// clang-format off
static const char *vertexShaderSourceCore = // auto-format hack
        "#version 150\n"
        "in vec4 vertex;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSourceCore = // auto-format hack
        "#version 150\n"
        "in vec4 colour;\n"
        "out highp vec4 fragColor;\n"
        "void main() {\n"
        "   fragColor = colour;\n"
        "}\n";

static const char *vertexShaderSource = // auto-format hack
        "attribute vec4 vertex;\n"
        "attribute vec4 colour;\n"
        "varying vec4 fragColour;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "   fragColour = colour;\n"
        "}\n";

static const char *fragmentShaderSource = // auto-format hack
        "varying highp vec4 fragColour;\n"
        "void main() {\n"
        "   gl_FragColor = fragColour;\n"
        "}\n";
// clang-format on

void AGLTriangles::loadTriangleData(
    const std::vector<std::pair<std::vector<Point2f>, QRgb>> &triangleData) {

    init(static_cast<int>(triangleData.size()));

    for (auto &triangle : triangleData) {
        float r = static_cast<float>(qRed(triangle.second)) / 255.0f;
        float g = static_cast<float>(qGreen(triangle.second)) / 255.0f;
        float b = static_cast<float>(qBlue(triangle.second)) / 255.0f;
        for (auto &point : triangle.first) {
            add(QVector3D(static_cast<float>(point.x), static_cast<float>(point.y), 0.0f),
                QVector3D(r, g, b));
        }
    }
}

void AGLTriangles::setupVertexAttribs() {
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                             DATA_DIMENSIONS * static_cast<GLsizei>(sizeof(GLfloat)), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                             DATA_DIMENSIONS * static_cast<GLsizei>(sizeof(GLfloat)),
                             (void *)(3 * sizeof(GLfloat)));
    m_vbo.release();
}

void AGLTriangles::initializeGL(bool m_core) {
    if (m_data.size() == 0)
        return;
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("colour", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(constData(), m_count * static_cast<GLsizei>(sizeof(GLfloat)));

    setupVertexAttribs();
    m_program->release();
    m_built = true;
}

void AGLTriangles::updateGL(bool m_core) {
    if (m_program == 0) {
        // has not been initialised yet, do that instead
        initializeGL(m_core);
    } else {
        m_vbo.bind();
        m_vbo.allocate(constData(), m_count * static_cast<GLsizei>(sizeof(GLfloat)));
        m_vbo.release();
        m_built = true;
    }
}

void AGLTriangles::cleanup() {
    if (!m_built)
        return;
    m_vbo.destroy();
    delete m_program;
    m_program = 0;
}

void AGLTriangles::paintGL(const QMatrix4x4 &mProj, const QMatrix4x4 &m_mView,
                           const QMatrix4x4 &m_mModel) {
    if (!m_built)
        return;
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, mProj);
    m_program->setUniformValue(m_mvMatrixLoc, m_mView * m_mModel);

    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, vertexCount());

    m_program->release();
}

void AGLTriangles::add(const QVector3D &v, const QVector3D &c) {
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = c.x();
    *p++ = c.y();
    *p++ = c.z();
    m_count += DATA_DIMENSIONS;
}
