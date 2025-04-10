#include "nmea_parser.h"

float nmeaToDecimal(float coordinate, char direction)
{
    int degree = (int)(coordinate / 100);
    float minutes = coordinate - (degree * 100);
    float decimal = degree + (minutes / 60);

    if (direction == 'S' || direction == 'W')
    {
        decimal = -decimal;
    }

    return decimal;
}

/*bool gpsParse(char *strParse, Location *out) {
  if (!strncmp(strParse, "$GNGGA", 6)) {
    sscanf(strParse, "$GNGGA,%f,%f,%c,%f,%c",
      &out->utcTime, &out->nmeaLat, &out->northsouth, &out->nmeaLong, &out->eastwest);
  }
  else if (!strncmp(strParse, "$GNGLL", 6)) {
    sscanf(strParse, "$GNGLL,%f,%c,%f,%c,%f",
      &out->nmeaLat, &out->northsouth, &out->nmeaLong, &out->eastwest, &out->utcTime);
  }
  else if (!strncmp(strParse, "$GNRMC", 6)) {
    sscanf(strParse, "$GNRMC,%f,%c,%f,%c,%f,%c",
      &out->utcTime, &out->posStatus, &out->nmeaLat, &out->northsouth, &out->nmeaLong, &out->eastwest);
  } else {
    return false;
  }

  out->decimalLat  = nmeaToDecimal(out->nmeaLat, out->northsouth);
  out->decimalLong = nmeaToDecimal(out->nmeaLong, out->eastwest);
  return true;
}*/
bool gpsParse(char *strParse, Location *out)
{
    if (strncmp(strParse, "$GNGGA", 6) == 0)
    {
        float utc = 0, lat = 0, lng = 0;
        char ns = '\0', ew = '\0';

        char *checksumPtr = strchr(strParse, '*');
        if (!checksumPtr || (checksumPtr - strParse) < 20)
        {
            return false;
        }

        int parsed = sscanf(strParse, "$GNGGA,%f,%f,%c,%f,%c",
                            &utc, &lat, &ns, &lng, &ew);

        if (parsed == 5)
        {
            out->utcTime = utc;
            out->nmeaLat = lat;
            out->northsouth = ns;
            out->nmeaLong = lng;
            out->eastwest = ew;
            out->decimalLat = nmeaToDecimal(lat, ns);
            out->decimalLong = nmeaToDecimal(lng, ew);

            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

int gps_Validate(char *nmea)
{
    int index = 0;
    int calculatedCheck = 0;

    if (nmea[0] != '$')
        return 0;

    index = 1;

    while (nmea[index] != '*' && nmea[index] != '\0')
    {
        calculatedCheck ^= nmea[index];
        index++;
    }

    if (nmea[index] != '*')
        return 0;

    int receivedChecksum;
    sscanf(&nmea[index + 1], "%02X", &receivedChecksum);

    return (receivedChecksum == calculatedCheck);
}
