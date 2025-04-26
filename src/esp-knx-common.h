#pragma once

#include <Arduino.h>
#include <knx.h>
#include <esp-knx-webserver.h>

#define delayCheckMillis(last, duration) (millis() - last >= duration)
#define delayCheckMicros(last, duration) (micros() - last >= duration)

static unsigned long lastMicros = 0;
static uint8_t hardwareType[LEN_HARDWARE_TYPE] = {0};
static uint8_t firmwareVersion[LEN_HARDWARE_TYPE] = {0};
static bool knxDisabled = false;

void keepCycleTime(uint16_t minCycleTimeMicros);
void setKnxHostname(const char *hostname);
void setKnxMode(knxModeOptions_t state);
String getKnxPhysAddr(void);
knxModeOptions_t getKnxMode(void);
void setFirmwareVersion(uint8_t openKnxId, uint8_t applicationNumber, uint8_t applicationVersion);
bool checkKnxApp();
String getKnxAppDetails(void);