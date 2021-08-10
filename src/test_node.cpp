#include "i2c_driver.h" 
#include "pb6s40a_control.h"
#include <iostream>
#include <unistd.h> 

const char * device = "/dev/i2c-1"; // NANO
//const char * device = "/dev/i2c-8";  //NX
I2CDriver i2c1_driver;
Pb6s40aDroneControl drone_control(i2c1_driver);
Pb6s40aLedsControl leds_control(i2c1_driver);


ERROR_WARN_LOG esc_error_logs[4] ={ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT,ERROR_WARN_LOG_INIT};
RUN_DATA_Struct esc_data_logs[4];
ADB_DEVICE_INFO esc_device_infos[4];
RESISTANCE_STRUCT esc_resistance_structs[4];



LEDS_COUNT mounted_leds_count;
COLOR leds_fl_color_buff[10];
COLOR leds_fr_color_buff[10];
COLOR leds_rl_color_buff[10];
COLOR leds_rr_color_buff[10];
COLOR leds_ad_color_buff[40];

uint8_t drone_arm_state = drone_disarm;

/*Power board firmware number*/
POWER_BOARD_INFO power_board_info;


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

      /*Power board firmware read*/
      status = drone_control.PowerBoardInfoGet(&power_board_info);
      std::cout<<"status:"<<(uint32_t)status<<"  POWER BOARD firmware (maj.mid.min): "<<(uint32_t)power_board_info.fw_number.major<<"."<<(int)power_board_info.fw_number.mid<<"."<<(int)power_board_info.fw_number.minor
      <<"  Serial number:"<<(uint32_t)power_board_info.serial_number<<"  HW build:"<<(uint32_t)power_board_info.hw_build<<std::endl;
      
      std::cout<<"TESTING FIRMWARE ? : "<<((uint32_t)(power_board_info.hw_build & 0x0001))<<std::endl;

      /*DRONE ARM STATE*/     
      //SET arm/disarm STATE
      /*status = drone_control.DroneArmSet(drone_arm);
      //GET STATE
      status = drone_control.DroneArmGet(&drone_arm_state);
      std::cout<<"status:"<<(int)status<<" drone arm state:"<<(int)drone_arm_state<<std::endl;*/

      /****ESC FUNCTION EXAMPLES ****/
     /*Functions to read/write state of ESC  - not used noww*/
      /*uint8_t esc_state;
      status= drone_control.EscSetState(escs_run);  //SET escs to sleep / run 
      status= drone_control.EscGetState(&esc_state);*/



      /*Functions to read ESCs error logs, data logs and info*/
      /*status= drone_control.EscGetErrorLogs(&esc_error_logs[0],esc1);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[1],esc2);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[2],esc3);
      status= drone_control.EscGetErrorLogs(&esc_error_logs[3],esc4);

      std::cout<<"Diagnostic status for error logs: "<<(int)esc_error_logs[0].Diagnostic_status<<(int)esc_error_logs[1].Diagnostic_status<<(int)esc_error_logs[2].Diagnostic_status<<(int)esc_error_logs[3].Diagnostic_status<<std::endl;
      
      status= drone_control.EscGetDataLogs(&esc_data_logs[0],esc1);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[1],esc2);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[2],esc3);      
      status= drone_control.EscGetDataLogs(&esc_data_logs[3],esc4);  
      for (int i = 0; i < 4; i++)
      {
            std::cout<<"Diagnostic status="<<(int)esc_data_logs[i].Diagnostic_status<<"  ESC"<<(int)i<<" IS motor max: "<<(int)esc_data_logs[i].Is_Motor_Max<<" IS motor avg: "<<(int)esc_data_logs[i].Is_Motor_Avg
            <<" Temp esc max: "<<(int)esc_data_logs[i].Temp_ESC_Max<<" Temp motor max: "<<(int)esc_data_logs[i].Temp_Motor_Max<<std::endl;
      }

      status= drone_control.EscGetDeviceInfo(&esc_device_infos[0],esc1); 
      status= drone_control.EscGetDeviceInfo(&esc_device_infos[1],esc2);
      status= drone_control.EscGetDeviceInfo(&esc_device_infos[2],esc3);
      status= drone_control.EscGetDeviceInfo(&esc_device_infos[3],esc4);
      for (int i = 0; i < 4; i++)
      {
         std::cout<<"Diagnostic status="<<(int)esc_device_infos[i].Diagnostic_status<<"  ESC"<<i<<" FIRMWARE (maj.mid.min): "<<(int)esc_device_infos[i].fw_number.major<<"."<<(int)esc_device_infos[i].fw_number.mid<<"."<<(int)esc_device_infos[i].fw_number.minor<<"   device addr:"<<(int)esc_device_infos[i].device_address<<
         "   hw build:"<<(uint32_t)esc_device_infos[i].hw_build<<"   serial num:"<<(uint32_t)esc_device_infos[i].serial_number<<std::endl;
      }*/
      

      /*ESC CONFIG MODE - only for esc config script*/
      //status = drone_control.EscStartOrEscapeConfigMode();
      //std::cout<<"EscStartOrEscapeConfigMode:  status:"<<(int)status<<std::endl;

      //NEXT STEP
      //status = drone_control.EscNextStepInConfigMode();
      //std::cout<<"EscNextStepInConfigMode:  status:"<<(int)status<<std::endl;

      //ESC READ RESISTANCES
      /*status= drone_control.EscGetResistance(&esc_resistance_structs[0],esc1); 
      status= drone_control.EscGetResistance(&esc_resistance_structs[1],esc2);
      status= drone_control.EscGetResistance(&esc_resistance_structs[2],esc3);
      status= drone_control.EscGetResistance(&esc_resistance_structs[3],esc4);
      for (int i = 0; i < 4; i++)
      {
         std::cout<<"Diagnostic status="<<(int)esc_resistance_structs[i].Diagnostic_status<<"  ESC"<<i<<": PhaseA: "<<esc_resistance_structs[i].Phase[0]<<" PhaseB: "
         <<esc_resistance_structs[i].Phase[1]<<" PhaseC: "<<esc_resistance_structs[i].Phase[2]<< " GLOBAL: "<<esc_resistance_structs[i].Global<<std::endl;
      }*/



      /****LEDS FUNCTION EXAMPLES ****/
      //Functions to GET/SET mounted leds count 
      //LEDS_COUNT ledc;
      //status=leds_control.LedsGetLedsCount(ledc);
      mounted_leds_count.fl_leds_count=8;
      mounted_leds_count.fr_leds_count=8;
      mounted_leds_count.rl_leds_count=8;
      mounted_leds_count.rr_leds_count=8;
      mounted_leds_count.ad_leds_count=8;
      status=leds_control.LedsSetLedsCount(mounted_leds_count);

      std::cout<<"LEDS count set status: "<<(int)status<<std::endl;
      

      status= leds_control.LedsSwitchPredefinedEffect(false);
      std::cout<<"LEDS turn off status: "<<(int)status<<std::endl;

      std::cout<<"led turned off .. press key to continue"<<std::endl;
      std::getc(stdin);
      std::cout<<" ... ..."<<std::endl;

      status= leds_control.LedsSetPredefinedEffect(RED,RED,RED,RED,40,40,1,false);
      //status= leds_control.LedsSetPredefinedEffect(GREEN,GREEN,GREEN,GREEN,40,40,1,false); 
      std::cout<<"LEDS set effect status: "<<(int)status<<std::endl;    
      
      status= leds_control.LedsSwitchPredefinedEffect(true);
      std::cout<<"LEDS turn on status: "<<(int)status<<std::endl;

      /*std::cout<<"press key"<<std::endl;
      std::getc(stdin);
      std::cout<<" ... ..."<<std::endl;*/      
      COLOR pom_color = RED;
      COLOR off_color = OFFCOLOR;

      while(0)
      {       
            /*CONTROL ADDITIONAL LED CHANNEL IN WHILE CYCLE (GREEN/BLUE TOGGLING) */

            pom_color=GREEN; 

            usleep(500000);
            leds_control.LedsSetBufferWithOneColor(leds_ad_color_buff,pom_color,8); // SET WHOLE BUFFER WITH ONE COLOR 
            status=leds_control.LedsSendColorBuffer(ad_buffer,leds_ad_color_buff,mounted_leds_count.ad_leds_count);
            status=leds_control.LedsUpdate(); // TRIGGERS UPDATE OF LEDS
            usleep(500000);
            pom_color.R=0;
            pom_color.G=0;
            pom_color.B=255;
            leds_control.LedsSetBufferWithOneColor(leds_ad_color_buff,pom_color,8);
            status=leds_control.LedsSendColorBuffer(ad_buffer,leds_ad_color_buff,mounted_leds_count.ad_leds_count);
            status=leds_control.LedsUpdate();
             
      }
      

      i2c1_driver.I2cClose();

       return 0;
}
