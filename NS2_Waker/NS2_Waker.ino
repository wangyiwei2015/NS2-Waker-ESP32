#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp_mac.h>

#define DEVICE_NAME "NS2_Waker"
#define BUTTON_PIN 9 // ESP32-C3 BOOT PIN
// 目标JC蓝牙MAC地址 (Base MAC is BT MAC 的末位 - 2)
const uint8_t baseMac[6] = {0x78, 0x81, 0x8c, 0xXX, 0xXX, 0xXX-2};
BLEAdvertising *pAdvertising;

void initBLE() {
    //Serial.println(">>> 初始化 BLE 模块...");
    esp_base_mac_addr_set(baseMac);
    BLEDevice::init(DEVICE_NAME);
    pAdvertising = BLEDevice::getAdvertising();
    BLEAdvertisementData advData;
    // 设置 Flags (对应 len 0x02, type 0x01, data 0x06)
    advData.setFlags(0x06);
    // 传入 Manufacturer Data（去掉前 len 1B, type FF）
    const uint8_t mfgData[] = {
        0x53, 0x05, // 0x0553 (Vendor ID) + 26 bytes Payload
        0xXX, 0xXX, 0xXX, 0xXX,
        0xXX, 0xXX, 0xXX, 0xXX,
        0xXX, 0xXX, 0xXX, 0xXX,
        0xXX, 0xXX, 0xXX, 0xXX,
        0x0F, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    advData.setManufacturerData(String((const char*)mfgData, sizeof(mfgData)));
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->setMinInterval(0x0020); // 20ms (0x20 * 0.625ms)
    pAdvertising->setMaxInterval(0x0040);
    //Serial.println(">>> BLE 配置就绪，等待按键触发。");
}

void triggerWakeup() {
    pAdvertising->start();
    delay(3000); // 持续高频广播
    pAdvertising->stop();
}

void setup() {
    //Serial.begin(115200);
    // 等待 USB CDC 串口就绪（超时 2 秒跳过，防止脱机无法启动）
    //unsigned long start = millis();
    //while (!Serial && (millis() - start < 2000));
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    initBLE();
}

void loop() {
    if (digitalRead(BUTTON_PIN) == 0) {
        triggerWakeup();
        // 简易按键去抖与释放等待
        while (digitalRead(BUTTON_PIN) == 0) delay(20);
        delay(200); // 释放防抖
    }
    delay(20);
}
