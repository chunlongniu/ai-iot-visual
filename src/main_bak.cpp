#include "esp32-hal-gpio.h"
#include "i2c.hpp"
#include "http/client.hpp"
#include "adapter.hpp"
#include <Arduino.h>
#include <camera.hpp>
#include "HTTPClient.h"
#include <Arduino_JSON.h>

using namespace fifo_camera;
using namespace ov2640;
unsigned long timeDelay = 5000;

const char* ssid = "XIAOXINXIN-TPLINK";
const char* passwd = "Cyy879200*$$";
const char* baseURL = "http://10.2.0.11:5000/devices";


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
    //Serial.begin(9600);
    wifi.setup();
}

void loop() {
    while(true) {
        Serial.println("request ---");
        String payload = "";
        HTTPClient http_client;
        http_client.addHeader("Content-Type", "application/json");
        http_client.begin(baseURL);
        int res_code = http_client.GET();
        if (res_code > 0) {
            Serial.println(res_code);
            payload = http_client.getString();
            Serial.println(payload);
        } else {
            Serial.println("error");
        }
        http_client.end();
        delay(timeDelay);
    }
}
