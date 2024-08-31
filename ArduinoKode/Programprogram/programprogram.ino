#include <stdio.h>

unsigned int data[32768] = {};

const int addressBus[15] = {50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};

const int dataBus[16] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; // MINSTE SIGNIFICANT BIT FØRST

const int lightPin = 8; // når lyser - er klar til å skrive flere instruksjoner
const int wePin = 52;   // Write enable pin

int standardDelay = 200;

void setup()
{
    Serial.begin(9600);
    pinMode(wePin, OUTPUT);
    pinMode(lightPin, OUTPUT);

    digitalWrite(lightPin, HIGH);
    digitalWrite(wePin, HIGH);

    for (int i = 0; i < 15; i++)
    {
        pinMode(addressBus[i], OUTPUT);
        digitalWrite(addressBus[i], LOW);
    }
    for (int i = 0; i < 16; i++)
    {
        pinMode(dataBus[i], OUTPUT);
        digitalWrite(dataBus[i], LOW);
    }

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

void loop()
{
    {
        digitalWrite(lightPin, LOW);
        standardDelay = 30;
        for (int i; i < 32768; i++)
        {
            write(i, data[i]);
        }
        digitalWrite(lightPin, HIGH);
    }
}