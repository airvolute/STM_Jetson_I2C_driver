/**
  ******************************************************************************
  * File Name          : pb6s40a_control.h
  * Description        : Header file of Autopilot power board (PB6S40A) driver
  ******************************************************************************
  * INSTRUCTIONS
  * /LEDS/
  * -sum of all mounted leds must not exceed 50
  * -maximum led count for fl,fr,rl,rr channel is 10, for add channel is maximum 40 leds                            
  * - RGB value is from range 0-128
  * 
  * /ESC CONTROL/
  * - ERROR_WARN_LOG structure contain diagnostic_status variable, which is filled by 0 when there was no error communicating with esc
  ******************************************************************************
  */
#include "i2c_driver.h"
#include <cstring> 
#include <iostream>


/*Power board PB6S40A - I2C2 MESSAGE REGISTERS*/
#define I2C2_POWER_BOARD_STATUS				0x00
#define I2C2_POWER_BOARD_INFO				0x01
#define I2C2_DRONE_ARM_STATE_SET			0x02
#define I2C2_DRONE_ARM_STATE_GET			0x03
#define I2C2_START_OR_ESCAPE_ESC_CONFIG		0x04
#define I2C2_NEXT_STEP_ESC_CONFIG			0x05
#define I2C2_STM_POWER_CASES_GET			0x06
#define I2C2_DRONE_TURN_OFF					0x07
#define I2C2_POWER_BOARD_SETUP				0x08
#define I2C2_POWER_BOARD_SETUP_GET			0x09

#define I2C2_ESC_STATE_GET_REG			0x11
#define I2C2_ESC_STATE_SET_REG			0x12
#define I2C2_ESC1_ERROR_GET_REG			0x13
#define I2C2_ESC2_ERROR_GET_REG			0x14
#define I2C2_ESC3_ERROR_GET_REG			0x15
#define I2C2_ESC4_ERROR_GET_REG			0x16
#define I2C2_ESC1_DATA_GET_REG			0x17
#define I2C2_ESC2_DATA_GET_REG			0x18
#define I2C2_ESC3_DATA_GET_REG			0x19
#define I2C2_ESC4_DATA_GET_REG			0x1A
#define I2C2_ESC1_INFO_GET_REG			0x1B
#define I2C2_ESC2_INFO_GET_REG			0x1C
#define I2C2_ESC3_INFO_GET_REG			0x1D
#define I2C2_ESC4_INFO_GET_REG			0x1E
#define I2C2_ESC1_RESIST_GET_REG		0x1F
#define I2C2_ESC2_RESIST_GET_REG		0x20
#define I2C2_ESC3_RESIST_GET_REG		0x21
#define I2C2_ESC4_RESIST_GET_REG		0x22
#define I2C2_ESC_RUNTIME_STATUS_GET     0x23

#define I2C2_LEDS_UPDATE_REG			0x60
#define I2C2_LEDS_COUNT_GET_REG			0x61
#define I2C2_LEDS_COUNT_SET_REG			0x62
#define I2C2_LEDS_COLOR_SET_REG			0x63
#define I2C2_LEDS_COLOR_SET_LONG_REG	0x64
#define I2C2_LEDS_SWITCH_PREDEF_EFFECT_REG	0x65
#define I2C2_LEDS_SET_PREDEF_EFFECT_REG		0x66

#define I2C2_POWER_BOARD_INFO_LEN		12
#define I2C2_POWER_BOARD_SETUP_LEN		8
#define I2C2_DRONE_ARM_STATE_LEN		1
#define I2C2_ESC_STATE_REG_LENGTH		1
#define I2C2_ESC_ERROR_REG_LENGTH		25   //size of ERROR_WARN_LOG
#define I2C2_ESC_DATA_REG_LENGTH		9	 //size of RUN_DATA_Struct 8 + status
#define I2C2_ESC_INFO_GET_REG_LENGTH	13   //size of ADB_DEVICE_INFO struct 12+status
#define I2C2_ESC_RESIST_GET_REG_LENGTH  17
#define I2C2_ESC_RUNTIME_STATUS_GET_LEN 4
#define I2C2_LEDS_UPDATE_REG_LENGTH		1
#define I2C2_LEDS_COUNT_REG_LENGTH		5
#define I2C2_LEDS_COLOR_SET_LENGTH		31
#define I2C2_LEDS_COLOR_SET_LONG_LENGTH	121
#define I2C2_LEDS_SWITCH_PREDEF_EFFECT_LEN  1
#define I2C2_LEDS_SET_PREDEF_EFFECT_LEN		16 
#define I2C2_STM_POWER_CASES_GET_LEN		8
#define I2C2_POWER_BOARD_STATUS_LEN			1

#define ERROR_WARN_INIT {0x0000, 0x0000}
#define ERROR_WARN_LOG_INIT {ERROR_WARN_INIT, ERROR_WARN_INIT, ERROR_WARN_INIT}

#define I2C2_MAIN_BOARD_ADDRESS 0x10
#define I2C2_SECONDARY_BOARD_ADDRESS 0x11

enum esc_states{escs_sleep,	escs_run, escs_diagnostic};
enum esc_numbers{esc1=1,esc2,esc3,esc4};

typedef struct _ERROR_WARN_
{
    uint32_t Error;
    uint32_t Warn;
} ERROR_WARN;

typedef struct _ERROR_WARN_LOG_
{
    ERROR_WARN Last; // From the current start-up
    ERROR_WARN Prev; // From the previous start-up
    ERROR_WARN All;  // Summary from old start-ups (Excluded previous)
    uint8_t Diagnostic_status;  //0=NO_ERROR
} ERROR_WARN_LOG;

typedef struct _RUN_DATA_Struct_
{
    uint32_t Is_Motor_Max  :12;//in IQ8
    uint32_t Is_Motor_Avg  :12;//in 0.1A
    uint32_t Temp_ESC_Max  :8; //offset -50

    uint32_t Temp_Motor_Max:8; //offset -50
    uint32_t Reserved      :24;
    uint8_t  Diagnostic_status;
} RUN_DATA_Struct;

typedef struct
{
	uint8_t major;
	uint8_t mid;
	uint8_t minor;

} FW_NUMBER;

typedef struct
{
	FW_NUMBER fw_number;    
	uint32_t serial_number;
	uint32_t hw_build;
	uint8_t device_address;
    uint8_t  Diagnostic_status;
} ADB_DEVICE_INFO;

typedef struct
{
    float   Phase[3];
    float   Global;
    uint8_t	Diagnostic_status;
 } RESISTANCE_STRUCT;

typedef struct
{
	FW_NUMBER fw_number;
    uint8_t dummy;
	uint32_t serial_number;
	uint32_t hw_build;
} POWER_BOARD_INFO;

typedef struct
{
	uint8_t autostart;
    uint8_t jetson_rtc_maintain;
    uint8_t esc_autorun;
    uint8_t power_sensor_l;
    uint8_t power_sensor_h;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
} POWER_BOARD_SETUP;

typedef struct
{   
    uint8_t LPWR_resets_count;     
    uint8_t WWDG_resets_count;
	uint8_t IWDG_resets_count;
	uint8_t SFT_resets_count;
	uint8_t POR_resets_count;
	uint8_t PIN_resets_count;
	uint8_t OB_resets_count;
	uint8_t	V18PWR_resets_count;

} STM_RESET_CAUSES;

typedef struct
{
	uint8_t esc1_status;
	uint8_t esc2_status;
	uint8_t esc3_status;
    uint8_t esc4_status;

} ESCS_RUNTIME_STATUS;

enum power_board_statuses
{
	program_state_run,
	program_state_Esc_config,
	program_state_turning_off,
	program_state_off,
	program_state_standby
};

enum powerboard_setup
{
	SETUP_AUTOSTART,
	SETUP_JETSON_RTC_MAINTAIN,
	SETUP_ESC_AUTORUN,
	SETUP_POWER_SENSOR_L_FITTED,
	SETUP_POWER_SENSOR_H_FITTED
	//ADD NEW CONFIG (up to 8)
};

class Pb6s40aDroneControl
{
    private: 
        I2CDriver& i2c_driver;
    public:    
        uint8_t i2c_slave_address;

        Pb6s40aDroneControl(I2CDriver& i2c_driver_, uint8_t i2c_address);

        int PowerBoardInfoGet(POWER_BOARD_INFO* pow_board_info_struct);

        int PowerBoardStatusGet(uint8_t* pb_status);

        int PowerBoardSetupRead(POWER_BOARD_SETUP* power_board_setup_struct);

        int PowerBoardSetupWrite(POWER_BOARD_SETUP power_board_setup_struct);

        int DroneTurnOff();

        int DroneArmSet(uint8_t arm_state);

        int DroneArmGet(uint8_t* arm_state);

        int GetStmResetCauses(STM_RESET_CAUSES* struct_pointer);

        int EscNextStepInConfigMode();

        int EscStartOrEscapeConfigMode();

        int EscSetState(uint8_t pesc_state);
        
        int EscGetState(uint8_t* pesc_state);

        int EscGetErrorLogs(ERROR_WARN_LOG* struct_pointer, uint8_t esc_number);

        int EscGetDataLogs(RUN_DATA_Struct* struct_pointer, uint8_t esc_number);

        int EscGetDeviceInfo(ADB_DEVICE_INFO* struct_pointer, uint8_t esc_number);

        int EscGetResistance(RESISTANCE_STRUCT* struct_pointer, uint8_t esc_number);

        int EscGetRuntimeStatus(ESCS_RUNTIME_STATUS* struct_pointer);

        ~Pb6s40aDroneControl();
};    



enum led_buffers
{
	fl_buffer,
	fr_buffer,
	rl_buffer,
	rr_buffer,
	ad_buffer
};

enum drone_arm_statesd
{
	drone_disarm,
	drone_arm
};

typedef struct _LEDS_COUNT
{
  uint8_t fl_leds_count=10;
  uint8_t fr_leds_count=10;
  uint8_t rl_leds_count=10;
  uint8_t rr_leds_count=10;
  uint8_t ad_leds_count=10;
} LEDS_COUNT;

typedef struct _COLOR
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
} COLOR;

#define OFFCOLOR        {0, 0, 0}
#define RED		        {255, 0, 0}
#define GREEN		    {0, 255, 0}
#define BLUE		    {0, 0, 255}
#define YELLOW 		    {255, 255, 0}
#define CYAN		    {0, 255, 255}
#define MAGENTA		    {255, 0, 255}
#define WHITE           {255, 255, 255}

class Pb6s40aLedsControl
{
    private: 
        I2CDriver& i2c_driver;
    public:  
        uint8_t i2c_slave_address;  

        Pb6s40aLedsControl(I2CDriver& i2c_driver_, uint8_t i2c_address);

        int LedsSendColorBuffer(uint8_t led_channel, COLOR buffer[], uint8_t led_count);

        int LedsSetLedsCount(LEDS_COUNT &leds_count);

        int LedsGetLedsCount(LEDS_COUNT &leds_count);

        int LedsUpdate();

        int LedsSetBufferWithOneColor(COLOR buffer[], COLOR color_to_set, uint8_t led_count);

        int LedsSwitchPredefinedEffect(bool on_state);

        int LedsSetPredefinedEffect(COLOR fl_color, COLOR fr_color, COLOR rl_color, COLOR rr_color ,uint8_t on_time, uint8_t off_time, uint8_t effect_type, bool set_as_default);

        ~Pb6s40aLedsControl(){}
};    
