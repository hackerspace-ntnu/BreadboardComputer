#include <stdlib.h>
#include <stdio.h>
#include "assembler.c"

// const int addressBus[15] = {50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};

// const int dataBus[16] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; // MINSTE SIGNIFICANT BIT FØRST

const int SER = 26;
const int RCLK = 22; 
const int SCLK = 30;

// const int lightPin = 8; // når lyser - er klar til å skrive flere instruksjoner
// const int wePin = 52;   // Write enable pin

int standardDelay = 200;

#define antallInstruksjoner 21

const unsigned int nop = 0x00;     // 0000 0000
const unsigned int mv = 0x01;      // 0000 0001
const unsigned int li = 0x02;      // 0000 0010
const unsigned int ld = 0x03;      // 0000 0011
const unsigned int ldind = 0x04;   // 0000 0100
const unsigned int ldio = 0x05;    // 0000 0101
const unsigned int stio = 0x06;    // 0000 0110
const unsigned int add = 0x07;     // 0000 0111
const unsigned int sub = 0x08;     // 0000 1000 brukes ikke
const unsigned int neg = 0x09;     // 0000 1001
const unsigned int xorInst = 0x0A;     // 0000 1010
const unsigned int nand = 0x0B;    // 0000 1011
const unsigned int andInst = 0x0C;     // 0000 1100
const unsigned int orInst = 0x0D;      // 0000 1101
const unsigned int notInst = 0x0E;     // 0000 1110
const unsigned int jump = 0x0F;    // 0000 1111
const unsigned int jumpnz = 0x10;  // 0001 0000
const unsigned int jumpimm = 0x11; // 0001 0001
const unsigned int addimm = 0x12;  // 0001 0010
const unsigned int store = 0x13;   // 0001 0011
const unsigned int jumpz = 0x14;   // 0001 0100

const unsigned int r0 = 0x0; // 0000
const unsigned int r1 = 0x1; // 0001
const unsigned int r2 = 0x2; // 0010
const unsigned int r3 = 0x3; // 0011
const unsigned int r4 = 0x4; // 0100
const unsigned int r5 = 0x5; // 0101
const unsigned int r6 = 0x6; // 0110
const unsigned int r7 = 0x7; // 0111

const char* data[3500] = {"0001100001100010","0000010010010000","0000000000101010",};
const unsigned int instruksjoner[antallInstruksjoner] = {nop, mv, li, ld, ldind, ldio, stio, add, sub, neg, xorInst, nand, andInst, orInst, notInst, jump, jumpnz, jumpimm, addimm, store, jumpz};

const int localUpload = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(SER, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);

    digitalWrite(SER, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(RCLK, LOW);

    // digitalWrite(lightPin, HIGH);
    // digitalWrite(wePin, HIGH);

    // for (int i = 0; i < 15; i++)
    // {
    //     pinMode(addressBus[i], OUTPUT);
    //     digitalWrite(addressBus[i], LOW);
    // }
    // for (int i = 0; i < 16; i++)
    // {
    //     pinMode(dataBus[i], OUTPUT);
    //     digitalWrite(dataBus[i], LOW);
    // }

    Serial.print("Done setting up");
}

// Returnerer String med minst signifcant bit til venstre aka motsatt av vanlig.
char *intToString(int integer, int length)
{
    char *str = new char[length + 1];
    for (int i = 0; i < length; i++)
    {
        str[i] = (integer & 1) ? '1' : '0';
        integer >>= 1;
    }
    str[length] = '\0';
    return str;
}

// Skriver data til adresse
void write(int address, int data)
{
    char *bitAddr = intToString(address, 15);
    char *bitData = intToString(data, 16);
    for (int i = 0; i < 15; i++)
    {
        digitalWrite(addressBus[i], bitAddr[i] == '1' ? HIGH : LOW);
    }
    for (int i = 0; i < 16; i++)
    {
        digitalWrite(dataBus[i], bitData[i] == '1' ? HIGH : LOW);
    }
    // Lagrer informasjonen
    digitalWrite(wePin, LOW);
    delay(50);
    digitalWrite(wePin, HIGH);

    delete[] bitAddr;
    delete[] bitData;
    delay(standardDelay);
}

void assembler()
{
}

void loop()
{
    if (localUpload == 0)
    {
        digitalWrite(lightPin, LOW);
        standardDelay = 30;
        for (int i = 0; i < 32768; i++)
        {
            write(i, data[i]);
        }
        digitalWrite(lightPin, HIGH);
    }
    else
    {
    }
}
