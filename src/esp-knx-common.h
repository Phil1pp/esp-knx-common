#pragma once

#include <Arduino.h>
#include <knx.h>
#include <esp-knx-webserver.h>

#define delayCheckMillis(last, duration) (millis() - last >= duration)
#define delayCheckMicros(last, duration) (micros() - last >= duration)

void keepCycleTime(uint16_t minCycleTimeMicros);
void setKnxHostname(const char *hostname);
void setKnxMode(knxModeOptions_t state);
String getKnxPhysAddr(void);
knxModeOptions_t getKnxMode(void);
void setFirmwareVersion(uint8_t openKnxId, uint8_t applicationNumber, uint8_t applicationVersion);
bool checkKnxApp();
String getKnxAppDetails(void);
bool getKnxActive(void);
void restartDeviceCallback(void);
void knxProgLedOnCallback(void);
void knxProgLedOffCallback(void);