#include "esp32-hal-gpio.h"
#include "i2c.hpp"
#include "http/client.hpp"
#include "adapter.hpp"
#include <Arduino.h>
#include <camera.hpp>

using namespace fifo_camera;
using namespace ov2640;

const char* ssid = "";
const char* passwd = "";
const char* baseURL = "";


NormalCamera::Sensor<ov2640::XCLK_GPIO_NUM, ov2640::Y2_GPIO_NUM, ov2640::Y3_GPIO_NUM,
                    ov2640::Y4_GPIO_NUM, ov2640::Y5_GPIO_NUM, ov2640::Y6_GPIO_NUM,
                    ov2640::Y7_GPIO_NUM, ov2640::Y8_GPIO_NUM, ov2640::Y9_GPIO_NUM,
                    ov2640::PCLK_GPIO_NUM, ov2640::HREF_GPIO_NUM,
                    ov2640::VSYNC_GPIO_NUM, ov2640::PWDN_GPIO_NUM,
                    ov2640::RESET_GPIO_NUM, ov2640::SIOD_GPIO_NUM,
                    ov2640::SIOC_GPIO_NUM> _sensor;

WiFiConfig wifi(ssid, passwd);

void setup() {
    Serial.begin(115200);
    wifi.setup();


}

void loop() {
}
