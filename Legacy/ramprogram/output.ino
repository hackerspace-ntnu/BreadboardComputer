#include <stdlib.h>
#include <stdio.h>
const int SER = 26;
const int RCLK = 22;
const int SCLK = 30;
const int RedLED = A10;
const int BlueLED = A12;
const int GreenLED = A14;
const int SlowPin = 34;
const int MARIN = 38;
const int RAMIN = 42;
#define SetRed() setColor(255, 0, 0)
#define SetGreen() setColor(0, 255, 0)
#define SetYellow() setColor(255, 255, 0)
const char *data[3500] = {
"0000010000000000","0000001000000000","0000001001000000","0000001010001000","0010011000000000","0000011011010000","0000001000000000",};
char* intToString(int integer, int length){
  char* str = new char[length+1];
  for(int i = length-1; i > -1; i--){
    str[i] = (integer & 1) ? '1' : '0';
    integer >>= 1;
  }
  str[length] = '\0';
  return str;
}
void setup()
{
    Serial.begin(9600);
    pinMode(SER, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(SlowPin, INPUT);
    pinMode(RedLED, OUTPUT);
    pinMode(BlueLED, OUTPUT);
    pinMode(GreenLED, OUTPUT);
    pinMode(MARIN, OUTPUT);
    pinMode(RAMIN, OUTPUT);
    digitalWrite(SER, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(MARIN, LOW);
    digitalWrite(RAMIN, HIGH);
    SetRed();
    Serial.print("Done setting up");
    Handler(data);
    SetGreen();
}
void MARINCycle(){
  digitalWrite(MARIN, HIGH);
  delay(10);
  digitalWrite(MARIN, LOW);
}
void RAMINCycle(){
  digitalWrite(MARIN, LOW);
  delay(10);
  digitalWrite(MARIN, HIGH);
}
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RedLED, redValue);
  analogWrite(GreenLED, greenValue);
  analogWrite(BlueLED, blueValue);
}
void SR_writeAddress(char *str)
{
    word strToWord = 0;
    for (int i = 0; i < 16; i++)
    {
        strToWord = (strToWord << 1) | (str[i] - '0');
    }
    uint8_t counter;
    word shiftVal;
    for (counter = 0; counter < 16; counter++)
    {
        shiftVal = (strToWord & 0x01);
        digitalWrite(SER, shiftVal);
        digitalWrite(SCLK, HIGH);
        delay(10);
        digitalWrite(SCLK, LOW);
        delay(10);
        strToWord = strToWord >> 1;
    }
    digitalWrite(RCLK, HIGH);
    delay(10);
    digitalWrite(RCLK, LOW);
    Serial.println("Shift register latched");
    SetYellow();
    while(digitalRead(SlowPin) == 0){
      delay(100);
    }
    SetRed();
}
void SR_writeWord(char *str)
{
    word strToWord = 0;
    for (int i = 0; i < 16; i++)
    {
        strToWord = (strToWord << 1) | (str[i] - '0');  // Build the 16-bit word, MSB-first
    }

    uint8_t counter;
    for (counter = 0; counter < 16; counter++)
    {
        // Shift the MSB first
        uint8_t shiftVal = (strToWord >> (15 - counter)) & 0x01;
        digitalWrite(SER, shiftVal);
        digitalWrite(SCLK, HIGH);
        delay(10);  // Adjust this delay if needed
        digitalWrite(SCLK, LOW);
        delay(10);
    }

    digitalWrite(RCLK, HIGH);  // Latch the shift register
    delay(10);
    digitalWrite(RCLK, LOW);

    Serial.println("Shift register latched");
    SetYellow();

    // Wait for SlowPin signal to proceed
    while (digitalRead(SlowPin) == 0)
    {
        delay(100);
    }

    SetRed();
}
void Handler(char **data){
  int addresseCounter = 0;
  for (int i = 0; i < 3500; i++){
    Serial.print(addresseCounter);
    char* addresseCounterStr = intToString(addresseCounter, 16);
    SR_writeAddress(addresseCounterStr);
    Serial.print("Adresse dataen blir puttet i: ");
    Serial.println(addresseCounter, HEX);
    MARINCycle();
    addresseCounter++;
    Serial.print("Data: ");
    Serial.println(data[i]);
    SR_writeWord(data[i]);
    RAMINCycle();
  }
}
void loop()
{}
