#ifndef VIDEORENDERER_H
#define VIDEORENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QMutex>
#include <vector>

class VideoRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit VideoRenderer(QWidget *parent = nullptr);
    ~VideoRenderer();

    // Update video frame
    void updateFrame(const uint8_t *data, int width, int height);
    
    // Set video dimensions
    void setVideoDimensions(int width, int height);
    
    // Rotation control
    void setRotation(int angle);
    int getRotation() const { return rotationAngle; }
    
    // Get video dimensions
    int getVideoWidth() const { return videoWidth; }
    int getVideoHeight() const { return videoHeight; }

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    // OpenGL resources
    QOpenGLShaderProgram *shaderProgram;
    GLuint textureID;
    QOpenGLBuffer *vbo;
    QOpenGLBuffer *ebo;
    
    // Video properties
    int videoWidth;
    int videoHeight;
    int rotationAngle;
    
    // Frame data
    QMutex frameMutex;
    std::vector<uint8_t> frameData;
    bool hasNewFrame;
    
    // Transformation matrix
    QMatrix4x4 transformMatrix;
    
    // Helper functions
    void setupVertexData();
    void updateTransformMatrix();
};

#endif // VIDEORENDERER_H
