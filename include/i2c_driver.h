/**
  ******************************************************************************
  * File Name          : i2c_driver.h
  * Description        : Header file of i2c_driver for nvidia jetson
  ******************************************************************************
  * INSTRUCTIONS
  * -use only GetData and SetData functions
  * -read from device msg composition: 1.|ADDR+W|REG| 2.|ADDR+R|READ_DATA_FROM_DEVICE|
  * -write to  device msg composition: 1.|ADDR+W|REG|WRITE_DATA_TO_DEVICE|    
  ******************************************************************************
  */
#pragma once

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cstdint>
#include <cstring>

#define I2C_M_WR        0 
//#define I2C_M_RD        1
//#define I2C_RDWR        0x707



class I2CDriver
{
    private:      

        int i2c_fd;

    public:  

        int I2cOpen(const char * device);

        int I2cGetData(uint8_t address, uint8_t reg, uint8_t * read_buffer, uint8_t length);

        int I2cSetData(uint8_t address, uint8_t reg, uint8_t * buffer, uint8_t length);        

        int I2CWriteSingle(uint8_t address, uint8_t * buffer, uint8_t length);

        uint8_t I2cCalculateChecksum(uint8_t* source_ptr, uint8_t message_length);

        int I2cClose();

        ~I2CDriver();

};