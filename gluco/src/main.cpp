#include "Arduino.h"
#include <eHealth.h>

void
setup()
{
  delay(100);
}

void loop() {

  Serial.begin(115200);

  Serial.println("Gluco v0.1");
  Serial.println("Reading glucometer.");

  eHealth.readGlucometer();

  uint8_t numberOfData = eHealth.getGlucometerLength();
  Serial.print(F("Number of measures : "));
  Serial.println(numberOfData, DEC);
  delay(100);


  for (int i = 0; i<numberOfData; i++) {
    // The protocol sends data in this order
    Serial.println(F("=========================================="));

    Serial.print(F("Measure number "));
    Serial.println(i + 1);

    Serial.print(F("Date -> "));
    Serial.print(eHealth.glucoseDataVector[i].day);
    Serial.print(F(" of "));
    Serial.print(eHealth.numberToMonth(eHealth.glucoseDataVector[i].month));
    Serial.print(F(" of "));
    Serial.print(2000 + eHealth.glucoseDataVector[i].year);
    Serial.print(F(" at "));

    if (eHealth.glucoseDataVector[i].hour < 10) {
      Serial.print(0); // Only for best representation.
    }

    Serial.print(eHealth.glucoseDataVector[i].hour);
    Serial.print(F(":"));

    if (eHealth.glucoseDataVector[i].minutes < 10) {
      Serial.print(0);// Only for best representation.
    }
    Serial.print(eHealth.glucoseDataVector[i].minutes);

    if (eHealth.glucoseDataVector[i].meridian == 0xBB)
      Serial.println(F(" pm"));
    else if (eHealth.glucoseDataVector[i].meridian == 0xAA)
      Serial.println(F(" am"));

    Serial.print(F("Glucose value : "));
    Serial.print(eHealth.glucoseDataVector[i].glucose);
    Serial.println(F(" mg/dL"));
  }

  delay(3000);
}
