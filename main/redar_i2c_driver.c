#include "redar_i2c_driver.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

void REDAR_I2CInit()
{   
     i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = SCL_PIN,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_BUS_FREQUENCY_HZ
    };

    esp_err_t cret = i2c_param_config(I2C_NUM_0,&conf);
    esp_err_t iret = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    if(cret != ESP_OK || iret != ESP_OK)
    {
        ESP_LOGE("REDAR", "I2C INSTALL ERROR %i %i", cret, iret);
    }
}

int REDAR_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    uint8_t sa;                                                        
    int cnt = 0;
    int i = 0;
    uint8_t command[2] = {0,0};
    uint8_t * i2cData;
    int i2cData_len = 1664;
    i2cData = malloc(i2cData_len);
    uint16_t *p;
    
    p = data;
    sa = (slaveAddr << 1);
    command[0] = startAddress >> 8;
    command[1] = startAddress & 0x00FF;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    //slave address
    i2c_master_write_byte(cmd,sa | I2C_MASTER_WRITE, ACK_CHECK_EN);
    //device registers to be read
    i2c_master_write(cmd,command,2,ACK_CHECK_EN);

    i2c_master_start(cmd);
    //slave address before streaming
    i2c_master_write_byte(cmd,sa | I2C_MASTER_READ, ACK_CHECK_EN);
    //read all bytes -1 terminated with ACK
    i2c_master_read(cmd,i2cData,2*nMemAddressRead-1,ACK_VAL);
    //read final byte termated with NACK
    i2c_master_read_byte(cmd,i2cData+(2*nMemAddressRead-1),NACK_VAL);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
 

    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *p++ = (uint16_t)i2cData[i]*256 + (uint16_t)i2cData[i+1];
    }

    free(i2cData);

    if(ret != ESP_OK)
    {
        ESP_LOGE("REDAR", "I2C READ ERROR %i", ret);
    }
    return ret;   
} 

int REDAR_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    
    uint8_t sa;
    uint8_t command[4] = {0,0,0,0};
    static uint16_t dataCheck;
    

    sa = (slaveAddr << 1);
    command[0] = writeAddress >> 8;
    command[1] = writeAddress & 0x00FF;
    command[2] = data >> 8;
    command[3] = data & 0x00FF;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,sa | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write(cmd,command,4,ACK_CHECK_EN);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != ESP_OK)
    {
        ESP_LOGE("REDAR", "I2C WRITE ERROR");
    }
    
    REDAR_I2CRead(slaveAddr,writeAddress,1, &dataCheck);
    if ( dataCheck != data)
    {
        return -2;
    }   
    
    return 0;
}

