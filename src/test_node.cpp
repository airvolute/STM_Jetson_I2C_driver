#include "i2c_driver.h" 
#include "pb6s40a_control.h"
#include <iostream>
#include <unistd.h> 

const char * device = "/dev/i2c-1";
I2CDriver i2c1_driver;
Pb6s40aDroneControl drone_control(i2c1_driver);
Pb6s40aLedsControl leds_control(i2c1_driver);


ERROR_WARN_LOG esc_error_logs[4] ={ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT};
RUN_DATA_Struct esc_data_logs[4];

LEDS_COUNT mounted_leds_count;
COLOR leds_fl_color_buff[10];
COLOR leds_fr_color_buff[10];
COLOR leds_rl_color_buff[10];
COLOR leds_rr_color_buff[10];
COLOR leds_ad_color_buff[40];

int main(int argc, char **argv)
{
      if(i2c1_driver.I2cOpen(device) != 0 )
      {
            //Handle error
            std::cout<<"I2C error happens when opening port: "<<std::endl;
      };

      std::cout<<"press key"<<std::endl;
      std::getc(stdin);
      std::cout<<" ... ..."<<std::endl;

      uint8_t status=0; // 0=No Error
      /****ESC FUNCTION EXAMPLES ****/

     /*Functions to read/write state of ESC*/
      /*uint8_t esc_state;
      status= drone_control.EscSetState(escs_run);
      status= drone_control.EscGetState(&esc_state);*/

      //Functions to read ESCs error logs
      /*status= drone_control.EscGetErrorLogs(&esc_error_logs[0],esc1);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[1],esc2);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[2],esc3);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[3],esc4);*/
      
      /*status= drone_control.EscGetDataLogs(&esc_data_logs[0],esc1);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[1],esc2);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[2],esc3);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[3],esc4);*/   
      
      /*std::cout<<(int)esc_data_logs[0].Temp_ESC_Max<<std::endl;
      std::cout<<(int)esc_data_logs[1].Temp_ESC_Max<<std::endl;
      std::cout<<(int)esc_data_logs[2].Temp_ESC_Max<<std::endl;
      std::cout<<(int)esc_data_logs[3].Temp_ESC_Max<<std::endl;*/     


      /****LEDS FUNCTION EXAMPLES ****/
      //Functions to GET/SET mounted leds count 
      //LEDS_COUNT ledc;
      //status=leds_control.LedsGetLedsCount(ledc);
      mounted_leds_count.fl_leds_count=10;
      mounted_leds_count.fr_leds_count=10;
      mounted_leds_count.rl_leds_count=10;
      mounted_leds_count.rr_leds_count=10;
      mounted_leds_count.ad_leds_count=9;
      status=leds_control.LedsSetLedsCount(mounted_leds_count);

      

      /*std::cout<<"press key"<<std::endl;
      std::getc(stdin);
      std::cout<<" ... ..."<<std::endl;*/      
      COLOR pom_color = RED;
      while(1)
      {
            usleep(100000);
            pom_color.R=128;
            pom_color.G=0;
            pom_color.B=0;
            //Setting all buffer for additional led channel with red collor and send it to device  
            leds_control.LedsSetBufferWithOneColor(leds_ad_color_buff,pom_color,40);
            status=leds_control.LedsSendColorBuffer(fl_buffer,leds_fl_color_buff,10);
            status=leds_control.LedsSendColorBuffer(fr_buffer,leds_fr_color_buff,10);
            status=leds_control.LedsSendColorBuffer(rl_buffer,leds_rl_color_buff,10);
            status=leds_control.LedsSendColorBuffer(rr_buffer,leds_rr_color_buff,10);
            status=leds_control.LedsSendColorBuffer(ad_buffer,leds_ad_color_buff,9);
            status=leds_control.LedsUpdate();
            usleep(100000);
            pom_color.R=0;
            pom_color.G=0;
            pom_color.B=128;
            leds_control.LedsSetBufferWithOneColor(leds_ad_color_buff,pom_color,40);
            status=leds_control.LedsSendColorBuffer(fl_buffer,leds_fl_color_buff,10);
            status=leds_control.LedsSendColorBuffer(fr_buffer,leds_fr_color_buff,10);
            status=leds_control.LedsSendColorBuffer(rl_buffer,leds_rl_color_buff,10);
            status=leds_control.LedsSendColorBuffer(rr_buffer,leds_rr_color_buff,10);
            status=leds_control.LedsSendColorBuffer(ad_buffer,leds_ad_color_buff,9);
            status=leds_control.LedsUpdate();
      }
      

      i2c1_driver.I2cClose();

       return 0;
}
