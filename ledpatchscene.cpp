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

using namespace Magnum::Math::Literals;

LedPatchScene::LedPatchScene() {
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    _mesh = Magnum::MeshTools::compile(Magnum::Primitives::squareSolid());
    _timeline.start();
}

LedPatchScene::~LedPatchScene() {}

void LedPatchScene::viewportChanged(int width, int height) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, {width, height}});
}

void LedPatchScene::draw() {
    _timeline.nextFrame();
    float time = _timeline.previousFrameTime();

    Magnum::GL::Renderer::setClearColor(0x1a1a1a_rgbf);
    Magnum::GL::defaultFramebuffer.clear(
        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth
        );

    float startX = -((_cols - 1) * _spacing) / 2.0f;
    float startY = -((_rows - 1) * _spacing) / 2.0f;

    // Плавная пульсация яркости от синуса времени
    float pulse = 0.75f + std::sin(time * 3.0f) * 0.25f;

    for(int r = 0; r < _rows; ++r) {
        for(int c = 0; c < _cols; ++c) {
            float posX = startX + (c * _spacing);
            float posY = startY + (r * _spacing);

            Magnum::Matrix3 transformation =
                Magnum::Matrix3::translation({posX, posY}) *
                Magnum::Matrix3::scaling(Magnum::Vector2{_ledSize});

            _shader.setTransformationProjectionMatrix(transformation);

            if((r + c) % 2 == 0) {
                _shader.setColor(Magnum::Color3{0.0f, 1.0f * pulse, 0.0f});
            } else {
                _shader.setColor(Magnum::Color3{0.0f, 0.6f * pulse, 0.0f});
            }

            _shader.draw(_mesh);
        }
    }
}

void LedPatchScene::handleMouseClick(float x, float y) {
    float startX = -((_cols - 1) * _spacing) / 2.0f;
    float startY = -((_rows - 1) * _spacing) / 2.0f;

    for(int r = 0; r < _rows; ++r) {
        for(int c = 0; c < _cols; ++c) {
            float posX = startX + (c * _spacing);
            float posY = startY + (r * _spacing);

            if(x >= (posX - _ledSize) && x <= (posX + _ledSize) &&
                y >= (posY - _ledSize) && y <= (posY + _ledSize))
            {
                qDebug() << "Кликнули по светодиоду! Строка:" << r << "Столбец:" << c;
                return;
            }
        }
    }
}
