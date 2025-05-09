#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "QOpenGLFunctions"
#include <QObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QWidget>
#include <mutex>
#include <opencv.hpp>

extern "C" {
#include "libavcodec/avcodec.h"
}

/*
 *使用方式：初始化或当图像尺寸更改的时候调用 init_texture
 *        调用 paintMat 以cv::Mat 图像绘制
 */
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    GLWidget(QWidget* parent = nullptr);
    int init_texture(int w, int h);

    int checkInitState()
    {
        if (!GLisInit)
            return 1;
        if (!textIsInit)
            return 2;
        if (!projectionIsInit)
            return 3;
        return 0;
    }

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    std::mutex m_mutex;
    GLuint m_textureID; // OpenGL texture ID
    int m_frameWidth = 0; // Width of the frame
    int m_frameHeight = 0; // Height of the frame
    bool m_textureCreated = false; // Flag to check if texture has been created
    int m_width;
    int m_height;
    GLuint m_framebuffer;
    // GLuint m_shaderProgram;
    QOpenGLShaderProgram m_Program; // shader程序

    QOpenGLShaderProgram program; // shader程序
    GLuint unis[3] = { 0 }; // shader中yuv变量地址
    GLuint texs[3] = { 0 }; // openg的 texture地址

    // 材质内存空间
    unsigned char* datas[3] = { 0 };
    int width = 240;
    int height = 128;

    float imageWidth, imageHeight;
    void setProjection(int w, int h);
    bool GLisInit = false;
    bool textIsInit = false;
    bool projectionIsInit = false;
public slots:
    void paintMat(cv::Mat* mat);
    void paintFrame(std::shared_ptr<AVFrame>);
};

#endif // GLWIDGET_H
