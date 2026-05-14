#include "ledpatchscene.h"
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>
#include <cmath>
#include <QDebug>
#include <Magnum/GL/Buffer.h>
#include <Corrade/Containers/ArrayView.h>
#include <Magnum/GL/Context.h>
// #include <Magnum/GL/ContextState.h>
#include <Magnum/GL/Framebuffer.h>



using namespace Magnum::Math::Literals;

LedPatchScene::LedPatchScene() {
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    _mesh = Magnum::MeshTools::compile(Magnum::Primitives::squareSolid());
    _timeline.start();

    // Изначально вектор пуст — на экране нет ни одного диода!
}

LedPatchScene::~LedPatchScene() {}

void LedPatchScene::viewportChanged(int width, int height) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, {width, height}});
}

void LedPatchScene::draw(unsigned int qtFramebufferId) {
    _timeline.nextFrame();
    float time = _timeline.previousFrameTime();

    // 1. Обертываем ID буфера Qt в объект, который понимает Magnum
    Magnum::GL::Framebuffer qtFramebuffer = Magnum::GL::Framebuffer::wrap(
        qtFramebufferId,
        {{}, {Magnum::GL::defaultFramebuffer.viewport().size()}}
        );

    // 2. Активируем его и очищаем темно-серым цветом СТРОГО этот буфер
    qtFramebuffer.bind();
    Magnum::GL::Renderer::setClearColor(0x1a1a1a_rgbf);
    qtFramebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);

    if(_placedLeds.empty()) return;

    float pulse = 0.85f + std::sin(time * 5.0f) * 0.15f;

    // 3. ОТРИСОВКА СОЕДИНЯЮЩИХ ЛИНИЙ
    if(_placedLeds.size() > 1) {
        Magnum::GL::Mesh lineMesh;
        lineMesh.setPrimitive(Magnum::GL::MeshPrimitive::LineStrip);

        Magnum::GL::Buffer vertexBuffer;
        vertexBuffer.setData(Corrade::Containers::arrayView(_placedLeds.data(), _placedLeds.size()));

        lineMesh.addVertexBuffer(vertexBuffer, 0, Magnum::Shaders::FlatGL2D::Position{});
        lineMesh.setCount(_placedLeds.size());

        _shader.setTransformationProjectionMatrix(Magnum::Matrix3{});
        _shader.setColor(Magnum::Color3{0.0f, 0.5f, 0.8f}); // Синие линии
        _shader.draw(lineMesh);
    }

    // 4. ОТРИСОВКА САМИХ СВЕТОДИОДОВ
    for(const auto& ledPos : _placedLeds) {
        Magnum::Matrix3 transformation =
            Magnum::Matrix3::translation(ledPos) *
            Magnum::Matrix3::scaling(Magnum::Vector2{_ledSize});

        _shader.setTransformationProjectionMatrix(transformation);
        _shader.setColor(Magnum::Color3{0.0f, 1.0f * pulse, 0.5f * pulse}); // Зеленые леды
        _shader.draw(_mesh);
    }
}

void LedPatchScene::handleMouseClick(float x, float y) {
    // Создаем точку с координатами клика курсора
    Magnum::Vector2 clickPosition{x, y};

    // ГЛАВНОЕ ДЕЙСТВИЕ: добавляем новый светодиод в наш список!
    _placedLeds.push_back(clickPosition);

    qDebug() << "Поставили новый светодиод в координатах OpenGL: X =" << x << "Y =" << y;
    qDebug() << "Всего светодиодов на сцене:" << _placedLeds.size();
}
