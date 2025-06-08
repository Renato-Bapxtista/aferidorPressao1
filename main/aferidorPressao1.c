#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_bmp280.h" // Inclua a biblioteca BMP280
#include "ssd1306.h"    // Inclua a biblioteca SSD1306

#define I2C_MASTER_SCL_IO 22      // GPIO para SCL
#define I2C_MASTER_SDA_IO 21      // GPIO para SDA
#define I2C_MASTER_FREQ_HZ 100000 // Frequência do I2C
#define BMP280_I2C_ADDRESS 0x76   // Endereço do BMP280

static const char *TAG = "BMP280_OLED";

void app_main(void)
{
    // Inicializa o I2C
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);

    // Inicializa o BMP280
    bmp280_t bmp;
    if (bmp280_init(&bmp, I2C_NUM_0, BMP280_I2C_ADDRESS) != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao inicializar BMP280");
        return;
    }

    // Inicializa a tela OLED
    ssd1306_init(); // Inicializa a tela OLED

    while (1)
    {
        float temperature, pressure;
        bmp280_read(&bmp, &temperature, &pressure);

        // Limpa a tela OLED
        ssd1306_clear_screen();

        // Exibe os dados na tela OLED
        ssd1306_set_cursor(0, 0);
        ssd1306_draw_string("Temp: %.2f C", temperature);
        ssd1306_draw_string("Press: %.2f hPa", pressure / 100.0); // Converte Pa para hPa
        ssd1306_display();

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
