#include <stdlib.h>
#include <stdio.h>
const int SER = 7;
const int RCLK = 4;
const int SCLK = 6;
const int RedLED = A7;
const int BlueLED = A3;
const int GreenLED = A5;
const int SlowPin = 2;
const int MARIN = 52;
const int RAMIN = 53;
const int SHIFTOUT = 8;
const int OUTDISABLE = 51;
#define SetRed() setColor(255, 0, 0)
#define SetGreen() setColor(0, 255, 0)
#define SetYellow() setColor(255, 255, 0)
#define SetBlue() setColor(0, 0, 255) // stops when mar and ram reads inn data
const char *data[3500] = {
    "0000000000001000", "0000001000000000", "0010000010000100", "0100010000000100", "0000000000000000", "E"};
char *intToString(int integer, int length)
{
    char *str = new char[length + 1];
    for (int i = length - 1; i > -1; i--)
    {
        str[i] = (integer & 1) ? '1' : '0';
        integer >>= 1;
    }
    str[length] = '\0';
    return str;
}

void MARINCycle()
{
    digitalWrite(MARIN, HIGH);
    waitBlue();
    digitalWrite(MARIN, LOW);
}
void RAMINCycle()
{
    digitalWrite(RAMIN, HIGH);
    waitBlue();
    digitalWrite(RAMIN, LOW);
}

void shiftout_en()
{
    digitalWrite(OUTDISABLE, LOW);
    delay(10);
    digitalWrite(SHIFTOUT, LOW);
    delay(10);
}

void shiftout_dis()
{
    digitalWrite(SHIFTOUT, HIGH);
    delay(10);
    digitalWrite(OUTDISABLE, HIGH);
    delay(10);
}

void waitBlue()
{
    setColor(0, 0, 255);
    delay(200);
    while (digitalRead(SlowPin) == 0)
    {
        delay(100);
    }
    SetRed();
}

// flytter til å matche ir, skiller ikke mellom immidiates og instruksjon så rip
void shiftBitsToEnd(char *str)
{
    // Check if the string is null or too short
    if (str == nullptr || strlen(str) < 16)
        return;

    // Temporary arrays to hold segments of the string
    char inst[6] = {0};
    char srba[4] = {0};
    char srcb[4] = {0};
    char destin[4] = {0};

    // Copy specific ranges of bits/characters from the original string
    strncpy(inst, str + 2, 5);    // Copy bits 2-6 (5 bits)
    strncpy(srba, str + 7, 3);    // Copy bits 7-9 (3 bits)
    strncpy(srcb, str + 10, 3);   // Copy bits 10-12 (3 bits)
    strncpy(destin, str + 13, 3); // Copy bits 13-15 (3 bits)

    // Construct the new string with the rearranged parts
    snprintf(str, 17, "%s%s%s%s00", destin, srcb, srba, inst);
}

void setColor(int redValue, int greenValue, int blueValue)
{
    analogWrite(RedLED, redValue);
    analogWrite(GreenLED, greenValue);
    analogWrite(BlueLED, blueValue);
}

// Writes the address into marin
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
    while (digitalRead(SlowPin) == 0)
    {
        delay(100);
    }
    SetRed();
}
void SR_writeWord(char *str)
{
    word strToWord = 0;
    for (int i = 0; i < 16; i++)
    {
        strToWord = (strToWord << 1) | (str[i] - '0'); // Build the 16-bit word, MSB-first
    }

    uint8_t counter;
    for (counter = 0; counter < 16; counter++)
    {
        // Shift the MSB first
        uint8_t shiftVal = (strToWord >> (15 - counter)) & 0x01;
        digitalWrite(SER, shiftVal);
        digitalWrite(SCLK, HIGH);
        delay(10); // Adjust this delay if needed
        digitalWrite(SCLK, LOW);
        delay(10);
    }

    digitalWrite(RCLK, HIGH); // Latch the shift register
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
void Handler(char **data)
{
    shiftout_en();
    int addresseCounter = 0;
    for (int i = 0; i < 3500; i++)
    {
        if (data[i] == "E")
        {
            SR_writeAddress(intToString(0, 16));
            MARINCycle();
            break;
        }
        Serial.print(addresseCounter);
        char *addresseCounterStr = intToString(addresseCounter, 16);
        SR_writeAddress(addresseCounterStr);
        Serial.print("Adresse dataen blir puttet i: ");
        Serial.println(addresseCounter, HEX);
        MARINCycle();
        addresseCounter++;
        Serial.print("Data: ");
        Serial.println(data[i]);
        // shiftBitsToEnd(data[i]);  DONT use yet
        SR_writeWord(data[i]);
        RAMINCycle();
    }
    shiftout_dis();
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
    pinMode(SHIFTOUT, OUTPUT);
    pinMode(OUTDISABLE, OUTPUT);
    digitalWrite(SER, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(MARIN, LOW);
    digitalWrite(RAMIN, LOW);
    digitalWrite(SHIFTOUT, HIGH);
    digitalWrite(OUTDISABLE, HIGH);
    SetRed();
    Serial.print("Done setting up");
    Handler(data);
    SetGreen();
}

void loop()
{
}
