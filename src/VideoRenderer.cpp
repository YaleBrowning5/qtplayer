#include "VideoRenderer.h"
#include <QDebug>
#include <cstring>

VideoRenderer::VideoRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
    , shaderProgram(nullptr)
    , textureID(0)
    , vbo(nullptr)
    , ebo(nullptr)
    , videoWidth(0)
    , videoHeight(0)
    , rotationAngle(0)
    , hasNewFrame(false)
{
}

VideoRenderer::~VideoRenderer() {
    makeCurrent();
    
    if (shaderProgram) {
        delete shaderProgram;
    }
    
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
    
    if (vbo) {
        delete vbo;
    }
    
    if (ebo) {
        delete ebo;
    }
    
    doneCurrent();
}

void VideoRenderer::updateFrame(const uint8_t *data, int width, int height) {
    QMutexLocker locker(&frameMutex);
    
    videoWidth = width;
    videoHeight = height;
    
    size_t dataSize = width * height * 4; // RGBA
    frameData.resize(dataSize);
    memcpy(frameData.data(), data, dataSize);
    
    hasNewFrame = true;
    
    update(); // Trigger repaint
}

void VideoRenderer::setVideoDimensions(int width, int height) {
    videoWidth = width;
    videoHeight = height;
    update();
}

void VideoRenderer::setRotation(int angle) {
    rotationAngle = angle % 360;
    updateTransformMatrix();
    update();
}

void VideoRenderer::initializeGL() {
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Create and compile shaders
    shaderProgram = new QOpenGLShaderProgram(this);
    
    // Vertex shader
    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        
        out vec2 TexCoord;
        
        uniform mat4 transform;
        
        void main()
        {
            gl_Position = transform * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";
    
    // Fragment shader
    const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec2 TexCoord;
        
        uniform sampler2D textureSampler;
        
        void main()
        {
            FragColor = texture(textureSampler, TexCoord);
        }
    )";
    
    if (!shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "Vertex shader compilation failed:" << shaderProgram->log();
    }
    
    if (!shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qDebug() << "Fragment shader compilation failed:" << shaderProgram->log();
    }
    
    if (!shaderProgram->link()) {
        qDebug() << "Shader program linking failed:" << shaderProgram->log();
    }
    
    // Setup vertex data
    setupVertexData();
    
    // Create texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void VideoRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!shaderProgram || frameData.empty()) {
        return;
    }
    
    QMutexLocker locker(&frameMutex);
    
    // Upload texture data if we have a new frame
    if (hasNewFrame && !frameData.empty()) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, videoWidth, videoHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, frameData.data());
        hasNewFrame = false;
    }
    
    // Use shader program
    shaderProgram->bind();
    
    // Set transformation matrix
    shaderProgram->setUniformValue("transform", transformMatrix);
    
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shaderProgram->setUniformValue("textureSampler", 0);
    
    // Draw
    vbo->bind();
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    ebo->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    shaderProgram->release();
}

void VideoRenderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateTransformMatrix();
}

void VideoRenderer::setupVertexData() {
    // Vertex data: position (x, y, z) + texture coords (u, v)
    float vertices[] = {
        // positions        // texture coords
         1.0f,  1.0f, 0.0f,  1.0f, 0.0f,   // top right
         1.0f, -1.0f, 0.0f,  1.0f, 1.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,  0.0f, 0.0f    // top left
    };
    
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(vertices, sizeof(vertices));
    
    ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo->create();
    ebo->bind();
    ebo->allocate(indices, sizeof(indices));
}

void VideoRenderer::updateTransformMatrix() {
    transformMatrix.setToIdentity();
    
    if (videoWidth == 0 || videoHeight == 0) {
        return;
    }
    
    int w = width();
    int h = height();
    
    if (w == 0 || h == 0) {
        return;
    }
    
    // Calculate video aspect ratio considering rotation
    float videoAspect;
    if (rotationAngle == 90 || rotationAngle == 270) {
        // Swap width and height for 90/270 degree rotation
        videoAspect = (float)videoHeight / (float)videoWidth;
    } else {
        videoAspect = (float)videoWidth / (float)videoHeight;
    }
    
    float windowAspect = (float)w / (float)h;
    
    // Calculate scale to fit video in window while maintaining aspect ratio
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    
    if (videoAspect > windowAspect) {
        // Video is wider than window
        scaleY = windowAspect / videoAspect;
    } else {
        // Video is taller than window
        scaleX = videoAspect / windowAspect;
    }
    
    // Apply rotation
    transformMatrix.rotate(rotationAngle, 0, 0, 1);
    
    // Apply scale
    transformMatrix.scale(scaleX, scaleY);
}
