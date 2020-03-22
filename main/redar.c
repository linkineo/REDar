#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "MLX90640_API.h"

#define TA_SHIFT 8
float emissivity = 0.95;
float tr;
unsigned char slaveAddress = 0x33;
static uint16_t eeMLX90640[832];
static uint16_t mlx90640Frame[834];
paramsMLX90640 mlx90640;
static float mlx90640To[768];
int status;
const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;


void app_main(void)
{
    printf("Starting IR Cam...\n");
    REDAR_I2CInit();
    status = MLX90640_DumpEE (slaveAddress, eeMLX90640);
    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);



    while(1)
    {
        // default mode is chess mode, only every second pixel will be updated
        //2 cycles needed for full image update
        MLX90640_GetFrameData (slaveAddress, mlx90640Frame);
        tr = MLX90640_GetTa(mlx90640Frame, &mlx90640) - TA_SHIFT;
        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

        for(int p=0;p< 768; p++)
        {
            ESP_LOGI("REDAR","T[%i]=%.2f", p, mlx90640To[p]);
        }

        vTaskDelay( xDelay );
    }

}
