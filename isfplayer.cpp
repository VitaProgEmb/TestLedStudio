#include "isfplayer.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector2D>
#include <QVector4D>
#include <QDebug>

// Вертексный шейдер, генерирующий полноэкранный треугольник без буферов
const char* isf_v_shader = R"(
    #version 330 core
    void main() {
        float x = -1.0 + float((gl_VertexID & 1) << 2);
        float y = -1.0 + float((gl_VertexID & 2) << 1);
        gl_Position = vec4(x, y, 0.0, 1.0);
    }
)";

void ISFPlayer::initializeGL() {
    initializeOpenGLFunctions();
    m_timer.start();
    glClearColor(0, 0, 0, 1);
}

void ISFPlayer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}


void ISFPlayer::setUniformVec2(const QString &name, const QVector2D &val) {
    // Сохраняем в мапу векторов (мы ее создавали как QVector4D для универсальности)
    m_uniformValues[name] = QVector4D(val.x(), val.y(), 0.0f, 0.0f);
    update();
}

void ISFPlayer::loadFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << path;
        return;
    }

    QString fullContent = file.readAll();
    file.close();

    // 1. Извлекаем JSON заголовок
    int start = fullContent.indexOf("/*");
    int end = fullContent.indexOf("*/");
    QString jsonHeader = "";
    QString rawSource = fullContent;

    if (start != -1 && end != -1) {
        jsonHeader = fullContent.mid(start + 2, end - (start + 2));
        rawSource = fullContent.mid(end + 2);
    }

    // 2. Парсим INPUTS для создания пролога
    QJsonDocument doc = QJsonDocument::fromJson(jsonHeader.toUtf8());
    QJsonObject root = doc.object();
    QJsonArray inputs = root["INPUTS"].toArray();



    QString prologue = "#version 330 core\n";
    prologue += "uniform float TIME;\n";
    prologue += "uniform vec2 RENDERSIZE;\n";
    prologue += "out vec4 fragColor;\n";

    m_uniforms.clear();
    m_uniformValues.clear();

    for (const QJsonValue &v : inputs) {
        QJsonObject input = v.toObject();
        QString name = input["NAME"].toString();
        QString type = input["TYPE"].toString();

        if (type == "float") {
            prologue += QString("uniform float %1;\n").arg(name);
            m_uniforms[name] = input["DEFAULT"].toDouble(0.0);
        }
        else if (type == "point2D") {
            prologue += QString("uniform vec2 %1;\n").arg(name); // point2D это vec2!
            QJsonArray def = input["DEFAULT"].toArray();
            m_uniformValues[name] = QVector4D(def.at(0).toDouble(0.5), def.at(1).toDouble(0.5), 0.0, 0.0);
        }
        else if (type == "color") {
            prologue += QString("uniform vec4 %1;\n").arg(name);
            QJsonArray def = input["DEFAULT"].toArray();
            m_uniformValues[name] = QVector4D(def.at(0).toDouble(1.0), def.at(1).toDouble(1.0),
                                              def.at(2).toDouble(1.0), def.at(3).toDouble(1.0));
        }
        else if (type == "bool") {
            prologue += QString("uniform bool %1;\n").arg(name);
            m_uniforms[name] = input["DEFAULT"].toBool() ? 1.0f : 0.0f;
        }
    }

    prologue += "#define isf_FragNormCoord (gl_FragCoord.xy / RENDERSIZE)\n";
    prologue += "#line 1\n";

    // 3. Заменяем старый gl_FragColor и компилируем
    rawSource.replace("gl_FragColor", "fragColor");
    loadShader(jsonHeader, prologue + rawSource);
    emit shaderLoaded(inputs);
}

void ISFPlayer::loadShader(const QString &jsonHeader, const QString &fragmentSource) {
    if (!isValid()) return;
    makeCurrent();

    if (m_program) delete m_program;
    m_program = new QOpenGLShaderProgram(this);

    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, isf_v_shader))
        qDebug() << "Vertex Error:" << m_program->log();

    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSource))
        qDebug() << "Fragment Error:" << m_program->log();

    if (!m_program->link())
        qDebug() << "Link Error:" << m_program->log();

    doneCurrent();
    update();
}

void ISFPlayer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!m_program || !m_program->isLinked()) return;

    m_program->bind();

    // Стандартные Uniforms
    m_program->setUniformValue("TIME", (float)m_timer.elapsed() / 1000.0f);
    m_program->setUniformValue("RENDERSIZE", QVector2D(width(), height()));

    // Динамические Uniforms (из слайдеров или JSON)
    for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it) {
        m_program->setUniformValue(it.key().toUtf8().constData(), it.value());
    }

    // Отдельно для векторов (цветов)
    for (auto it = m_uniformValues.begin(); it != m_uniformValues.end(); ++it) {
        m_program->setUniformValue(it.key().toUtf8().constData(), it.value());
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_program->release();
    update();
}
