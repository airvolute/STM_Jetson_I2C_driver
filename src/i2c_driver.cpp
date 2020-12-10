#include "i2c_driver.h"


int I2CDriver::I2cOpen(const char * device)
{
  i2c_fd = open(device, O_RDWR);
  if (i2c_fd < 0)
  {
    perror(device);
    return -1;
  }
  return 0;
}
 
 /*I2cGetData sends write message as ADDR+REG+MSG*/
int I2CDriver::I2cGetData(uint8_t address, uint8_t reg, uint8_t * read_buffer, uint8_t length)
{
  uint8_t regp[] = {reg};
  struct i2c_msg messages[] = {
    { address, I2C_M_WR, 1, regp},
    { address, I2C_M_RD, length, read_buffer},
  };
  struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };
  int result = ioctl(i2c_fd, I2C_RDWR, &ioctl_data);
  if (result != 2)
  {
    perror("I2cGetData error");
    return -1;
  }
  return 0;
}

/*I2cSetData sends write message as single ADDR+REG+MSG*/
 int I2CDriver::I2cSetData(uint8_t address, uint8_t reg, uint8_t * buffer, uint8_t length)
{
  uint8_t send_buff[length+1];
  send_buff[0]=reg;
  std::memcpy(&send_buff[1],buffer,length);
  int result = I2CDriver::I2CWriteSingle(address, &send_buff[0], length+1);
  if (result != 0)
  {
    perror("I2cSetData error");
    return -1;
  }
  return 0;
}

/*OLD_Function write message was splitted (ADDR+REG,ADDR+MSG)*/
/*int I2CDriver::I2cSetData(uint8_t address, uint8_t reg, uint8_t * buffer, uint8_t length)
{
  uint8_t regp[] = {reg};
  struct i2c_msg messages[] = {
    { address, I2C_M_WR, 1, regp},
    { address, I2C_M_WR, length, buffer},
  };
  struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };
  int result = ioctl(i2c_fd, I2C_RDWR, &ioctl_data);
  if (result != 2)
  {
    perror("failed to get variables");
    return -1;
  }
  return 0;
}*/
 
int I2CDriver::I2CWriteSingle(uint8_t address, uint8_t * buffer, uint8_t length)
{
  struct i2c_msg message = { address, I2C_M_WR , length, buffer };
  struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
  int result = ioctl(i2c_fd, I2C_RDWR, &ioctl_data);
  if (result != 1)
  {
    perror("failed to set target");
    return -1;
  }
  return 0;
}

uint8_t I2CDriver::I2cCalculateChecksum(uint8_t* source_ptr, uint8_t message_length)
{
  uint8_t checksum=0;
	uint8_t* data_pointer=source_ptr;
	for(int i=0;i<message_length;i++)
	{
		checksum += *data_pointer;
		data_pointer++;
	}
	checksum = (~checksum);
	return checksum;
}


int I2CDriver::I2cClose()
{
  close(i2c_fd);
  return 0;
}

I2CDriver::~I2CDriver(){
  close(i2c_fd);
}