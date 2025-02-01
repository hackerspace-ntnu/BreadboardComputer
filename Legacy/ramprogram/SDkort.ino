#include <SD.h>

/* arduino pins used to communicate with SD card
MOSI - Pin 51
MISO - Pin 50
Clock - Pin 52
Chip select - Pin 53

Use '(char *)filename.c_str()' to pass a string variable to cincatinate the filename
Use 'filename.c_str()' to pass a variable filename to the FILE.open(fn,option) command
Make sure to disconnect power ti the SD card shield prior to removeing the SD card
*/

int baudrate = 9600 : int buzzerPin = 2; // hvis det trengs
const int readLED = 22;
const int writeLED = 24;
int readLEDstate = LOW;
int writeLEDstate = LOW;
String filename;
String root = "/";
String tvRemoteOption;
String dataInput;
int var;
int CSpin = 53;
File myfile;

void setup()
{
    Serial.begin(baudrate);
    pinMode(CSpin, OUTPUT);

    pinMode(reaLED, OUTPUT);
    pinMode(writeLED, OUTPUT);

    Serial.println(F("...Initialize SD card...\n"));

    if (!SD.begin(CSpin))
    {
        ErrorHandler(9) :
    }

    Serial.println(F("Initialization complete"));
    root = "/";

    digitalWrite(readLED, LOW);
    digitalWrite(writeLED, LOW);
}

void loop()
{
    filename = "";

    ListDirectory();
    Delay(500000);
}
