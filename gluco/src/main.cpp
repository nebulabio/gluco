#include "Arduino.h"
#include "../lib/eHealth/eHealth.h"


void setup()
{
     eHealth.readGlucometer();
     Serial.begin(115200);
     delay(100);
}


void loop()
{
     uint8_t data_length = eHealth.getGlucometerLength();

     for (int i = 0; i < data_length; i++) {
          String date = "";
          int hours = 0;

          // The protocol sends data in this order
          // measurement number, date (YYYY:MM:DD:HH:MM), measurement (mg/dL)
          Serial.println(i + 1); // measurement number

          // Formats the data to YYYY:MM:DD:HH:MM
          date.concat(eHealth.glucoseDataVector[i].year);
          date.concat(":");

          date.concat(eHealth.glucoseDataVector[i].month);
          date.concat(":");

          date.concat(eHealth.glucoseDataVector[i].day);
          date.concat(":");

          if (eHealth.glucoseDataVector[i].meridian == 0xBB) { hours += 12; };
          if (eHealth.glucoseDataVector[i].hour < 10)        { date.concat("0"); };
          hours += eHealth.glucoseDataVector[i].hour;
          date.concat(hours);
          date.concat(":");

          date.concat(eHealth.glucoseDataVector[i].minutes);


          // Prints the measurement somewhere?
          Serial.print(eHealth.glucoseDataVector[i].glucose);
     }

     delay(20000);
}
