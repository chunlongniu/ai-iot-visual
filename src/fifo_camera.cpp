#include "i2c.hpp"
#include <camera.hpp>

using namespace fifo_camera;

template<typename Camera, typename I2C>
void Esp32<Camera, I2C>::init() {
    _i2c->init();
    _camera->init();
}

template<typename Camera, typename I2C>
Esp32<Camera, I2C>::Esp32(PixelConfig* config, unsigned char* frame, Camera* camera, I2C* i2c) {
    _config = config;
    _frame  = frame;
    _camera = camera;
    _i2c    = i2c;
}

template<typename Camera, typename I2C>
void Esp32<Camera, I2C>::prepareCapture() {
    _camera->prepareCapture();
}

template<typename Camera, typename I2C>
void Esp32<Camera, I2C>::startCapture() {
    _camera->startCapture();
}

template<typename Camera, typename I2C>
void Esp32<Camera, I2C>::readFrame() {
    _camera->readFrame((*_frame), _config->x_res, _config->y_res, _config->bytes_per_pixel);
}
