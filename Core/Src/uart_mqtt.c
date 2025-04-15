#include "uart_mqtt.h"

const char host[] = "tcp://broker.emqx.io";
const int port = 1883;
const char topic[] = "testmodulesim";
const uint32_t timeOut = 1000;

char ATcommand[100];
uint8_t buffer[160];
uint8_t ATisOK = 0;
uint8_t CGREGisOK = 0;
uint32_t previousTick;

char charData[100];

void SIMTransmit(char *cmd)
{
    //memset(buffer, 0, sizeof(buffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 500);
    HAL_UART_Receive(&huart1, buffer, sizeof(buffer), 500);
    // mPrint1((char*)buffer);
}

/*void mPrint1(const char* format,...){
    va_list args;
    va_start(args, format);
    vsnprintf(buff, sizeof(buff), format, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t*)buff, strlen(buff), 1000);
}*/

void mqttPublish(const char *device_id, float decimalLat, float decimalLong)
{
    if (ATisOK == 0 || CGREGisOK == 0)
    {
        previousTick = HAL_GetTick();
        while (!ATisOK && timeOut > HAL_GetTick() - previousTick )
        {
            SIMTransmit("AT\r\n");
            ATisOK = 1;
        }

        if (ATisOK)
        {
            previousTick = HAL_GetTick();
            while (!CGREGisOK && timeOut > HAL_GetTick() - previousTick)
            {
                SIMTransmit("AT+CGREG?\r\n");
                CGREGisOK = 1;
            }
        }
    }
    if (CGREGisOK)
    {
        SIMTransmit("AT+CMQTTSTART\r\n");
        SIMTransmit("AT+CMQTTACCQ=0,\"client01\"\r\n");
        sprintf(ATcommand, "AT+CMQTTCONNECT=0,\"%s:%d\",60,1\r\n", host, port);
        SIMTransmit(ATcommand);

        sprintf(ATcommand, "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
        SIMTransmit(ATcommand);
        sprintf(ATcommand, "%s\r\n", topic);
        SIMTransmit(ATcommand);

        sprintf(charData, "%s, %.6f, %.6f", device_id, decimalLat, decimalLong);
        sprintf(ATcommand, "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(charData));
        SIMTransmit(ATcommand);
        SIMTransmit(charData);
        SIMTransmit("AT+CMQTTPUB=0,1,60,0\r\n");
    }
}