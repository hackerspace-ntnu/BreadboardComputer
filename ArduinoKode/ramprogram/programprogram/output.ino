#include <stdlib.h>
#include <stdio.h>
const int SER = 26;
const int RCLK = 22;
const int SCLK = 30;
int standardDelay = 200;

const char *data[3500] = {
    "0001100001100010",
    "0000010010010000",
    "0000000000101010",
};


void setup()
{
    Serial.begin(9600);
    pinMode(SER, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);

    digitalWrite(SER, LOW);
    digitalWrite(SCLK, LOW);
    digitalWrite(RCLK, LOW);

    Serial.print("Done setting up");
}

void SR_writeByte(char *str)
{

    word strToWord = 0;
    for (int i = 0; i < 16; i++)
    {
        strToWord = (strToWord << 1) | (str[i] - '0'); // Shift left and OR to append the bits
    }

    uint8_t counter;
    word shiftVal;

    // Debugging message
    Serial.print("Writing word: ");
    Serial.println(strToWord, BIN); // Print the value in binary

    for (counter = 0; counter < 16; counter++)
    {
        shiftVal = (strToWord & 0x01);
        digitalWrite(SER, shiftVal);

        // Debugging message for each bit
        Serial.print("Bit ");
        Serial.print(counter);
        Serial.print(": ");
        Serial.println(shiftVal);

        // Toggle shift register clock (SCLK)
        digitalWrite(SCLK, HIGH);
        delay(10);
        digitalWrite(SCLK, LOW);
        delay(10);

        strToWord = strToWord >> 1;
    }

    // Latch the register (RCLK)
    digitalWrite(RCLK, HIGH);
    delay(10);
    digitalWrite(RCLK, LOW);

    // Debugging message
    Serial.println("Shift register latched");
}

void loop()
{

}
