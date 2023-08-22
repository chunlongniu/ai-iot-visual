#include <Arduino.h>
#include <camera.hpp>
#include <i2c.hpp>
#include "hal/gpio_types.h"

namespace esp32 {
    const int VSYNC = GPIO_NUM_32;
    const int SIOD  = GPIO_NUM_21;
    const int SIOC  = GPIO_NUM_22;

    const int RRST  = GPIO_NUM_17;
    const int WRST  = GPIO_NUM_16;
    const int RCK   = GPIO_NUM_17;
    const int WR    = GPIO_NUM_0;

    const int D0    = GPIO_NUM_13;
    const int D1    = GPIO_NUM_12;
    const int D2    = GPIO_NUM_14;
    const int D3    = GPIO_NUM_27;
    const int D4    = GPIO_NUM_26;
    const int D5    = GPIO_NUM_25;
    const int D6    = GPIO_NUM_35;
    const int D7    = GPIO_NUM_34;

    i2c::I2C<SIOD, SIOC> _i2c;
    fifo_camera::Camera<i2c::I2C<SIOD, SIOC>, RRST, WRST, RCK, WR, D0, D1, D2, D3, D4, D5, D6, D7> _camera(_i2c);
    fifo_camera::PixelConfig config;
};

void fifo_camera::Esp32::init() {
    esp32::_i2c.init();
    esp32::_camera.init();
};
