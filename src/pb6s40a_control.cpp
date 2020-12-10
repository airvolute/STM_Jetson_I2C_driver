#include "pb6s40a_control.h"

/*DRONE CONTROL*/

int Pb6s40aDroneControl::EscSetState(uint8_t pesc_state){     
    uint8_t transmit_data[I2C2_ESC_STATE_REG_LENGTH+2]; //address + data + checksum  
    transmit_data[0]= I2C2_ESC_STATE_SET_REG;
    transmit_data[1]= pesc_state;
    transmit_data[2]= i2c_driver.I2cCalculateChecksum(&transmit_data[0],I2C2_ESC_STATE_REG_LENGTH+1);

    if(i2c_driver.I2cSetData(i2c_slave_address,I2C2_ESC_STATE_SET_REG,&transmit_data[1],I2C2_ESC_STATE_REG_LENGTH+1) != 0) 
    {     
        return 1;
    }else
    {
        return 0;
    }
}

int Pb6s40aDroneControl::EscGetState(uint8_t* pesc_state){  
    uint8_t status=0; 
    uint8_t received_data[I2C2_ESC_STATE_REG_LENGTH+1];    
    uint8_t checksum=0;   
    if(i2c_driver.I2cGetData(i2c_slave_address,I2C2_ESC_STATE_GET_REG,&received_data[0],(I2C2_ESC_STATE_REG_LENGTH+1)) != 0) 
    {      
        status = 1;
    }else
    {
        status = 0;
    }

    if(status==0)
    {
        checksum=i2c_driver.I2cCalculateChecksum(&received_data[0],I2C2_ESC_STATE_REG_LENGTH);
        if(checksum == received_data[I2C2_ESC_STATE_REG_LENGTH])
        {
            memcpy(pesc_state, &received_data[0], I2C2_ESC_STATE_REG_LENGTH); 
        }
        else status=2;
    }
    return status;
}

int Pb6s40aDroneControl::EscGetDataLogs(RUN_DATA_Struct* struct_pointer, uint8_t esc_number)
{
    uint8_t status=0;
    uint8_t received_data[I2C2_ESC_DATA_REG_LENGTH+1];
    uint8_t i2c_reg = I2C2_ESC1_DATA_GET_REG + (esc_number -1);
    uint8_t checksum=0;

    if(i2c_driver.I2cGetData(i2c_slave_address,i2c_reg,&received_data[0],(I2C2_ESC_DATA_REG_LENGTH+1)) != 0) 
    {      
        status = 1;
    }else
    {
        status = 0;
    }

    if(status==0)
    {
        checksum=i2c_driver.I2cCalculateChecksum(&received_data[0],I2C2_ESC_DATA_REG_LENGTH);
        if(checksum == received_data[I2C2_ESC_DATA_REG_LENGTH])
        {
            memcpy((uint8_t*)struct_pointer, &received_data[0], I2C2_ESC_DATA_REG_LENGTH); 
        }
        else status=2;
    }
    return status;
}

int Pb6s40aDroneControl::EscGetErrorLogs(ERROR_WARN_LOG* struct_pointer, uint8_t esc_number)
{
    uint8_t status=0;
    uint8_t received_data[I2C2_ESC_ERROR_REG_LENGTH+1];
    uint8_t i2c_reg = I2C2_ESC1_ERROR_GET_REG + (esc_number -1);
    uint8_t checksum=0;

    if(i2c_driver.I2cGetData(i2c_slave_address,i2c_reg,&received_data[0],(I2C2_ESC_ERROR_REG_LENGTH+1)) != 0) 
    {      
        status = 1;
    }else
    {
        status = 0;
    }

    if(status==0)
    {
        checksum=i2c_driver.I2cCalculateChecksum(&received_data[0],I2C2_ESC_ERROR_REG_LENGTH);
        if(checksum == received_data[I2C2_ESC_ERROR_REG_LENGTH])
        {
            memcpy((uint8_t*)struct_pointer, &received_data[0], I2C2_ESC_ERROR_REG_LENGTH); 
        }
        else status=2;
    }
    return status;   
}

Pb6s40aDroneControl::~Pb6s40aDroneControl(){
}



/*LEDS CONTROL*/

int Pb6s40aLedsControl::LedsSendColorBuffer(uint8_t led_channel, COLOR buffer[], uint8_t led_count)
{
    uint8_t reg = I2C2_LEDS_COLOR_SET_REG;
    uint8_t msg_length = I2C2_LEDS_COLOR_SET_LENGTH;
    if(led_channel<4 && led_count>10)return 1;
    if(led_channel==ad_buffer && led_count>10)
    {
        reg = I2C2_LEDS_COLOR_SET_LONG_REG;
        msg_length = I2C2_LEDS_COLOR_SET_LONG_LENGTH;
    }
    uint8_t transmit_data[msg_length+2]; //buff+ data + checksum  
    memset(&transmit_data[0], 0x00, msg_length+2);
    transmit_data[0]= reg;
    transmit_data[1]= led_channel;
    memcpy(&transmit_data[2], (uint8_t*)&buffer[0], (led_count*3)); 
    transmit_data[msg_length+1]= i2c_driver.I2cCalculateChecksum(&transmit_data[0],msg_length+1);

    if(i2c_driver.I2cSetData(i2c_slave_address,reg,&transmit_data[1],msg_length+1) != 0) 
    {     
        return 1;
    }else
    {
        return 0;
    }
}

int Pb6s40aLedsControl::LedsSetLedsCount(LEDS_COUNT &leds_count)
{
    if(leds_count.fl_leds_count>10 || leds_count.fr_leds_count>10 || leds_count.rl_leds_count>10 ||leds_count.rr_leds_count>10 || leds_count.ad_leds_count>40)return 1;
    uint8_t transmit_data[I2C2_LEDS_COUNT_REG_LENGTH+1]; //buff+data + checksum  
    memset(&transmit_data[0], 0x00, I2C2_LEDS_COUNT_REG_LENGTH+1);
    transmit_data[0]= I2C2_LEDS_COUNT_SET_REG;            
    memcpy(&transmit_data[1], (uint8_t*)&leds_count, I2C2_LEDS_COUNT_REG_LENGTH); 
    transmit_data[I2C2_LEDS_COUNT_REG_LENGTH+1]= i2c_driver.I2cCalculateChecksum(&transmit_data[0],I2C2_LEDS_COUNT_REG_LENGTH+1);

    if(i2c_driver.I2cSetData(i2c_slave_address,I2C2_LEDS_COUNT_SET_REG,&transmit_data[1],I2C2_LEDS_COUNT_REG_LENGTH+1) != 0) 
    {     
        return 1;
    }else
    {
        return 0;
    }
}

int Pb6s40aLedsControl::LedsGetLedsCount(LEDS_COUNT &leds_count)
{
    uint8_t status=0; 
    uint8_t received_data[I2C2_LEDS_COUNT_REG_LENGTH+1];    
    uint8_t checksum=0;   
    if(i2c_driver.I2cGetData(i2c_slave_address,I2C2_LEDS_COUNT_GET_REG,&received_data[0],(I2C2_LEDS_COUNT_REG_LENGTH+1)) != 0) 
    {      
        status = 1;
    }else
    {
        status = 0;
    }

    if(status==0)
    {
        checksum=i2c_driver.I2cCalculateChecksum(&received_data[0],I2C2_LEDS_COUNT_REG_LENGTH);
        if(checksum == received_data[I2C2_LEDS_COUNT_REG_LENGTH])
        {
            leds_count.fl_leds_count=received_data[0];
            leds_count.fr_leds_count=received_data[1];
            leds_count.rl_leds_count=received_data[2];
            leds_count.rr_leds_count=received_data[3];
            leds_count.ad_leds_count=received_data[4];
        }
        else status=2;
    }
    return status;
}

int Pb6s40aLedsControl::LedsSetBufferWithOneColor(COLOR buffer[], COLOR color_to_set, uint8_t led_count)
{
    for (int i = 0; i < led_count; i++)
    {
        buffer[i].R=color_to_set.R;
        buffer[i].G=color_to_set.G;
        buffer[i].B=color_to_set.B;
    }
    return 0;
}

int Pb6s40aLedsControl::LedsUpdate()
        {
            uint8_t transmit_data[I2C2_LEDS_UPDATE_REG_LENGTH+1]; //reg+data + checksum  
            transmit_data[0]= I2C2_LEDS_UPDATE_REG;
            transmit_data[1]= 0x01;
            transmit_data[2]= i2c_driver.I2cCalculateChecksum(&transmit_data[0],I2C2_ESC_STATE_REG_LENGTH+1);

            if(i2c_driver.I2cSetData(i2c_slave_address,I2C2_LEDS_UPDATE_REG,&transmit_data[1],I2C2_LEDS_UPDATE_REG_LENGTH+1) != 0) 
            {     
                return 1;
            }else
            {
                return 0;
            }
        }