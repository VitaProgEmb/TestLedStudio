#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

class ISFPlayer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    ISFPlayer(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
    void loadShader(const QString &jsonHeader, const QString &fragmentSource);
    void loadFile(const QString &path);
    void setUniformFloat(const QString &name, float value) {
        m_uniforms[name] = value;
    }
    QImage grabFrame() { return grabFramebuffer(); }
    void setUniformVec2(const QString &name, const QVector2D &val);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram *m_program = nullptr;
    QElapsedTimer m_timer;
    QMap<QString, float> m_uniforms;       // Для float и bool
    QMap<QString, QVector4D> m_uniformValues; // Для цветов

    // В реальном проекте здесь будет мапа для параметров из libisf
    // QMap<QString, float> m_parameters;
signals:
     void shaderLoaded(const QJsonArray &inputs);
};
