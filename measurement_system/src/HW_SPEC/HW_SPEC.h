#include <Arduino.h>

#define ID_0                    19
#define ID_1                    20
#define ID_2                    21
#define ID_3                    26
#define ID_4                    48
#define BUTTON                  GPIO_NUM_7
#define LED                     35
#define BLE_LED                 2
#define SENSORS_SUPPLY          3

uint8_t battery = 0x00;

uint8_t get_id()
{
    pinMode(ID_4, INPUT);
    pinMode(ID_3, INPUT);
    pinMode(ID_2, INPUT);
    pinMode(ID_1, INPUT);
    pinMode(ID_0, INPUT);

    bool id_4, id_3, id_2, id_1, id_0;
    id_4 = digitalRead(ID_4);
    id_3 = digitalRead(ID_3);
    id_2 = digitalRead(ID_2);
    id_1 = digitalRead(ID_1);
    id_0 = digitalRead(ID_0);

    return uint8_t((id_4 << 4) | (id_3 << 3) | (id_2 << 2) | (id_1 << 1) | (id_0 << 0) + 1);
}