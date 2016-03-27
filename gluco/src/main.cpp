#include "Arduino.h"
#include <eHealth.h>

void
setup()
{
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only

  Serial.println("Gluco v0.1");
  Serial.println("Reading from glucometer...");
  eHealth.readGlucometer();
  delay(100);
}


void
loop()
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

    if (eHealth.glucoseDataVector[i].meridian == 0xBB) {
      hours += 12;
    };
    if (eHealth.glucoseDataVector[i].hour < 10) {
      date.concat("0");
    };
    hours += eHealth.glucoseDataVector[i].hour;
    date.concat(hours);
    date.concat(":");

    date.concat(eHealth.glucoseDataVector[i].minutes);

    // Prints the measurement somewhere?
    Serial.print("Incoming :: ");
    Serial.print(eHealth.glucoseDataVector[i].glucose);
  }

  delay(20000);
}
