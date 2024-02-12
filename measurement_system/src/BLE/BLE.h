#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <Arduino.h>
#include <string.h>



#define DEVICE_ID_SERVICE_UUID                      BLEUUID((uint16_t)0x180A)
#define DEVICE_ID_CHARACTERISTIC_UUID               BLEUUID((uint16_t)0x2A23)
#define DEVICE_ID_DESCRIPTOR_UUID                   BLEUUID((uint16_t)0x2700)

#define ENVIRONMENTAL_SENSING_SERVICE_UUID          BLEUUID((uint16_t)0x181A)
#define TEMPERATURE_CHARACTERISTIC_UUID             BLEUUID((uint16_t)0x2A6E)
#define HUMIDITY_CHARACTERISTIC_UUID                BLEUUID((uint16_t)0x2A6F)
#define TEMPERATURE_DESCRIPTOR_UUID                 BLEUUID((uint16_t)0x272F)
#define HUMIDITY_DESCRIPTOR_UUID                    BLEUUID((uint16_t)0x27AD)

#define BATTERY_SERVICE_UUID                        BLEUUID((uint16_t)0x180F)
#define BATTERY_CHARACTERISTIC_UUID                 BLEUUID((uint16_t)0x2A19)
#define BATTERY_DESCRIPTOR_UUID                     BLEUUID((uint16_t)0x27AD)



bool smartphone_connected = false;

BLEServer *server = NULL;


class CustomCallback : public BLEServerCallbacks
{
    void onConnect(BLEServer *MyServer)//;
    {
        smartphone_connected = true;
        pinMode(BLE_LED, OUTPUT);
        digitalWrite(BLE_LED, HIGH);
    }
    void onDisconnect(BLEServer *MyServer)//;
    {
        smartphone_connected = false;
        digitalWrite(BLE_LED, LOW);
        server->getAdvertising()->stop();
    }
};

void create_service_device()
{
    BLEService* service = server->createService(DEVICE_ID_SERVICE_UUID);
    BLECharacteristic* characteristic = new BLECharacteristic(DEVICE_ID_CHARACTERISTIC_UUID, 
                                                              BLECharacteristic::PROPERTY_READ);
    BLEDescriptor* descriptor = new BLEDescriptor(DEVICE_ID_DESCRIPTOR_UUID);

    characteristic->addDescriptor(descriptor);
    service->addCharacteristic(characteristic);

    characteristic->setValue(&id, 1);
    descriptor->setValue("System ID");

    service->start();
}

void create_service_environmental_sensing()
{
    BLEService* service = server->createService(ENVIRONMENTAL_SENSING_SERVICE_UUID, 32);
    BLECharacteristic* characteristic_temp1 = new BLECharacteristic(TEMPERATURE_CHARACTERISTIC_UUID, 
                                                                    BLECharacteristic::PROPERTY_READ);
    BLECharacteristic* characteristic_temp2 = new BLECharacteristic(TEMPERATURE_CHARACTERISTIC_UUID, 
                                                                    BLECharacteristic::PROPERTY_READ);
    BLECharacteristic* characteristic_temp3 = new BLECharacteristic(TEMPERATURE_CHARACTERISTIC_UUID, 
                                                                    BLECharacteristic::PROPERTY_READ);
    BLECharacteristic* characteristic_hum1 = new BLECharacteristic(HUMIDITY_CHARACTERISTIC_UUID, 
                                                                   BLECharacteristic::PROPERTY_READ);
    BLECharacteristic* characteristic_hum2 = new BLECharacteristic(HUMIDITY_CHARACTERISTIC_UUID, 
                                                                   BLECharacteristic::PROPERTY_READ);
    BLECharacteristic* characteristic_hum3 = new BLECharacteristic(HUMIDITY_CHARACTERISTIC_UUID, 
                                                                    BLECharacteristic::PROPERTY_READ);
    BLEDescriptor* descriptor_temp1 = new BLEDescriptor(TEMPERATURE_DESCRIPTOR_UUID);
    BLEDescriptor* descriptor_temp2 = new BLEDescriptor(TEMPERATURE_DESCRIPTOR_UUID);
    BLEDescriptor* descriptor_temp3 = new BLEDescriptor(TEMPERATURE_DESCRIPTOR_UUID);
    BLEDescriptor* descriptor_hum1 = new BLEDescriptor(BATTERY_DESCRIPTOR_UUID);
    BLEDescriptor* descriptor_hum2 = new BLEDescriptor(BATTERY_DESCRIPTOR_UUID);
    BLEDescriptor* descriptor_hum3 = new BLEDescriptor(BATTERY_DESCRIPTOR_UUID);

    characteristic_temp1->addDescriptor(descriptor_temp1);
    characteristic_temp2->addDescriptor(descriptor_temp2);
    characteristic_temp3->addDescriptor(descriptor_temp3);
    characteristic_hum1->addDescriptor(descriptor_hum1);
    characteristic_hum2->addDescriptor(descriptor_hum2);
    characteristic_hum3->addDescriptor(descriptor_hum3);
    service->addCharacteristic(characteristic_temp1);
    service->addCharacteristic(characteristic_hum1);
    service->addCharacteristic(characteristic_temp2);
    service->addCharacteristic(characteristic_hum2);
    service->addCharacteristic(characteristic_temp3);
    service->addCharacteristic(characteristic_hum3);


    int ble_temp1 = (0x0000 | temp1)*100;
    if(temp1 > 128)
    {
        ble_temp1 = (0x0000 | (temp1-256))*100;
    }

    int ble_temp2 = (0x0000 | temp2)*100;
    if(temp2 > 128)
    {
        ble_temp2 = (0x0000 | (temp2-256))*100;
    }

    int ble_temp3 = (0x0000 | temp3)*100;
    if(temp3 > 128)
    {
        ble_temp3 = (0x0000 | (temp1-256))*100;
    }
    uint16_t ble_hum1 = (0x0000 | hum1)*100;
    uint16_t ble_hum2 = (0x0000 | hum2)*100;
    uint16_t ble_hum3 = (0x0000 | hum3)*100;

    characteristic_temp1->setValue(ble_temp1);
    characteristic_temp2->setValue(ble_temp2);
    characteristic_temp3->setValue(ble_temp3);
    characteristic_hum1->setValue(ble_hum1);
    characteristic_hum2->setValue(ble_hum2);
    characteristic_hum3->setValue(ble_hum3);
    descriptor_temp1->setValue("Temperature of Sensor 1");
    descriptor_temp2->setValue("Temperature of Sensor 2");
    descriptor_temp3->setValue("Temperature of Sensor 3");
    descriptor_hum1->setValue("Humidity of Sensor 1");
    descriptor_hum2->setValue("Humidity of Sensor 2");
    descriptor_hum3->setValue("Humidity of Sensor 3");

    service->start();
}

void create_service_battery()
{
    BLEService* service = server->createService(BATTERY_SERVICE_UUID);
    BLECharacteristic* characteristic = new BLECharacteristic(BATTERY_CHARACTERISTIC_UUID, 
                                                              BLECharacteristic::PROPERTY_READ);
    BLEDescriptor* descriptor = new BLEDescriptor(BATTERY_DESCRIPTOR_UUID);

    characteristic->addDescriptor(descriptor);
    service->addCharacteristic(characteristic);

    characteristic->setValue(&battery, 1);
    descriptor->setValue("Battery");
    
    
    service->start();
}

void init_ble()
{
    std::string name("Measurement_system_");
    name += char(uint8_t(id/10) + 0x30);
    name += char(uint8_t(id%10) + 0x30);
    for (int i = 0; i < name.length(); i++)
    {
        Serial.print(name[i]);
    }
    Serial.println();

    // Device / Server
    BLEDevice::init(name);
    server = BLEDevice::createServer();
    server->setCallbacks(new CustomCallback());

    // Service Device ID
    create_service_device();

    // Service Battery
    create_service_battery();

    // Service Temperature & Humidity
    create_service_environmental_sensing();

    // Start Advertising 
    server->getAdvertising()->start();
}