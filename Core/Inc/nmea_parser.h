#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	float nmeaLong;
	float nmeaLat;
	float utcTime;
	char northsouth;
	char eastwest;
	char posStatus;
	float decimalLong;
	float decimalLat;
}Location;

extern Location GPS;

float nmeaToDecimal(float coordinate, char direction);

bool gpsParse(char *strParse, Location *out);

int gps_Validate(char *nmea);

#endif
