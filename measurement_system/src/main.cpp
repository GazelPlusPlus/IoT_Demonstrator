#include <Arduino.h>
#include <HW_SPEC/HW_SPEC.h>
#include <SHT31/sht31.h>
#include <LoRaWAN/LoRaWAN.h>
#include <DeepSleep/DeepSleep.h>
#include <BLE/BLE.h>


enum heltec_state
{
    STATE_WAKE_UP,
    STATE_INIT_LORA,
    STATE_MEASURE,
    STATE_CALIBRATION,
    STATE_JOINING_TTN_SERVER,
    STATE_SENDING_PACKET,
    STATE_INIT_BLE,
    STATE_BLE_IS_ACTIVE,
    STATE_SLEEP_TIME_CALCULATION,
    STATE_DEEP_SLEEP
};
int state = STATE_WAKE_UP;

int counter = 0;

bool id_0 = false;
bool id_1 = false;
bool id_2 = false;
bool id_3 = false;
bool id_4 = false;



void setup() 
{
    Serial.begin(115200);
}

void loop() 
{
    switch(state)
    {
        case STATE_WAKE_UP:
            esp_sleep_wakeup_cause_t wakeup_reason;
            wakeup_reason = esp_sleep_get_wakeup_cause();
            if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
            {
                Serial.println("\nWake up by timer !");
                state = STATE_INIT_LORA;
            }
            else if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
            {
                Serial.println("\nWake up by button !");
                state = STATE_INIT_BLE;
            }
            else
            {
                Serial.println("\nWake up undefined !");
                state = STATE_DEEP_SLEEP;
            }
            pinMode(LED, OUTPUT);
            digitalWrite(LED, HIGH);
            break;
        
        case STATE_INIT_LORA:
            hw_init();
            state = STATE_MEASURE;
            break;
        
        case STATE_MEASURE:
            make_measure();
            state = STATE_CALIBRATION;
            break;
        
        case STATE_CALIBRATION:
            // TODO 
            state = STATE_JOINING_TTN_SERVER;
            break;

        case STATE_JOINING_TTN_SERVER:
            lmh_join();
            state = STATE_SENDING_PACKET;
            break;
        
        case STATE_SENDING_PACKET:
            counter++;
            delay(1000);
            if(has_joined)
            {
                counter = 0;
                send_lora_frame(id, temp1, temp2, temp3, hum1, hum2, hum3, battery);  
                state = STATE_SLEEP_TIME_CALCULATION;
            }
            if(counter > 20) // 20 seconds to find the TTN network
            {
                Serial.println("TTN network not found, going to sleep !");
                state = STATE_DEEP_SLEEP;
            }
            // TODO au cas ou il n'arrive pas à join au server TTN, se rendort
            break;
        
        case STATE_INIT_BLE:
            make_measure();
            init_ble();
            delay(15000);
            state = STATE_BLE_IS_ACTIVE;
            if(!smartphone_connected)
            {
                state = STATE_SLEEP_TIME_CALCULATION;
            }
            break;

        case STATE_BLE_IS_ACTIVE:
            if(!smartphone_connected)
            {
                state = STATE_INIT_BLE;
            }
            break;
        
        case STATE_SLEEP_TIME_CALCULATION:
            // TODO 
            state = STATE_DEEP_SLEEP;
            break;

        case STATE_DEEP_SLEEP:
            esp_sleep_enable_timer_wakeup(uint64_t(time_to_sleep));
            esp_sleep_enable_ext0_wakeup(BUTTON, HIGH);
            Serial.println("Enter in deep sleep mode");
            esp_deep_sleep_start();
            break;
    }
}   

// void loop()
// {
//     init_ble();
// }
