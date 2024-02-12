#include <LoRaWan-Arduino.h>
#include <SPI.h>

#define JOINREQ_NBTRIALS 3

bool has_joined = false;

// ESP32 - SX126x pin configuration
int PIN_LORA_RESET = 12; // LORA RESET
int PIN_LORA_NSS = 8;	 // LORA SPI CS
int PIN_LORA_SCLK = 9;	 // LORA SPI CLK
int PIN_LORA_MISO = 11;	 // LORA SPI MISO
int PIN_LORA_MOSI = 10;	 // LORA SPI MOSI
int PIN_LORA_BUSY = 13;	 // LORA SPI BUSY
int PIN_LORA_DIO_1 = 14; // LORA DIO_1
int RADIO_TXEN = -1;	 // LORA ANTENNA TX ENABLE
int RADIO_RXEN = -1;	 // LORA ANTENNA RX ENABLE

hw_config hwConfig;

// Define Helium or TTN OTAA keys. All msb (big endian).
uint8_t nodeDeviceEUI[8] = {0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02};
uint8_t nodeAppEUI[8] = {0x1B, 0x03, 0x17, 0x0A, 0x16, 0x0A, 0x00, 0x01};
uint8_t nodeAppKey[16] = {0x05, 0xAD, 0xCA, 0x41, 0x9D, 0xC5, 0xCB, 0x1A, 0x2C, 0xE4, 0x22, 0xBE, 0xCA, 0x51, 0xA7, 0x45};


uint8_t m_lora_app_data_buffer[256]; // Lora user application data buffer.
lmh_app_data_t m_lora_app_data = {m_lora_app_data_buffer, 0, 0, 0, 0};	 // Lora user application data structure.

uint32_t i2 = 0;
void send_lora_frame(uint8_t id, uint8_t temp1, uint8_t temp2, uint8_t temp3, uint8_t hum1, uint8_t hum2, uint8_t hum3, uint8_t battery)
{
    uint16_t i = 0;

    m_lora_app_data.port = LORAWAN_APP_PORT;
    m_lora_app_data.buffer[i++] = id; // ID
    m_lora_app_data.buffer[i++] = temp1; // temp 1
    m_lora_app_data.buffer[i++] = temp2; // temp 2
    m_lora_app_data.buffer[i++] = temp3; // temp 3
    m_lora_app_data.buffer[i++] = hum1; // humidity 1
    m_lora_app_data.buffer[i++] = hum2; // humidity 2 
    m_lora_app_data.buffer[i++] = hum3; // humidity 3
    m_lora_app_data.buffer[i++] = battery; // battery
	m_lora_app_data.buffsize = i;


	lmh_error_status error = lmh_send(&m_lora_app_data, LMH_UNCONFIRMED_MSG);
	if (error == LMH_SUCCESS)
	{
	}

	Serial.printf("lmh_send result %d\n", error);
}

void lorawan_join_fail_handler(void)
{
	Serial.println("OTAA joined failed");
}

void lorawan_has_joined_handler(void)
{
    Serial.println("Connected to TTN");
    has_joined = true;
}

void lorawan_confirm_tx_finished(bool result){}

void lorawan_rx_handler(lmh_app_data_t *app_data){}
void lorawan_confirm_class_handler(DeviceClass_t Class){}
void lorawan_unconfirm_tx_finished(void){}
void tx_lora_periodic_handler(void){}

lmh_param_t lora_param_init = {LORAWAN_ADR_OFF, DR_3, LORAWAN_PUBLIC_NETWORK,
							   JOINREQ_NBTRIALS, LORAWAN_DEFAULT_TX_POWER, LORAWAN_DUTYCYCLE_OFF};

lmh_callback_t lora_callbacks = {BoardGetBatteryLevel, BoardGetUniqueId, BoardGetRandomSeed,
								 lorawan_rx_handler, lorawan_has_joined_handler, 
								 lorawan_confirm_class_handler, lorawan_join_fail_handler,
								 lorawan_unconfirm_tx_finished, lorawan_confirm_tx_finished};



// Define the HW configuration between MCU and SX126x
void hw_init()
{
    hwConfig.CHIP_TYPE = SX1262_CHIP;	// Example uses an eByte E22 module with an SX1262
    hwConfig.PIN_LORA_RESET = PIN_LORA_RESET; // LORA RESET
    hwConfig.PIN_LORA_NSS = PIN_LORA_NSS;	  // LORA SPI CS
    hwConfig.PIN_LORA_SCLK = PIN_LORA_SCLK;	  // LORA SPI CLK
    hwConfig.PIN_LORA_MISO = PIN_LORA_MISO;	  // LORA SPI MISO
    hwConfig.PIN_LORA_DIO_1 = PIN_LORA_DIO_1; // LORA DIO_1
    hwConfig.PIN_LORA_BUSY = PIN_LORA_BUSY;	  // LORA SPI BUSY
    hwConfig.PIN_LORA_MOSI = PIN_LORA_MOSI;	  // LORA SPI MOSI
    hwConfig.RADIO_TXEN = RADIO_TXEN;	// LORA ANTENNA TX ENABLE (e.g. eByte E22 module)
    hwConfig.RADIO_RXEN = RADIO_RXEN;	// LORA ANTENNA RX ENABLE (e.g. eByte E22 module)
    hwConfig.USE_DIO2_ANT_SWITCH = false;	// LORA DIO2 does not control antenna
    hwConfig.USE_DIO3_TCXO = true;	// LORA DIO3 controls oscillator voltage (e.g. eByte E22 module)
    hwConfig.USE_DIO3_ANT_SWITCH = false;	// LORA DIO3 does not control antenna


	// Setup the EUIs and Keys
	lmh_setDevEui(nodeDeviceEUI);
	lmh_setAppEui(nodeAppEUI);
	lmh_setAppKey(nodeAppKey);

    uint32_t err_code = lora_hardware_init(hwConfig);
	if (err_code != 0)
	{
		Serial.printf("lora_hardware_init failed - %d\n", err_code);
	}
    
    err_code = lmh_init(&lora_callbacks,        // callback functions in LoRaWAN/LoRaWAN.h
                        lora_param_init,        // init parameters in LoRaWAN/LoRaWAN.h
                        true,                   // OTAA
                        CLASS_A,                // Class
                        LORAMAC_REGION_EU868,   // Region of communication
                        false);                 // Changement of region
	
    if (err_code != 0)
	{
		Serial.printf("lmh_init failed - %d\n", err_code);
	}

	lmh_setSubBandChannels(2);
}
