#include "stdio.h"
#include "string"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "core.hpp"

#define _WIFI_SSID "XIAOXINXIN-TPLINK"
#define _WIFI_PASSWD "Cyy879200*$$"
const char *MAIN_TAG = "main function";


wifi_config_t init_wifi_config()
{
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = _WIFI_SSID,
            .password = _WIFI_PASSWD,
            .threshold = {
                .authmode = WIFI_AUTH_WPA2_PSK,
            },
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    return wifi_config;
}

extern "C" void app_main()
{
    ESP_LOGI(MAIN_TAG, " BEGIN ....");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    // wifi_config_t config = init_wifi_config();
    // ESP_LOGI(MAIN_TAG, "ESP_WIFI_MODE_STA");
    // wifi::setup(config);

    // esp_http_client_config_t http_client_config = {
    //     .url = "http://10.2.0.11:5000/devices/",
    //     .auth_type = HTTP_AUTH_TYPE_BASIC,
    // };
    // http::prepare(http_client_config);
    // mqtt::app_start(mqtt::MQTT_BROKER_ENDPOINT);

    motor::config_led();
    while(true) {
        ESP_LOGI(MAIN_TAG, "MOTOR UP ... ");
        motor::set_motor1_up();
        motor::set_motor2_down();
        vTaskDelay(100);
        ESP_LOGI(MAIN_TAG, "MOTOR DOWN ... ");
        motor::set_motor1_down();
        motor::set_motor2_up();
        vTaskDelay(100);
    }
  
}

