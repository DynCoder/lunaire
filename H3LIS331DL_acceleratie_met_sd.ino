
/* get accelerate data of H3LIS331DL
 * Auth : lawliet(lawliet.zou@gmail.com)
 * version : 0.1
 */

#include <H3LIS331DL.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

//please get these value by running H3LIS331DL_AdjVal Sketch.
#define VAL_X_AXIS  40
#define VAL_Y_AXIS  -98
#define VAL_Z_AXIS  500

H3LIS331DL h3lis;

const int chipSelect = 4;

unsigned long millis_start;
unsigned long millis_end;
unsigned long millis_flight;
int i;
bool flying = false;


void setup(){
  Serial.begin(9600);
  h3lis.init();
  h3lis.importPara(VAL_X_AXIS,VAL_Y_AXIS,VAL_Z_AXIS);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop(){
  String dataString = "";

  double xyz[3];
  h3lis.getAcceleration(xyz);
    
/*  Serial.print(xyz[0]);
  Serial.print("\t");
  Serial.print(xyz[1]);
  Serial.print("\t");
  Serial.print(xyz[2]);
  Serial.println("");*/

  if (flying == false && (xyz[0]) > 2){
    Serial.println("Takeoff detected");
    millis_start = millis();
    flying = true;
  }
  
  if (flying == true){
    millis_end = millis();
    millis_flight = millis_end - millis_start;
    dataString += String(millis_flight);

    dataString += ";";

    dataString += String(xyz[0]);
        dataString += ";";
    dataString += String(xyz[1]);
        dataString += ";";
    dataString += String(xyz[2]);

    File dataFile = SD.open("values.csv", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening values.csv");
    }
  }
  
  if (flying == true && (xyz[0]) <= 1){
    flying = false;
  }
  
  delay(0);
}
