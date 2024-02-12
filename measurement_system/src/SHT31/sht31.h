#pragma once

#include <Adafruit_SHT31.h>
#include <SPI.h>


uint8_t id = 0x00;
uint8_t temp1 = 0x00;
uint8_t temp2 = 0x00;
uint8_t temp3 = 0x00;
uint8_t hum1 = 0x00;
uint8_t hum2 = 0x00;
uint8_t hum3 = 0x00;

class SHT31 : public Adafruit_SHT31
{
private:
    uint8_t nb_port_mux_num;

public:
    SHT31(uint8_t nb)
    {
        nb_port_mux_num = 0;
        if(nb_port_mux_num >= 0 && nb_port_mux_num < 8)
        {
            nb_port_mux_num = nb;
        }
    };

    uint8_t get_temperature(void)
    {
        Wire.beginTransmission(0x70);
        Wire.write(1 << nb_port_mux_num);
        Wire.endTransmission();        
        // float temp = -10; //this->readTemperature();
        return uint8_t((int)(this->readTemperature())%256);
    }

    uint8_t get_humidity(void)
    {
        Wire.beginTransmission(0x70);
        Wire.write(1 << nb_port_mux_num);
        Wire.endTransmission();
        return uint8_t(this->readHumidity());
    }
};

SHT31 sht31_1(7);
SHT31 sht31_2(5);
SHT31 sht31_3(3);

void make_measure()
{
    pinMode(SENSORS_SUPPLY, OUTPUT);
    digitalWrite(SENSORS_SUPPLY, HIGH);
    delay(1);
    Wire.begin();

    if(!sht31_1.begin(0x44))
    {
        Serial.println("SHT number 1 not found");
    }
    if(!sht31_2.begin(0x44))
    {
        Serial.println("SHT number 2 not found");
    }
    if(!sht31_3.begin(0x44))
    {
        Serial.println("SHT number 3 not found");
    }
    id = get_id();
    temp1 = sht31_1.get_temperature();
    temp2 = sht31_2.get_temperature();
    temp3 = sht31_3.get_temperature();
    hum1 = sht31_1.get_humidity();
    hum2 = sht31_2.get_humidity();
    hum3 = sht31_3.get_humidity();

    Serial.println();
    Serial.println("id: " + String(id));
    Serial.println("temp1: " + String(temp1));
    Serial.println("temp2: " + String(temp2));
    Serial.println("temp3: " + String(temp3));
    Serial.println("hum1: " + String(hum1));
    Serial.println("hum2: " + String(hum2));
    Serial.println("hum3: " + String(hum3));
    Serial.println("battery: " + String(battery));
    Serial.println();

    battery = (uint8_t)90;
    digitalWrite(SENSORS_SUPPLY, LOW);
}