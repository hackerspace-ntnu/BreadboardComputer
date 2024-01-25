#include <Arduino.h>

const unsigned int jOp = 0x0001;   // Binary: 0000 0000 0000 0001
const unsigned int jnOp = 0x0002;  // Binary: 0000 0000 0000 0010
const unsigned int jzOp = 0x0003;  // Binary: 0000 0000 0000 0011
const unsigned int jcOp = 0x0004;  // Binary: 0000 0000 0000 0100
const unsigned int jnzOp = 0x0005; // Binary: 0000 0000 0000 0101

const unsigned int addOp = 0x0000;    // Binary: 0000 0000 0000 0000
const unsigned int xorOp = 0x0008;    // Binary: 0000 0000 0000 1000
const unsigned int nandOp = 0x0010;   // Binary: 0000 0000 0001 0000
const unsigned int shiftrOp = 0x0018; // Binary: 0000 0000 0001 1000

const unsigned int ramoutOp = 0x0020; // Binary: 0000 0000 0010 0000
const unsigned int pcoutOp = 0x0040;  // Binary: 0000 0000 0100 0000
const unsigned int srcaOp = 0x0060;   // Binary: 0000 0000 0110 0000
const unsigned int srcbOp = 0x0080;   // Binary: 0000 0000 1000 0000
const unsigned int aluoutOp = 0x00A0; // Binary: 0000 0000 1010 0000
const unsigned int zeroOp = 0x00E0;   // Binary: 0000 0000 1110 0000
const unsigned int oneOp = 0x0100;    // Binary: 0000 0001 0000 0000
const unsigned int fullfOp = 0x0120;  // Binary: 0000 0001 0010 0000

const unsigned int destinOp = 0x0200; // Binary: 0000 0010 0000 0000
const unsigned int aluinOp = 0x0400;  // Binary: 0000 0100 0000 0000
const unsigned int aluopOp = 0x0600;  // Binary: 0000 0110 0000 0000   setter LE på aluout og flagg register
const unsigned int irinOp = 0x0800;   // Binary: 0000 1000 0000 0000
const unsigned int raminOp = 0x0A00;  // Binary: 0000 1010 0000 0000
const unsigned int marOp = 0x0E00;    // Binary: 0000 1110 0000 0000

const unsigned int pceOp = 0x1000;    // Binary: 0001 0000 0000 0000
const unsigned int resettOp = 0x2000; // Binary: 0010 0000 0000 0000

unsigned int tState = 0x00;
const unsigned int max = 0x08;

bool isFetch = true;

// Function to add two numbers bitwise

const int dataBus[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22}; // 16-bit data bus
const int addressBus[] = {39, 37, 35, 33, 31, 29, 27, 25, 23};                          // 9-bit address bus
const int cePin = 49;                                                                   // Chip Enable
const int wePin = 51;                                                                   // Write Enable
const int finPin = 53;                                                                  // pin that controls the finish light

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(1);
  Serial.print("Start");
  Serial.println("");
  // Initialize data bus pins as OUTPUT
  for (int i = 0; i < 16; i++)
  {
    pinMode(dataBus[i], OUTPUT);
  }

  // Initialize address bus pins as OUTPUT
  for (int i = 0; i < 9; i++)
  {
    pinMode(addressBus[i], OUTPUT);
  }

  // Initialize control signal pins as OUTPUT
  pinMode(cePin, OUTPUT);
  pinMode(wePin, OUTPUT);
  pinMode(finPin, OUTPUT);

  // Initially, set control signals to their default state
  digitalWrite(cePin, HIGH);  // Enable the chip
  digitalWrite(wePin, HIGH);  // Disable write
  digitalWrite(finPin, HIGH); // turn led on
}

void loop()
{
  Serial.println();
  nop();
  delay(100);
  mv();
  delay(100);
  li();
  delay(100);
  ld();
  delay(100);
  ldind();
  delay(100);
  ldio();
  delay(100);
  stio();
  delay(100);
  add();
  delay(100);
  sub();
  delay(100);
  neg();
  delay(100);
  xorinst();
  delay(100);
  andinst();
  delay(100);
  orinst(); // Very important!! Don't remove!!
  delay(100);
  notinst();
  delay(100);
  j();
  delay(100);
  jnz();
  delay(100);
  jimm();
  delay(100);
  addi();
  delay(100);
  st();
  delay(100);
  jz();
  Serial.println("End");
  delay(10000);
  digitalWrite(finPin, LOW);
  delay(1000);
  digitalWrite(finPin, HIGH);
  delay(10000000);
}

void buildinst(int array[], unsigned int address, unsigned int length)
{
  unsigned int fetchaddress = ((address << 4) | getTState());
  Serial.println("Fetching time");
  fetch(fetchaddress);
  for (int i = 0; i < length; i++)
  {
    unsigned int tstateaddress = ((address << 4) | getTState());
    chooseTState(tstateaddress, array[i]);
  }
  Serial.println("Reseting time");
  writeData(((address << 4) | getTState()), resettOp); // Resets the Tstate counter
  Serial.println("End of loop");
  resetT();
  Serial.println();
}
void fetch(unsigned int address)
{
  writeData(address + getTState(), pcoutOp | marOp);           // PCO, MARIN: Move from pc out to marin, used only in fetch
  writeData(address + getTState(), ramoutOp | irinOp | pceOp); // RAMO, IRIN, PCE: Move from ram to instruction register. also pc counts +1
}

void nop()
{
  Serial.println("Nop instruction");
  buildinst(new int[0]{}, 0x00, 0);
  Serial.println();
}

void mv()
{
  Serial.println("MV instruction");
  buildinst(new int[1]{3}, 0x01, 1);
  Serial.println();
}

void li()
{
  buildinst(new int[2]{10, 7}, 0x02, 2);
}

void ld()
{
  buildinst(new int[4]{10, 7, 8, 9}, 0x03, 4);
}

void ldind()
{
  buildinst(new int[2]{8, 9}, 0x04, 2);
}

void ldio()
{
  buildinst(new int[5]{10, 4, 11, 12, 9}, 0x05, 5);
}

void stio()
{
  buildinst(new int[5]{10, 4, 11, 12, 13}, 0x06, 5);
}

void add()
{
  buildinst(new int[3]{14, 6, 5}, 0x07, 3);
}
void sub()
{
  buildinst(new int[7]{15, 16, 25, 17, 25, 11, 5}, 0x08, 7); // må bli en software instruksjon
}
void neg()
{
  buildinst(new int[5]{14, 16, 25, 17, 5}, 0x09, 5);
}
void xorinst()
{
  buildinst(new int[3]{14, 18, 5}, 0x0A, 3);
}
void nand()
{
  buildinst(new int[3]{14, 19, 5}, 0x0B, 3);
}
void andinst()
{
  buildinst(new int[5]{14, 19, 25, 16, 5}, 0x0C, 5);
}
void orinst()
{
  // Banned to the shadowrealm
}
void notinst()
{
  buildinst(new int[3]{14, 16, 5}, 0x0E, 3);
}
void j()
{
  buildinst(new int[1]{20}, 0x0F, 1);
}
void jnz()
{
  buildinst(new int[3]{14, 21, 22}, 0x10, 3);
}
void jimm()
{
  buildinst(new int[2]{10, 23}, 0x11, 2);
}
void addi()
{
  buildinst(new int[4]{10, 4, 11, 5}, 0x12, 4);
}
void st()
{
  buildinst(new int[2]{8, 13}, 0x13, 2);
}
void jz()
{
  buildinst(new int[3]{14, 21, 24}, 0x14, 3);
}
void incT()
{
  tState = (tState + 1);
}

unsigned int getTState()
{
  return tState;
}

void resetT()
{
  tState = 0x00;
}

void writeData(unsigned int address, unsigned int data)
{
  Serial.print(getTState(), HEX);
  Serial.print("\t\t    ");
  printBinaryWithLeadingZeros(address);
  Serial.print("\t\t");
  printBinaryWithLeadingZeros(data);
  Serial.println("");
  // Output the address to the address bus
  for (int i = 0; i < 9; i++)
  {
    digitalWrite(addressBus[i], (address & (1 << i)) ? HIGH : LOW);
  }

  // Output the data to the data bus
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(dataBus[i], (data & (1 << i)) ? HIGH : LOW);
  }

  // Enable write
  digitalWrite(wePin, LOW);
  delay(100);
  // Disable write
  digitalWrite(wePin, HIGH);

  incT();
}

void printBinaryWithLeadingZeros(unsigned int value)
{
  for (int i = 16 - 1; i >= 0; i--)
  {
    Serial.print((value & (1 << i)) ? '1' : '0');
  }
}

void chooseTState(unsigned int tStateAddress, unsigned int i)
{
  Serial.print(i);
  Serial.print("\t\t");
  switch (i)
  {
  case 3:
    writeData(tStateAddress, srcaOp | destinOp); // SRCA, DESTIN: Move from source A register to destin register
    break;

  case 4:
    writeData(tStateAddress, ramoutOp | aluinOp | pceOp); // RAMO, ALUIN, PCE: Move from ram to alu input register and counts PC
    break;

  case 5:
    writeData(tStateAddress, aluoutOp | destinOp); // ALUOUT, DESTIN: Move from alu output register to destin
    break;

  case 6:
    writeData(tStateAddress, srcbOp | aluopOp | addOp); // SRCB, ALUOP, ADD Put src on buss and do ADD operation
    break;

  case 7:
    writeData(tStateAddress, ramoutOp | destinOp | pceOp); // RAMO, DESTIN, PCE
    break;

  case 8:
    writeData(tStateAddress, srcaOp | marOp); // SRCA, MARIN
    break;

  case 9:
    writeData(tStateAddress, ramoutOp | destinOp); // RAMO, DESTIN
    break;

  case 10:
    writeData(tStateAddress, pcoutOp | marOp); // PCO, MARIN
    break;

  case 11:
    writeData(tStateAddress, srcaOp | aluopOp | addOp); // SRCA, ALUOP, ADD
    break;

  case 12:
    writeData(tStateAddress, aluoutOp | marOp); // ALUOUT, MARIN
    break;

  case 13:
    writeData(tStateAddress, raminOp | srcbOp); // RAMIN, SRCB
    break;

  case 14:
    writeData(tStateAddress, srcaOp | aluinOp); // SRCA, ALUIN
    break;

  case 15:
    writeData(tStateAddress, srcbOp | aluinOp); // SRCB, ALUIN
    break;

  case 16:
    writeData(tStateAddress, fullfOp | aluopOp | xorOp); // 0xFFFF, ALUOP, XOR
    break;

  case 17:
    writeData(tStateAddress, oneOp | aluopOp | addOp); // 0x0001, ALUOP, ADD
    break;

  case 18:
    writeData(tStateAddress, srcbOp | aluopOp | xorOp); // SRCB, ALUOP, XOR
    break;

  case 19:
    writeData(tStateAddress, srcbOp | aluopOp | nandOp); // SRCB, ALUOP, NAND
    break;

  case 20:
    writeData(tStateAddress, srcaOp | jOp); // SRCA, J
    break;

  case 21:
    writeData(tStateAddress, zeroOp | aluopOp | addOp); // 0x0000, ALUOP, ADD
    break;

  case 22:
    writeData(tStateAddress, jnzOp | aluoutOp); // JNZ, ALUOUT
    break;

  case 23:
    writeData(tStateAddress, ramoutOp | jOp); // RAMO, J
    break;

  case 24:
    writeData(tStateAddress, jzOp | aluoutOp); // JZ, ALUOUT
    break;

  case 25:
    writeData(tStateAddress, aluoutOp | aluinOp); // ALUOUT, ALUIN: Favorite T state, saves the output of alu to use in next operation
    break;
  }
}
