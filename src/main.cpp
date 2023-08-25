#include "i2c.hpp"
#include <Arduino.h>
#include <camera.hpp>

using namespace fifo_camera;

//Esp32 esp32;

void setup() {
    Serial.begin(115200);
    i2c::I2C<esp32::SIOD , esp32::SIOC> _i2c;
    Camera<i2c::I2C<esp32::SIOD , esp32::SIOC>, esp32::RRST,
        esp32::WRST, esp32::RCK, esp32::WR, esp32::D0, esp32::D1,
        esp32::D2, esp32::D3, esp32::D4, esp32::D5, esp32::D6, esp32::D7> _camera(_i2c);


    // esp32::config.x_res = 160;
    // esp32::config.y_res = 120;
    // esp32::config.bytes_per_pixel = 2;
    // int frame_size = \
    //         esp32::config.x_res * esp32::config.y_res *
    //         esp32::config.bytes_per_pixel;
    // unsigned char frame[frame_size];
    //esp32.init(esp32::config, frame);
    Serial.println("Start");
}

void loop() {
}
