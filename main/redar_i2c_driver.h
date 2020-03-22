#ifndef _REDAR_I2C_DRIVER_
#define _REDAR_I2C_DRIVER_


#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_BUS_FREQUENCY_HZ 1000000
#define ACK_CHECK_EN 0x1            
#define ACK_CHECK_DIS 0x0          
#define ACK_VAL 0x0               
#define NACK_VAL 0x1 
#define I2C_MASTER_TIMEOUT_MS 1000

#include <stdint.h>


    void REDAR_I2CInit(void);
    int REDAR_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data);
    int REDAR_I2CWrite(uint8_t slaveAddr,uint16_t writeAddress, uint16_t data);
#endif