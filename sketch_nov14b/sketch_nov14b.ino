const unsigned int j = 0x0001;
const unsigned int jn = 0x0002;
const unsigned int jz = 0x0003;
const unsigned int jc = 0x0004;
const unsigned int add = 0x0000;
const unsigned int xorOp = 0x0008;
const unsigned int nandOp = 0x0010;
const unsigned int shiftrOp = 0x0018;
const unsigned int ramoutOp = 0x0020;
const unsigned int pcoutOp = 0x0040;
const unsigned int srcaOp = 0x0060;
const unsigned int srcbOp = 0x0080;
const unsigned int aluoutOp = 0x00A0;
const unsigned int zeroOp = 0x00E0;
const unsigned int oneOp = 0x0100;
const unsigned int fullfOp = 0x0120;
const unsigned int destinOp = 0x0200;
const unsigned int aluinOp = 0x0400;
const unsigned int aluopOp = 0x0600;
const unsigned int irinOp = 0x0800;
const unsigned int raminOp = 0x0A00;
const unsigned int marOp = 0x0E00;
const unsigned int pceOp = 0x1000;
const unsigned int resettOp = 0x2000;


unsigned char tState = 0x00;
const unsigned int max = 0x08;

void incT()
{
  tState = (tState + 1) % 8;
}

void resetT()
{
  tState = 0x00;
}

// Function to add two numbers bitwise
unsigned int addNumbers(const unsigned int num1, const unsigned int num2)
{
  return num1 | num2;
}

const int dataBus[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48}; // 14-bit data bus
const int addressBus[] = {23, 25, 27, 29, 31, 33, 35, 37};  // 8-bit address bus
const int cePin = 49;    // Chip Enable
const int wePin = 51;    // Write Enable
const int finPin = 52;

void setup()
{
  Serial.begin(9600);
  // Initialize data bus pins as OUTPUT
  for (int i = 0; i < 14; i++)
  {
    pinMode(dataBus[i], OUTPUT);
  }

  // Initialize address bus pins as OUTPUT
  for (int i = 0; i < 8; i++)
  {
    pinMode(addressBus[i], OUTPUT);
  }

  // Initialize control signal pins as OUTPUT
  pinMode(cePin, OUTPUT);
  pinMode(wePin, OUTPUT);
  pinMode(finPin, OUTPUT);

  // Initially, set control signals to their default state
  digitalWrite(cePin, HIGH); // Disable the chip
  digitalWrite(wePin, HIGH); // Disable write
  digitalWrite(finPin, HIGH);
}

void writeData(const unsigned int address, const unsigned int data)
{
  Serial.println(address);
  Serial.println(data);
  Serial.println(tState);
  // Output the address to the address bus
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(addressBus[i], (address & (1 << i)) ? HIGH : LOW);
  }

  // Enable the chip
  digitalWrite(cePin, LOW);

  // Enable write
  digitalWrite(wePin, LOW);

  // Output the data to the data bus
  for (int i = 0; i < 14; i++)
  {
    digitalWrite(dataBus[i], (data & (1 << i)) ? HIGH : LOW);
  }

  // Wait for a short time (based on the RAM IC's datasheet) to ensure the data is written.

  // Disable write
  digitalWrite(wePin, HIGH);

  // Disable the chip
  digitalWrite(cePin, HIGH);
  incT();
}

void fetch(unsigned int address)
{
  resetT();
  writeData(address + tState, addNumbers(pcoutOp, marOp));
  writeData(address + tState, addNumbers(addNumbers(ramoutOp, irinOp), pceOp));
}

void nopInstruction()
{
  fetch(0x00);
}

void addInstruction()
{
  unsigned int address = 0x01 + max;
  fetch(address);
  // First T state
  writeData(address + tState, addNumbers(srcaOp, aluopOp));
  writeData(address + tState, addNumbers(addNumbers(srcbOp, add), aluinOp));
  writeData(address + tState, addNumbers(destinOp, aluoutOp));
}

void xorInstruction()
{
  unsigned int address = 0x02+ max;
  fetch(address);
  // First T state
  writeData(address + tState, addNumbers(srcaOp, aluopOp));
  writeData(address + tState, addNumbers(addNumbers(srcbOp, xorOp), aluinOp));
  writeData(address + tState, addNumbers(destinOp, aluoutOp));
}

void nandInstruction(){
  unsigned int address = 0x03+ max;
  fetch(address);
  writeData(address + tState, addNumbers(srcaOp,aluopOp));
  writeData(address + tState, addNumbers(addNumbers(srcbOp, nandOp), aluinOp));
  writeData(address + tState, addNumbers(destinOp, aluoutOp));
}

void shiftrInstruction()
{
  unsigned int address = 0x04+ max;
  fetch(address);
  // First T state
  writeData(address + tState, srcaOp | aluopOp);
  writeData(address + tState, (srcbOp | shiftrOp) | aluinOp);
  writeData(address + tState, destinOp | aluoutOp);
}

void invertInstructions()
{
  unsigned int address = 0x05+ max;
  fetch(address);
  // First T state
  writeData(address + tState, srcaOp | aluopOp);
  writeData(address + tState, (fullfOp | xorOp) | aluinOp);
  writeData(address + tState, destinOp | aluoutOp);
}

void moveInstruction()
{
  unsigned int address = 0x06+ max;
  fetch(address);
  // First T state
  writeData(address + tState, srcaOp | destinOp);
}

void storewordInstruction()
{
  unsigned int address = 0x07+ max;
  fetch(address);
  // First T state
  writeData(address + tState, (pceOp | pcoutOp) | marOp);
  writeData(address + tState, srcaOp | raminOp);
}

void loadwordInstruction()
{
  unsigned int address = 0x08+ max;
  fetch(address);
  // First T state
  writeData(address + tState, (pceOp | pcoutOp) | marOp);
  writeData(address + tState, destinOp | ramoutOp);
}

void jumpInstruction(){
  unsigned int address = 0x09 + max;
  fetch(address);
  writeData(address + tState, (pceOp | pcoutOp) | destinOp);
  writeData(address+ tStater, j | srcaOp);
}

// Implement other instructions similarly...
void finInst(){
  digitalWrite(finPin, LOW);
}
void finStart(){
  digitalWrite(finPin, HIGH);
}

// unsigned int addAddresse(const unsigned int address1)
// {
//   return address1 + tState;
// }


void loop()
{
  nandInstruction();
  finInst();
  delay(500);
  addInstruction();
  finInst();
  delay(500);
  finStart();
  xorInstruction();
  finInst();
  delay(500);
  finStart();
  nandInstruction();
  finInst();
  delay(500);
  finStart();
  shiftrInstruction();
  finInst();
  delay(500);
  finStart();
  invertInstructions();
  finInst();
  delay(500);
  moveInstruction();
  finInst();
  delay(500);
  finStart();
  storewordInstruction();
  finInst();
  delay(500);
  finStart();
  loadwordInstruction();
  delay(500);
  jumpInstruction();
  delay(50000);
}
