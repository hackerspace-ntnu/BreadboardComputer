// const int addressBus[] = {23, 25, 27, 29, 31, 33, 35, 37};  // 8-bit address bus
// const int cePin = 49;    // Chip Enable
// const int wePin = 51;    // Write Enable
// const int finPin = 52;


// void setup()
// {
//   Serial.begin(9600);
//   // Initialize address bus pins as OUTPUT
//   for (int i = 0; i < 8; i++)
//   {
//     pinMode(addressBus[i], OUTPUT);
//   }

//   // Initialize control signal pins as OUTPUT
//   pinMode(cePin, OUTPUT);
//   pinMode(wePin, OUTPUT);
//   pinMode(finPin, OUTPUT);

//   // Initially, set control signals to their default state
//   digitalWrite(cePin, LOW); // Disable the chip
//   digitalWrite(wePin, LOW); // Disable write
//   digitalWrite(finPin, HIGH);
// }


// void loop(){
//   unsigned int address = 0x01;
  
//   // Output the address to the address bus
//   for (int i = 0; i < 8; i++)
//   {
//     Serial.println(address & (1 << i));
//     digitalWrite(addressBus[i], (address & (1 << i)) ? HIGH : LOW);
//   }
//   delay(5000000000);
//   digitalWrite(finPin, LOW);
// }