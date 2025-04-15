#ifndef UART_MQTT_H
#define UART_MQTT_H

#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;
  
extern const char host[];  
extern const int port;  
extern const char topic[];  
extern const uint32_t timeOut;

extern char ATcommand[100];  
extern uint8_t buffer[160];  
extern uint8_t ATisOK;  
extern uint8_t CGREGisOK;  
extern uint32_t previousTick;

extern char charData[100];

void SIMTransmit(char *cmd);
void mqttPublish(const char *device_id, float lat, float lon);

#endif

