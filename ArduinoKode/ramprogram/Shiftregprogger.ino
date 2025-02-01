#include "types.h"

// init function, writeByte
void SR_init(Shift_Register *sr)
{
    pinMode(22, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(28, OUTPUT);

    // Debugging message
    Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}
// Byte value and write to data pin
// 0b
void SR_writeByte(const Shift_Register *const sr, byte val)
{
    uint8_t counter;
    byte shiftVal;

    // Debugging message
    Serial.print("Writing byte: ");
    Serial.println(val, BIN); // Print the value in binary

    for (counter = 0; counter < NUM_BITS_IN_BYTES; counter++)
    {
        shiftVal = (val & 0x01);
        digitalWrite(24, shiftVal);

        // Debugging message for each bit
        Serial.print("Bit ");
        Serial.print(counter);
        Serial.print(": ");
        Serial.println(shiftVal);

        // Toggle shift register clock (SCLK)
        digitalWrite(28, HIGH);
        delay(10);
        digitalWrite(28, LOW);
        delay(10);

        val = val >> 1;
    }

    // Latch the register (RCLK)
    digitalWrite(26, HIGH);
    delay(10);
    digitalWrite(26, LOW);

    // Debugging message
    Serial.println("Shift register latched");
}

Shift_Register sr1 =
    {
        .SER = 4,  // data
        .RCLK = 5, // shift register clock
        .SCLK = 6  // store data clock
};

void setup()
{
    SR_init(&sr1);
    SR_writeByte(&sr1, 0x03);
}

void loop()
{
}
