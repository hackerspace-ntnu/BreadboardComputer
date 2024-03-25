//Mikroinstruksjoner
const unsigned int j = 0x0001;   // Binary: 0000 0000 0000 0001
const unsigned int jn = 0x0002;  // Binary: 0000 0000 0000 0010
const unsigned int jz = 0x0003;  // Binary: 0000 0000 0000 0011
const unsigned int jc = 0x0004;  // Binary: 0000 0000 0000 0100
const unsigned int jnz = 0x0005; // Binary: 0000 0000 0000 0101
const unsigned int add = 0x0000;    // Binary: 0000 0000 0000 0000
const unsigned int xorr = 0x0008;    // Binary: 0000 0000 0000 1000
const unsigned int nand = 0x0010;   // Binary: 0000 0000 0001 0000
const unsigned int shiftr = 0x0018; // Binary: 0000 0000 0001 1000
const unsigned int ramout = 0x0020; // Binary: 0000 0000 0010 0000
const unsigned int pcout = 0x0040;  // Binary: 0000 0000 0100 0000
const unsigned int srca = 0x0060;   // Binary: 0000 0000 0110 0000
const unsigned int srcb = 0x0080;   // Binary: 0000 0000 1000 0000
const unsigned int aluout = 0x00A0; // Binary: 0000 0000 1010 0000
const unsigned int zero = 0x00E0;   // Binary: 0000 0000 1110 0000
const unsigned int one = 0x0100;    // Binary: 0000 0001 0000 0000
const unsigned int fullf = 0x0120;  // Binary: 0000 0001 0010 0000
const unsigned int destin = 0x0200; // Binary: 0000 0010 0000 0000
const unsigned int aluin = 0x0400;  // Binary: 0000 0100 0000 0000
const unsigned int alu = 0x0600;  // Binary: 0000 0110 0000 0000   setter LE på aluout og flagg register
const unsigned int irin = 0x0800;   // Binary: 0000 1000 0000 0000
const unsigned int ramin = 0x0A00;  // Binary: 0000 1010 0000 0000
const unsigned int marin = 0x0E00;    // Binary: 0000 1110 0000 0000
const unsigned int pce = 0x1000;    // Binary: 0001 0000 0000 0000 //øker pc med 1
const unsigned int reset = 0x2000; // Binary: 0010 0000 0000 0000

//Instruksjoner
const int nop[8] = {pcout|marin, ramout|irin|pce, reset, 0,0,0,0,0};
const int mv[8] ={pcout|marin, ramout|irin|pce, srca|destin, reset, 0,0,0,0};
const int li[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|destin|pce, reset, 0,0,0};
const int ld[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|destin|pce, srca|marin, ramout|destin, reset, 0};
const int ldind[8] = {pcout|marin, ramout|irin|pce, srca|marin, ramout|destin, reset, 0,0,0};
const int ldio[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|aluin|pce, srca|alu|add, aluout|marin, ramout|destin, reset};
const int stio[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|aluin|pce, srca|alu|add, aluout|marin, ramin|srcb, reset};
const int addInst[8] = {pcout|marin, ramout|irin|pce, srca|aluin, srcb|alu|add, aluout|destin, reset, 0,0};
const int sub[8] = {pcout|marin, ramout|irin|pce, reset ,0,0,0,0,0}; //Sub har ikke plass på 8 T-state, vi trenger ikke sub tho
const int neg[8] = {pcout|marin, ramout|irin|pce, srca|aluin, fullf|alu|xorr, aluout|aluin, one|alu|add, aluout|destin, reset};
const int xorInst[8] = {pcout|marin, ramout|irin|pce, srca|aluin, srcb|alu|xorr, aluout|destin, reset, 0,0};
const int nandInst[8] = {pcout|marin, ramout|irin|pce, srca|aluin, srcb|alu|nand, aluout|destin, reset, 0,0};
const int andInst[8] = {pcout|marin, ramout|irin|pce, srca|aluin, srcb|alu|nand, aluout|aluin, fullf|alu|xorr, aluout|destin, reset};
const int orInst[8] = {pcout|marin, ramout|irin|pce,  reset, 0,0,0,0,0}; //Or instruksjon ikke implimentert
const int notInst[8] = {pcout|marin, ramout|irin|pce, srca|aluin, fullf|alu|xorr, aluout|destin, reset, 0,0};
const int jump[8] = {pcout|marin, ramout|irin|pce, srca|j, reset, 0,0,0,0};
const int jumpnz[8] = {pcout|marin, ramout|irin|pce, srca|aluin, zero|alu|add, jnz|aluout, reset, 0,0};
const int jumpimm[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|j, reset, 0,0,0};
const int addimm[8] = {pcout|marin, ramout|irin|pce, pcout|marin, ramout|aluin|pce, srca|alu|add, aluout|destin, reset, 0};
const int store[8] = {pcout|marin, ramout|irin|pce, srca|marin, ramin|srcb, reset, 0,0,0};
const int jumpz[8] = {pcout|marin, ramout|irin|pce, srca|aluin, zero|alu|add, jz|aluout, reset, 0,0};

#define maxTstate 8
#define antallInstruksjoner 21

//satt til 6 midlertidig:
const int *instruksjoner[antallInstruksjoner] = {nop, mv, li, ld, ldind, ldio,stio,addInst,sub,neg,
  xorInst,nandInst,andInst,orInst,notInst,jump,jumpnz,jumpimm,addimm,store,jumpz};

const int lightPin = 8; // når lyser - er klar til å skrive flere instruksjoner
const int instrPin = 7; //pin som går gjennom en instruksjon
const int expressPin = 6; //Pin som går gjennom alle instruksjoneen fort og automatisk!!!!
const int slowPin = 5; //Pin som manuelt går gjennom hver mikroinstruksjon.
const int wePin = 52; //Write enable pin

const int addressBus[9] = {38,36,34,32,30,28,26,24,22};

const int dataBus[15] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25}; // 15 bit data bus (bruker bare 14 bit) MINSTE SIGNIFICANT BIT FØRST

bool oneInstr = false;
bool expressActivated = false;
bool slowPressed = false;

int standardDelay = 200;

void setup(){
  Serial.begin(9600);
  pinMode(wePin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(instrPin, INPUT_PULLUP);
  pinMode(expressPin, INPUT_PULLUP);
  pinMode(slowPin, INPUT_PULLUP);

  digitalWrite(lightPin, HIGH);
  digitalWrite(wePin, HIGH);

  for(int i =0; i<9; i++){
    pinMode(addressBus[i], OUTPUT);
    digitalWrite(addressBus[i],LOW);
  }
  for(int i =0; i<15; i++){
  pinMode(dataBus[i], OUTPUT);
  digitalWrite(dataBus[i],LOW);
  }

  Serial.print("Done setting up");
}

//Returnerer String med minst signifcant bit til venstre aka motsatt av vanlig.
char* intToString(int integer, int length){
  char* str = new char[length+1];
  for(int i =0; i<length; i++){
    str[i] = (integer & 1) ? '1' : '0';
    integer >>=1;
  }
  str[length]='\0';
  return str;
}

//Skriver data til adresse
void write(int address, int data){
    char* bitAddr = intToString(address, 9);
    char* bitData = intToString(data, 15);
    Serial.print("adresse: ");
    Serial.println(bitAddr);
    Serial.print("data: ");
    Serial.println(bitData);
    for(int i =0; i<9; i++){
      digitalWrite(addressBus[i], bitAddr[i]=='1' ? HIGH : LOW);
    }
    for (int i =0; i<15; i++){
      digitalWrite(dataBus[i], bitData[i] =='1' ? HIGH : LOW);
    }
    //Lagrer informasjonen
    digitalWrite(wePin, LOW);
    delay(50);
    digitalWrite(wePin, HIGH);

    delete[] bitAddr;
    delete[] bitData;
    delay(standardDelay);
}

int instrC =0; //instruksjon Counter
int mikroC =0; //mikroinstruksjon Counter
void loop(){
  oneInstr = digitalRead(instrPin);
  expressActivated = digitalRead(expressPin);
  slowPressed = digitalRead(slowPin);


  //Går gjennom en instruksjon
  if(oneInstr && instrC < antallInstruksjoner){
    digitalWrite(lightPin, LOW);
    Serial.print("begynner på neste instruksjon: ");
    Serial.println(instrC);
    for(; mikroC<sizeof(nop)/sizeof(nop[0]); mikroC++){
      write(instrC*maxTstate+mikroC, instruksjoner[instrC][mikroC]);
    }
    mikroC =0;
    instrC++;
    digitalWrite(lightPin, HIGH);
  }

  //Går gjennom alle instruksjonene med insane speed
  if(expressActivated){
    digitalWrite(lightPin, LOW);
    standardDelay =30;
    for(; instrC<antallInstruksjoner; instrC++){
      Serial.print("begynner på neste instruksjon: ");
      Serial.println(instrC);
      for(; mikroC<sizeof(nop)/sizeof(nop[0]); mikroC++){
        write(instrC*maxTstate+mikroC, instruksjoner[instrC][mikroC]);
      }
      mikroC=0;
    }
    digitalWrite(lightPin, HIGH);
  }

  //Går gjennom en mikrointruksjon
  if(slowPressed && instrC < antallInstruksjoner){
    if(mikroC==8){
      instrC++;
      mikroC =0;
    }
    digitalWrite(lightPin, LOW);
    Serial.print("nåværende instuksjon: ");
    Serial.println(instrC);
    Serial.print("begynner på neste mikroinstruksjon: ");
    Serial.println(mikroC);

    write(instrC*maxTstate+mikroC, instruksjoner[instrC][mikroC]);
    mikroC++;
  }
}