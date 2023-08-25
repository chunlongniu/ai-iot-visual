#include <Arduino.h>
#include <ArduinoJson.h>
#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "HardwareSerial.h"
#include "WiFiType.h"
#include "HTTPClient.h"
#include "esp32-hal.h"
#include "action.hpp"
#include <WiFi.h>
#include <cstdio>

class WiFiConfig {
    private:
        const char* _ssid;
        const char* _passwd;
    public:
        WiFiConfig(const char* ssid, const char* passwd) {
            _ssid   = ssid;
            _passwd = passwd;
        }

        void setup() {
            WiFi.begin(_ssid, _passwd);
            Serial.println("Connecting");
            while(WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }
            Serial.println("");
            Serial.print("Connect to WiFi network with IP Address:");
            Serial.println(WiFi.localIP());
        }
};


class EHTTPClient {
    private:
        const char* _baseURL;
    public:
        EHTTPClient(const char* baseURL):_baseURL(baseURL) {}
        void post(enum action::Action _action, unsigned char* frame, unsigned int frame_size) {
            char buffer[sizeof(_action) + frame_size + 3];
            if (WiFi.status() == WL_CONNECTED) {
                HTTPClient http;
                http.begin(_baseURL);
                http.addHeader("Content-Type", "text/plain");
                sprintf(buffer, "%d:%s", _action, frame); 
                Serial.println(buffer);
                int resp_code = http.POST(buffer);

                if (resp_code > 0) {
                    String resp = http.getString();
                    Serial.println(resp_code);
                    Serial.println(resp);
                }
                http.end();
            }
        }
};

