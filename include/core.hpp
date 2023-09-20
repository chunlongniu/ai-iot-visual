#include <cstdint>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/ledc.h"

#include "mqtt_client.h"

namespace wifi
{
    const uint16_t WIFI_REGRY_CONN_MAXNUM = 3;
    const uint32_t WIFI_CONNECTED_BIT = BIT0;
    const uint32_t WIFI_FAIL_BIT = BIT1;
    const char *TAG = "WIFI CONNECTION";
    int s_retry_num = 0;
    static EventGroupHandle_t s_wifi_event_group;
    static EventBits_t _bits;

    void event_handler(void *arg, esp_event_base_t event_base,
                       int32_t event_id, void *event_data)
    {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect();
        }
        else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            if (s_retry_num < WIFI_REGRY_CONN_MAXNUM)
            {
                esp_wifi_connect();
                s_retry_num++;
                ESP_LOGI(TAG, "retry to connect to the AP");
            }
            else
            {
                xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            }
            ESP_LOGI(TAG, "connect to the AP fail");
        }
        else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
        {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
            s_retry_num = 0;
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        }
    }

    void wifi_init_sta(void)
    {
        s_wifi_event_group = xEventGroupCreate();

        ESP_ERROR_CHECK(esp_netif_init());

        ESP_ERROR_CHECK(esp_event_loop_create_default());

        esp_netif_create_default_wifi_sta();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_t instance_got_ip;
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &event_handler,
                                                            NULL,
                                                            &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            &event_handler,
                                                            NULL,
                                                            &instance_got_ip));
    }

    void setup(wifi_config_t wifi_config)
    {
        wifi_init_sta();
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
        _bits = xEventGroupWaitBits(s_wifi_event_group,
                                    WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                    pdFALSE,
                                    pdFALSE,
                                    portMAX_DELAY);
    };

    bool getStatus(wifi_config_t wifi_config)
    {
        ESP_LOGI(TAG, "wifi_init_sta finished.");
        bool connected = false;
        if (_bits & WIFI_CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                     wifi_config.sta.ssid, wifi_config.sta.password);
            connected = true;
        }
        else if (_bits & WIFI_FAIL_BIT)
        {
            ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                     wifi_config.sta.ssid, wifi_config.sta.password);
        }
        else
        {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }
        return connected;
    }
}

namespace http
{

    /**
     *  esp_http_client_config_t config = {
     *      .url = "http://localhost:8080/devices"
     *  }
     *
     */

    const char *TAG = "HTTP Client";
    const uint32_t RESP_MAX_BUFF_SIZE = 1024;

    esp_err_t _http_event_handle(esp_http_client_event_t *event)
    {
        switch (event->event_id)
        {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", event->data_len, (char *)event->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", event->data_len);
            if (!esp_http_client_is_chunked_response(event->client))
            {
                printf("%.*s", event->data_len, (char *)event->data);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        }
        return ESP_OK;
    }

    static esp_http_client_handle_t _client;

    void prepare(const esp_http_client_config_t config)
    {
        esp_http_client_cleanup(_client);
        _client = esp_http_client_init(&config);
    }

    void get()
    {
        int ctxt_len = 0;
        char output_buffer[RESP_MAX_BUFF_SIZE];
        esp_http_client_set_method(_client, HTTP_METHOD_GET);
        esp_err_t err = esp_http_client_open(_client, 0);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to open HTTP connection: %s",
                     esp_err_to_name(err));
        }
        else
        {
            ctxt_len = esp_http_client_fetch_headers(_client);
            if (ctxt_len < 0)
            {
                ESP_LOGE(TAG, "HTTP client fetch headers failed");
            }
            else
            {
                int data_read = esp_http_client_read_response(
                    _client, output_buffer, RESP_MAX_BUFF_SIZE);
                if (data_read >= 0)
                {
                    ESP_LOGI(TAG,
                             "HTTP GET Status = %d, content_length = %" PRId64,
                             esp_http_client_get_status_code(_client),
                             esp_http_client_get_content_length(_client));
                    ESP_LOG_BUFFER_HEX(TAG, output_buffer, data_read);
                }
                else
                {
                    ESP_LOGE(TAG, "Failed to read response");
                }
            }
        }
    }

    void post(const char *url, const char *data, int length)
    {
        if (_client != NULL)
        {
            esp_http_client_set_url(_client, url);
            esp_http_client_set_method(_client, HTTP_METHOD_POST);
            esp_http_client_set_header(_client, "Content-Type", "application/json");
            esp_http_client_set_post_field(_client, data, length);
            esp_err_t err = esp_http_client_perform(_client);
            if (err == ESP_OK)
            {
                ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %" PRId64,
                         esp_http_client_get_status_code(_client),
                         esp_http_client_get_content_length(_client));
            }
            else
            {
                ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
            }
        }
        else
        {
            ESP_LOGI(TAG, "POST %s Failed, client is NULL", url);
        }
    }
}

namespace mqtt
{
    const char* TAG = "MQTT";

    void log_error_if_nonzero(const char *message, int error_code)
    {
        if (error_code != 0) {
            ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
        }
    }

    void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
        ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
        esp_mqtt_client_handle_t client = event->client;
        int msg_id;
        switch ((esp_mqtt_event_id_t)event_id)
        {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/mocrocontroller/0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            //msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
            {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
        }
    }

    const char* MQTT_BROKER_ENDPOINT = "mqtt://10.2.0.11:1883";

    void app_start(const char* mqtt_broker_endpoint)
    {
        esp_mqtt_client_config_t mqtt_cfg = {
            .broker = {
                .address = {
                    .uri = mqtt_broker_endpoint,
                }
            },
            .credentials = {
                .client_id = "123456",
                .set_null_client_id = false,
            }
        };
        esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
   
        /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
        esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
        esp_mqtt_client_start(client);
        
    }

}

#include "driver/gpio.h"
#include "freertos/queue.h"

namespace motor {
    const gpio_num_t MOTOR_1_GPIO_OUTPUT = GPIO_NUM_18;
    const gpio_num_t MOTOR_2_GPIO_OUTPUT = GPIO_NUM_25;
    const gpio_num_t MOTOR_DIRT_1_GPIO_OUTPUT = GPIO_NUM_17;
    const gpio_num_t MOTOR_DIRT_2_GPIO_OUTPUT = GPIO_NUM_26;
    const uint32_t LOWER = 0x0;
    const uint32_t HIGHER = 0x1;

    static void config_led(void) {
        gpio_reset_pin(MOTOR_1_GPIO_OUTPUT);
        gpio_reset_pin(MOTOR_2_GPIO_OUTPUT);
        gpio_reset_pin(MOTOR_DIRT_1_GPIO_OUTPUT);
        gpio_reset_pin(MOTOR_DIRT_2_GPIO_OUTPUT);
        gpio_set_direction(MOTOR_1_GPIO_OUTPUT, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_2_GPIO_OUTPUT, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_DIRT_1_GPIO_OUTPUT, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_DIRT_2_GPIO_OUTPUT, GPIO_MODE_OUTPUT);
    }

    void forward() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_DIRT_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_2_GPIO_OUTPUT, LOWER);
    }

    void backward() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_DIRT_1_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_DIRT_2_GPIO_OUTPUT, HIGHER);
    }

    void forward_left() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_2_GPIO_OUTPUT, LOWER);
    }

    void forward_right() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, HIGHER);
        gpio_set_level(MOTOR_DIRT_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_2_GPIO_OUTPUT, LOWER);
    }

    void stop_motor() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_1_GPIO_OUTPUT, LOWER);
        gpio_set_level(MOTOR_DIRT_2_GPIO_OUTPUT, LOWER);
    }

    void set_motor1_up() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, 1);
    }

    void set_motor1_down() {
        gpio_set_level(MOTOR_1_GPIO_OUTPUT, 0);
    }

    void set_motor2_up() {
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, 1);
    }
    
    void set_motor2_down() {
        gpio_set_level(MOTOR_2_GPIO_OUTPUT, 0);
    }
}
