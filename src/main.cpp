#include "esp32-hal-gpio.h"
#include "i2c.hpp"
#include "http/client.hpp"
#include <Arduino.h>
#include <camera.hpp>

using namespace fifo_camera;

const char* ssid = "";
const char* passwd = "";
const char* baseURL = "";

Esp32<Camera<i2c::I2C<esp32::SIOD , esp32::SIOC>, esp32::RRST,
        esp32::WRST, esp32::RCK, esp32::WR, esp32::D0, esp32::D1,
        esp32::D2, esp32::D3, esp32::D4, esp32::D5, esp32::D6, esp32::D7>,
        i2c::I2C<esp32::SIOD, esp32::SIOC>> _esp32;

const int X_RES = 160;
const int Y_RES = 120;
const int BYTES_PER_PIXEL = 2;

PixelConfig config = {.x_res=X_RES, .y_res=Y_RES, .bytes_per_pixel=BYTES_PER_PIXEL};
i2c::I2C<esp32::SIOD , esp32::SIOC> _i2c;
Camera<i2c::I2C<esp32::SIOD , esp32::SIOC>, esp32::RRST,
    esp32::WRST, esp32::RCK, esp32::WR, esp32::D0, esp32::D1,
    esp32::D2, esp32::D3, esp32::D4, esp32::D5, esp32::D6, esp32::D7> _camera(_i2c);
const int frame_size = X_RES * Y_RES * BYTES_PER_PIXEL;

unsigned char frame[frame_size];

WiFiConfig wifi(ssid, passwd);
EHTTPClient ehttpClient(baseURL);

void setup() {
    Serial.begin(115200);
    wifi.setup();
    _esp32.setup(&config, frame, &_camera, &_i2c);
    ehttpClient.post(action::L , frame, frame_size);
    Serial.println("Start");
}

void loop() {
    while(!digitalRead(esp32::VSYNC));
    while (digitalRead(esp32::VSYNC));
    _esp32.prepareCapture();
    _esp32.startCapture();
    while(!digitalRead(esp32::VSYNC));
    _esp32.stopCapture();

    //color
    _esp32.readFrame();
    _esp32.resetFrame();
}
