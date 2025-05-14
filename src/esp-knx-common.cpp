#include "esp-knx-common.h"

unsigned long lastMicros = 0;
uint8_t hardwareType[LEN_HARDWARE_TYPE] = {0};
uint8_t firmwareVersion[LEN_HARDWARE_TYPE] = {0};
bool knxDisabled = false;

void keepCycleTime(uint16_t minCycleTimeMicros)
{
    unsigned long cycleTime = micros() - lastMicros;
    if (cycleTime < minCycleTimeMicros)
    {
        // delay here until minimum cycle time is reached
        // necessary for constant LED fading times
        delayMicroseconds(minCycleTimeMicros - cycleTime);
    }
    lastMicros = micros();
}

void setKnxHostname(const char *hostname)
{
        // PID_FRIENDLY_NAME is used to identify the device over Search Request from ETS. If not configured, PID_FRIENDLY_NAME is empty and so is the Name in the SearchReqest.
        uint8_t NoOfElem = 30;
        uint8_t *friendlyName = new uint8_t[NoOfElem];
        strncpy((char *)friendlyName, hostname, NoOfElem);
        friendlyName[NoOfElem - 1] = '\0'; // make sure the string is null terminated
        knx.bau().propertyValueWrite(OT_IP_PARAMETER, 0, PID_FRIENDLY_NAME, NoOfElem, 1, friendlyName, 0);
}

void setKnxMode(knxModeOptions_t state)
{
    knxDisabled = state == KNX_MODE_OFF;
    knx.progMode(state == KNX_MODE_PROG);
}

String getKnxPhysAddr()
{
    uint16_t indAddr = knx.individualAddress();
    uint8_t area = indAddr >> 12;
    uint8_t line = (indAddr >> 8) & 0x0F;    
    uint8_t member = indAddr & 0xFF;
    return String(area) + "." + String(line) + "." + String(member);
}

knxModeOptions_t getKnxMode()
{
    if(knxDisabled)
    {
        return KNX_MODE_OFF;
    }
    else
    {
        return knx.progMode() ? KNX_MODE_PROG : KNX_MODE_NORMAL;
    }
}

void setFirmwareVersion(uint8_t openKnxId, uint8_t applicationNumber, uint8_t applicationVersion)
{
    firmwareVersion[1] = 0xFA;
    firmwareVersion[2] = openKnxId;
    firmwareVersion[3] = applicationNumber;
    firmwareVersion[4] = applicationVersion;
    knx.bau().deviceObject().hardwareType(firmwareVersion);
}

bool checkKnxApp(void)
{
    uint8_t NoOfElem = 1;
    uint8_t *data;
    uint32_t length;
    knx.bau().propertyValueRead(OT_APPLICATION_PROG, 0, PID_PROG_VERSION, NoOfElem, 1, &data, length);
    if(length == 5)
    {
        memcpy(hardwareType, data, length);
        delete[] data;
        // check if the firmware application is the same as the loaded application
        // check if the major version (x.0) is the same
        bool knxAppOk = memcmp(hardwareType, firmwareVersion, 4) == 0 && hardwareType[4]/16 == firmwareVersion[4]/16;
        Serial.printf("Hardware type: %02X %02X %02X %02X %02X\n", hardwareType[0], hardwareType[1], hardwareType[2], hardwareType[3], hardwareType[4]);
        Serial.printf("Firmware type: %02X %02X %02X %02X %02X\n", firmwareVersion[0], firmwareVersion[1], firmwareVersion[2], firmwareVersion[3], firmwareVersion[4]);
        return knxAppOk;
    }
    return false;
}

String getKnxAppDetails(void)
{
    return "Firmware:  0x" + String(uint16_t(256)*firmwareVersion[2]+firmwareVersion[3], HEX) + ", Version: " + String(firmwareVersion[4]/16,DEC) + "." + String(firmwareVersion[4]%16,DEC) +
           " Loaded App: 0x" + String(uint16_t(256)*hardwareType[2]+hardwareType[3], HEX) + ", Version: " + String(hardwareType[4]/16,DEC) + "." + String(hardwareType[4]%16,DEC);
}

bool getKnxActive(void)
{
    return knxDisabled == false;
}

void restartDeviceCallback()
{
    //knx.bau().writeMemory();
    ESP.restart();
}