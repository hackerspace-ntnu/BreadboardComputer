//instruksjonsformat: dest, srcB, srcA, opcode 3bit, 3bit, 3bit, 7bit (vi burker 5 bit):
#include <stdio.h>
const int ramWrite = 22;
const int adrWrite = 50;
const int adrOutput = 52;
int databus[16];

#define nop 0
#define mv 1
#define li 2
#define ld 3
#define ldind 4
#define ldio 5
#define addInst 6
#define sub 7
#define neg 8
#define xorInst 9
#define nandInst 10
#define andInst 11
#define orInst 12
#define notInst 13
#define jump 14
#define jumpnz  15
#define jumpimm  16
#define addimm  17
#define store  18
#define jumpz  19

void setup(){
  pinMode(ramWrite, OUTPUT);
  pinMode(adrWrite, OUTPUT);
  pinMode(adrOutput, OUTPUT);
  Serial.begin(9600);

  delay(1000);
  Serial.println("NY data");

  for(int i =23; i<= 53; i+=2){
    pinMode(i,OUTPUT);
    databus[(i-23)/2]=i;
    printf("Test %d",i);
    Serial.println(i);
  }
  for(int i =0; i < sizeof(databus)/sizeof(int); i++){
    Serial.println(databus[i]);
  }
    int instr =0;
  Serial.println("test4");
  //instruksjon, destination, srca, srcb
  instr = createInstruction(mv,1,2,3);
  Serial.println(intToString(instr,16));
  //Serial.println(createInstruction(nop,0,0,0));
}

int createInstruction(int opcode, int dest, int srca, int srcb){
  return opcode<<9 | srca <<6 | srcb <<3 | dest;
}

char* intToString(int integer, int length){
  char* str = new char[length+1];
  for(int i =0; i<length; i++){
    str[i] = (integer & 1) ? '1' : '0';
    integer >>=1;
  }
  str[length]='\0';
  return str;
}

void loop(){

}