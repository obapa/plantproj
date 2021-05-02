#include "extraFunctions.h"

//for temperature circuit
#define TEMPR 10000.0
#define TEMPC1 1.009249522e-03
#define TEMPC2 2.378405444e-04
#define TEMPC3 2.019202697e-07

char* rfFlash (const char *adr){
  //reads string from flash. String must be <=128 bytes
  //Serial.println("rfFlashStart");
  char buffer[128] ={};
  Serial.println(strlen_P(adr));
  for (int k = 0; k <strlen_P(adr); k++) {
    buffer[k] = pgm_read_byte_near(adr+k);
    //Serial.print(pgm_read_byte_near(adr+k));
    //Serial.print(" ");
  }
  //Serial.println();
  //Serial.println("rfFlashEnd");
  return buffer;//return address of str
}

void rfFlash (const char* adr, char* out){
  //copy string from flash to char* out
  /*
  char *temp = new char[128];
  rfFlash(fHost, temp);
  delete[] temp;
  */
  for (int k = 0; k < strlen_P(adr); k++) {
    out[k] = pgm_read_byte_near(adr+k);
  }
}

float readTemp(int analogPin){
  /*Equation, analogPin is a voltage from volt divider circuit
  Find R, then use Steinhart-Hart equation to calculate temperature

  int anIn = analogRead(A0);
  float vOut = 3.0 * anIn / 1024.0;
  float r2 = 10000.0;
  float logR1, r1, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  r1 = r2 * ((3.0/vOut)-1.0);
  logR1 = log(r1);
  T = (1.0 / (c1 + c2*logR1 + c3*logR1*logR1*logR1));
  T -= 273.15;
  T = (T * 9.0)/ 5.0 + 32.0;*/
  float tempBuf;

  tempBuf = log(TEMPR * ((1024.0/analogRead(analogPin))-1));
  tempBuf = (1 / (TEMPC1 + TEMPC2*tempBuf + TEMPC3*tempBuf*tempBuf*tempBuf)) - 273.15;
  tempBuf = (tempBuf * 9)/ 5.0 + 32;
  Serial.printf("Temperature: %f, ", tempBuf);
  Serial.println();
}
