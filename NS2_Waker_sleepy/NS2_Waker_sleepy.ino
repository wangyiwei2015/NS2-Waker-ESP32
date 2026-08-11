#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp_mac.h>
#include <esp_sleep.h>

#define DEVICE_NAME "NS2_Waker"
#define BUTTON_PIN 9 // ESP32-C3 BOOT PIN
// 目标JC蓝牙MAC地址 (Base MAC is BT MAC 的末位 - 2)
const uint8_t baseMac[6] = {0x78, 0x81, 0x8c, 0xXX, 0xXX, 0xXX-2};
const uint8_t mfgData[] = {
    0x53, 0x05,
    0xXX, 0xXX, 0xXX, 0xXX,
    0xXX, 0xXX, 0xXX, 0xXX,
    0xXX, 0xXX, 0xXX, 0xXX,
    0xXX, 0xXX, 0xXX, 0xXX,
    0x0F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
BLEAdvertising *pAdvertising;

void loop() {} // No loops~

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    esp_base_mac_addr_set(baseMac);
    BLEDevice::init(DEVICE_NAME);
    // 调Tx功率提升唤醒距离
    BLEDevice::setPower(ESP_PWR_LVL_P9);
    pAdvertising = BLEDevice::getAdvertising();
    BLEAdvertisementData advData;
    advData.setFlags(0x06);
    advData.setManufacturerData(String((const char*)mfgData, sizeof(mfgData)));
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->setMinInterval(0x0020);
    pAdvertising->setMaxInterval(0x0040);
    pAdvertising->start();
    delay(3000);
    pAdvertising->stop();
    // 配置 GPIO 9 低电平唤醒 Deep Sleep
    esp_deep_sleep_enable_gpio_wakeup((1ULL << BUTTON_PIN), ESP_GPIO_WAKEUP_GPIO_LOW);
    // 等待按键松开后再休眠，避免休眠间被同一次按键重复唤醒
    while (digitalRead(BUTTON_PIN) == 0) delay(10);
    delay(50);
    esp_deep_sleep_start(); // 深度休眠超低功耗
}