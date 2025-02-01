// Linux command for å kjøre gcc HC-Assembler.c -o HC-Assembler && cat kode.txt | ./HC-Assembler > output.ino

// li må ha samme srca som destin

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N_STATES 36
#define START_STATE 0
#define ERROR 36
#define ACCEPT 40

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
const unsigned int xor = 0x0A;     // 0000 1010
const unsigned int nand = 0x0B;    // 0000 1011
const unsigned int and = 0x0C;     // 0000 1100
const unsigned int or = 0x0D;      // 0000 1101
const unsigned int not = 0x0E;     // 0000 1110
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

const unsigned int registers[8] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};

int transition_table[N_STATES][256];
int state = 0;
void get_registers();
int fileNotEmpty = 0;

// denne er feil, skal egt ikke være mer enn en bokstav i hver state
// er litt ork å sette opp
void initialize_table()
{
    for (int s = 0; s < N_STATES; s++)
    {
        for (int c = 0; c < 256; c++)
        {
            transition_table[s][c] = ERROR;
        }
    }

    // after starting state
    transition_table[0]['n'] = 1;
    transition_table[0]['a'] = 4;       // nd
    transition_table[4]['n'] = 21;      // d
    transition_table[4]['d'] = 10;      // d
    transition_table[10]['d'] = ACCEPT; // accept
    transition_table[21]['d'] = ACCEPT; // accept
    transition_table[0]['m'] = 2;       // v
    transition_table[2]['v'] = ACCEPT;  // accept
    transition_table[0]['l'] = 3;
    transition_table[0]['x'] = 5; // or
    transition_table[5]['o'] = 6; // r
    //   transition_table[6]['r'] = 25; //accept
    transition_table[0]['o'] = 6;       // r
    transition_table[6]['r'] = ACCEPT;  // accept
    transition_table[0]['j'] = 7;       // ump
    transition_table[7]['u'] = 33;      // mp
    transition_table[33]['m'] = 34;     // p
    transition_table[34]['p'] = ACCEPT; // accept eller videre
    transition_table[0]['s'] = 11;      // tore
    transition_table[11]['t'] = 12;     // ore eller io
    transition_table[12]['o'] = 15;     // re
    transition_table[15]['r'] = 17;     // e
    transition_table[17]['e'] = ACCEPT; // accept
    transition_table[12]['i'] = 16;     // o

    // after n
    transition_table[1]['o'] = 8;      // t eller p
    transition_table[8]['t'] = ACCEPT; // accept
    transition_table[8]['p'] = ACCEPT; // accept
    transition_table[1]['e'] = 9;      // g
    transition_table[9]['g'] = ACCEPT; // accept
    transition_table[1]['a'] = 4;      // nd

    // after l
    transition_table[3]['i'] = ACCEPT;  // accept
    transition_table[3]['d'] = ACCEPT;      // ind eller io TIDLIGERE ->14
    transition_table[14]['i'] = 4;      // nd
    transition_table[14]['i'] = 16;     // o
    transition_table[16]['o'] = ACCEPT; // accept

    // after jump
    transition_table[35]['i'] = 18;     // mm
    transition_table[18]['m'] = 26;     // m
    transition_table[26]['m'] = ACCEPT; // accept
    transition_table[35]['n'] = 19;     // z
    transition_table[19]['z'] = ACCEPT; // accept
    transition_table[35]['z'] = ACCEPT; // accept
}

char lexeme_buffer[1024];
int lexeme_length = 0;

char *intToString(unsigned int integer, int length)
{
    char *str = (char *)malloc(length + 1); // Allocate memory for the string (+1 for null terminator)
    if (str == NULL)
    {
        // Handle memory allocation failure
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = length - 1; i >= 0; i--)
    {                                       // Start from the end of the string
        str[i] = (integer & 1) ? '1' : '0'; // Check the least significant bit
        integer >>= 1;                      // Shift the integer right by 1 bit
    }
    str[length] = '\0'; // Null-terminate the string
    return str;
}

// viktig å matche registerene med den som blir brukt i mikroinstruksjonene
void handle_statement()
{
    // if (fileNotEmpty)
    // {
    //     printf("\n");
    // }
    // else
    // {
    //     fileNotEmpty = 1;
    // }
    printf("\"");
    if (strncmp(lexeme_buffer, "nop", 3) == 0)
    {
        printf("%s", intToString(nop, 7));
        get_registers(0, 0, 0);
    }
    else if (strncmp(lexeme_buffer, "mv", 2) == 0)
    {
        printf("%s", intToString(mv, 7));
        get_registers(3, 3, 0);
    }
    else if (strncmp(lexeme_buffer, "li", 2) == 0)
    {
        printf("%s", intToString(li, 7));
        get_registers(3, 3, 16); // bruk samme srca som destin
    }
    else if (strncmp(lexeme_buffer, "ld", 2) == 0)
    {
        printf("%s", intToString(ld, 7));
        get_registers(3, 3, 16);
    }
    else if (strncmp(lexeme_buffer, "ldind", 5) == 0)
    {
        printf("%s", intToString(ldind, 7));
        get_registers(3, 3, 0);
    }
    else if (strncmp(lexeme_buffer, "ldio", 4) == 0)
    {
        printf("%s", intToString(ldio, 7));
        get_registers(3, 3, 16);
    }
    else if (strncmp(lexeme_buffer, "stio", 4) == 0)
    {
        printf("%s", intToString(stio, 7));
        get_registers(3, 3, 16);
    }
    else if (strncmp(lexeme_buffer, "neg", 3) == 0)
    {
        printf("%s", intToString(neg, 7));
        get_registers(3, 3, 0);
    }
    else if (strncmp(lexeme_buffer, "xor", 3) == 0)
    {
        printf("%s", intToString(xor, 7));
        get_registers(3, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "nand", 4) == 0)
    {
        printf("%s", intToString(nand, 7));
        get_registers(3, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "or", 2) == 0)
    {
        printf("%s", intToString(or, 7));
        get_registers(3, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "not", 3) == 0)
    {
        printf("%s", intToString(not, 7));
        get_registers(3, 3, 0);
    }
    else if (strncmp(lexeme_buffer, "jumpnz", 6) == 0)
    {
        printf("%s", intToString(jumpnz, 7));
        get_registers(0, 0, 16);
    }
    else if (strncmp(lexeme_buffer, "jumpimm", 7) == 0)
    {
        printf("%s", intToString(jumpimm, 7));
        get_registers(0, 0, 16);
    }
    else if (strncmp(lexeme_buffer, "addimm", 6) == 0)
    {
        char *str = intToString(addimm, 7);
        printf("%s", str);
        free(str);
        get_registers(3, 3, 16);
    }
    else if (strncmp(lexeme_buffer, "add", 3) == 0)
    {
        char *str = intToString(add, 7);
        printf("%s", str);
        free(str);
        get_registers(3, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "and", 3) == 0)
    {
        char *str = intToString(and, 7);
        printf("%s", str);
        free(str);
        get_registers(3, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "store", 5) == 0)
    {
        printf("%s", intToString(store, 7));
        get_registers(0, 3, 3);
    }
    else if (strncmp(lexeme_buffer, "jumpz", 5) == 0)
    {
        printf("%s", intToString(jumpz, 7));
        get_registers(0, 3, 0);
    }
    else if (strncmp(lexeme_buffer, "jump", 4) == 0)
    {
        printf("%s", intToString(jump, 7));
        get_registers(0, 3, 0);
    }
    else
    {
        printf("Unknown instruction: %s\n", lexeme_buffer);
    }
    printf("\",");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverseString(const char *str)
{
    int len = strlen(str);                    // Get the length of the string
    char *reversed = (char *)malloc(len + 1); // Allocate space for the reversed string (+1 for null-terminator)

    if (reversed == NULL) // Check for allocation failure
    {
        return NULL;
    }

    for (int i = 0; i < len; i++)
    {
        reversed[i] = str[len - 1 - i]; // Reverse the string
    }

    reversed[len] = '\0'; // Null-terminate the reversed string
    return reversed;
}

// 3 er register og 5 er immidiate
void get_registers(int destin, int srca, int srcb)
{
    int finished = 0;
    int read;
    int length = destin + srca + srcb;
    char *rbuffer = (char *)malloc((length + 1) * sizeof(char));
    if (rbuffer == NULL)
    {
        fprintf(stderr, "feil i mallocing");
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    int register_length = 0;
    while ((read = getchar()) != '\n')
    {
        if (read == EOF)
        {
            finished = 1;
            break;
        }
        if (register_length > length)
        {
            exit(EXIT_FAILURE);
        }
        rbuffer[register_length++] = read;
    }
    unsigned int rmask = 0x7;
    unsigned int imask = 0xFFFF;
    int active_destin = 0;
    int active_srca = 0;
    int imm_index = 4;
    int index_offset = 4;
    char convertToNum = '0';
    switch (destin)
    {
    case 3:
        active_destin = 1;
        printf("%s", intToString(rmask & ((unsigned int)rbuffer[1] - convertToNum), 3)); // 1 er i index 1 og srca er i index 4
        break;

    case 0:
        printf("%s", intToString(0x00, 3));
        break;
    }
 // Adding the comma after each string as you need

    switch (srca)
    {
    case 3:
        active_srca = 1;
        printf("%s", intToString(rmask & ((unsigned int)rbuffer[4] - convertToNum), 3));
        imm_index = 6;
        break;

    case 0:
        printf("%s", intToString(0x00, 3));
        break;
    }
  // Adding the comma after each string as you need

    switch (srcb)
    {
    case 16:
        if (active_destin == 1 && active_srca == 1)
        {
            imm_index = 6;
            char num[6] = {0};
            fprintf(stderr, "Info dump; imm_index == %d\n", imm_index);
            unsigned int final = 0; // Initialize final variable

            int num_index = 0; // Index for the num array

            // Extract the number from rbuffer (stop at space or null terminator)
            while (rbuffer[imm_index] != ' ' && rbuffer[imm_index] != '\0')
            {
                num[num_index++] = rbuffer[imm_index];
                fprintf(stderr, "rbuffer: %c, on index: %d\n", rbuffer[imm_index], imm_index);
                imm_index++;
            }
            num[num_index] = '\0'; // Null-terminate the string

            // Convert the extracted number string to an integer
            final = (unsigned int)atoi(num);
            char *finalstr;
            finalstr = reverseString(intToString(imask & final, 16));
            // Print the extracted number and its value
            fprintf(stderr, "Extracted number: %s\n", num);
            fprintf(stderr, "Converted number: %u\n", final);

            // Convert the number to binary and print the binary representation
            fprintf(stderr, "Binary representation of %u: ", final);
            for (int i = (sizeof(final) * 8) - 1; i >= 0; i--)
            {
                fprintf(stderr, "%u", (final >> i) & 1); // Print each bit
            }
            fprintf(stderr, "\n");
            printf("000");
            printf("\",");
            printf("\"%s", finalstr);
        }
        if (active_destin == 0 && active_srca == 0)
        {
            imm_index = 0;
            char num[6] = {0};
            fprintf(stderr, "Info dump; imm_index == %d\n", imm_index);
            unsigned int final = 0; // Initialize final variable

            int num_index = 0; // Index for the num array

            // Extract the number from rbuffer (stop at space or null terminator)
            while (rbuffer[imm_index] != ' ' && rbuffer[imm_index] != '\0')
            {
                num[num_index++] = rbuffer[imm_index];
                fprintf(stderr, "rbuffer: %c, on index: %d\n", rbuffer[imm_index], imm_index);
                imm_index++;
            }
            num[num_index] = '\0'; // Null-terminate the string

            // Convert the extracted number string to an integer
            final = (unsigned int)atoi(num);
            char *finalstr;
            // Print the extracted number and its value
            fprintf(stderr, "Extracted number: %s\n", num);
            fprintf(stderr, "Converted number: %u\n", final);
            finalstr = reverseString(intToString(imask & final, 16));
            // Convert the number to binary and print the binary representation
            fprintf(stderr, "Binary representation of %u: ", final);
            for (int i = (sizeof(final) * 8) - 1; i >= 0; i--)
            {
                fprintf(stderr, "%u", (final >> i) & 1); // Print each bit
            }
            fprintf(stderr, "\n");
            printf("\",\"%s", finalstr);
        }
        break;

    case 3:
        printf("%s", intToString(rmask & rbuffer[imm_index], 3));
        break;

    case 0:
        printf("%s", intToString(0x00, 3));
        break;
    }
// Adding the comma after each string as you need

    free(rbuffer);
    if (finished)
    {
        return;
    }
}



//First Stage printer første del av ARduino INO koden

void firstStage (){
    const char *code =
        "#include <stdlib.h>\n"
        "#include <stdio.h>\n"
        "const int SER = 26;\n"
        "const int RCLK = 22;\n"
        "const int SCLK = 30;\n"
        "const int RedLED = A10;\n"
        "const int BlueLED = A12;\n"
        "const int GreenLED = A14;\n"
        "const int SlowPin = 34;\n"
        "const int MARIN = 38;\n"
        "const int RAMIN = 42;\n"
        "#define SetRed() setColor(255, 0, 0)\n"
        "#define SetGreen() setColor(0, 255, 0)\n"
        "#define SetYellow() setColor(255, 255, 0)\n"
        "const char *data[3500] = {\n";
    printf("%s", code);
}
void lastStage(){
    const char *code =
        "};\n"
        "char* intToString(int integer, int length){\n"
        "  char* str = new char[length+1];\n"
        "  for(int i = length-1; i > -1; i--){\n"
        "    str[i] = (integer & 1) ? '1' : '0';\n"
        "    integer >>= 1;\n"
        "  }\n"
        "  str[length] = '\\0';\n"
        "  return str;\n"
        "}\n"
        "void setup()\n"
        "{\n"
        "    Serial.begin(9600);\n"
        "    pinMode(SER, OUTPUT);\n"
        "    pinMode(SCLK, OUTPUT);\n"
        "    pinMode(RCLK, OUTPUT);\n"
        "    pinMode(SlowPin, INPUT);\n"
        "    pinMode(RedLED, OUTPUT);\n"
        "    pinMode(BlueLED, OUTPUT);\n"
        "    pinMode(GreenLED, OUTPUT);\n"
        "    pinMode(MARIN, OUTPUT);\n"
        "    pinMode(RAMIN, OUTPUT);\n"
        "    digitalWrite(SER, LOW);\n"
        "    digitalWrite(SCLK, LOW);\n"
        "    digitalWrite(SCLK, LOW);\n"
        "    digitalWrite(MARIN, LOW);\n"
        "    digitalWrite(RAMIN, HIGH);\n"
        "    SetRed();\n"
        "    Serial.print(\"Done setting up\");\n"
        "    Handler(data);\n"
        "    SetGreen();\n"
        "}\n"
        "void MARINCycle(){\n"
        "  digitalWrite(MARIN, HIGH);\n"
        "  delay(10);\n"
        "  digitalWrite(MARIN, LOW);\n"
        "}\n"
        "void RAMINCycle(){\n"
        "  digitalWrite(RAMIN, LOW);\n"
        "  delay(10);\n"
        "  digitalWrite(RAMIN, HIGH);\n"
        "}\n"
        "void setColor(int redValue, int greenValue, int blueValue) {\n"
        "  analogWrite(RedLED, redValue);\n"
        "  analogWrite(GreenLED, greenValue);\n"
        "  analogWrite(BlueLED, blueValue);\n"
        "}\n"
        "void SR_writeAddress(char *str)\n"
        "{\n"
        "    word strToWord = 0;\n"
        "    for (int i = 0; i < 16; i++)\n"
        "    {\n"
        "        strToWord = (strToWord << 1) | (str[i] - '0');\n"
        "    }\n"
        "    uint8_t counter;\n"
        "    word shiftVal;\n"
        "    for (counter = 0; counter < 16; counter++)\n"
        "    {\n"
        "        shiftVal = (strToWord & 0x01);\n"
        "        digitalWrite(SER, shiftVal);\n"
        "        digitalWrite(SCLK, HIGH);\n"
        "        delay(10);\n"
        "        digitalWrite(SCLK, LOW);\n"
        "        delay(10);\n"
        "        strToWord = strToWord >> 1;\n"
        "    }\n"
        "    digitalWrite(RCLK, HIGH);\n"
        "    delay(10);\n"
        "    digitalWrite(RCLK, LOW);\n"
        "    Serial.println(\"Shift register latched\");\n"
        "    SetYellow();\n"
        "    while(digitalRead(SlowPin) == 0){\n"
        "      delay(100);\n"
        "    }\n"
        "    SetRed();\n"
        "}\n"

        "void SR_writeWord(char *str)\n"
        "{\n"
        "    word strToWord = 0;\n"
        "    for (int i = 0; i < 16; i++)\n"
        "    {\n"
        "        strToWord = (strToWord << 1) | (str[i] - '0');  // Build the 16-bit word, MSB-first\n"
        "    }\n"
        "\n"
        "    uint8_t counter;\n"
        "    for (counter = 0; counter < 16; counter++)\n"
        "    {\n"
        "        // Shift the MSB first\n"
        "        uint8_t shiftVal = (strToWord >> (15 - counter)) & 0x01;\n"
        "        digitalWrite(SER, shiftVal);\n"
        "        digitalWrite(SCLK, HIGH);\n"
        "        delay(10);  // Adjust this delay if needed\n"
        "        digitalWrite(SCLK, LOW);\n"
        "        delay(10);\n"
        "    }\n"
        "\n"
        "    digitalWrite(RCLK, HIGH);  // Latch the shift register\n"
        "    delay(10);\n"
        "    digitalWrite(RCLK, LOW);\n"
        "\n"
        "    Serial.println(\"Shift register latched\");\n"
        "    SetYellow();\n"
        "\n"
        "    // Wait for SlowPin signal to proceed\n"
        "    while (digitalRead(SlowPin) == 0)\n"
        "    {\n"
        "        delay(100);\n"
        "    }\n"
        "\n"
        "    SetRed();\n"
        "}\n"

        "void Handler(char **data){\n"
        "  int addresseCounter = 0;\n"
        "  for (int i = 0; i < 3500; i++){\n"
        "    Serial.print(addresseCounter);\n"
        "    char* addresseCounterStr = intToString(addresseCounter, 16);\n"
        "    SR_writeAddress(addresseCounterStr);\n"
        "    Serial.print(\"Adresse dataen blir puttet i: \");\n"
        "    Serial.println(addresseCounter, HEX);\n"
        "    MARINCycle();\n"
        "    addresseCounter++;\n"
        "    Serial.print(\"Data: \");\n"
        "    Serial.println(data[i]);\n"
        "    SR_writeWord(data[i]);\n"
        "    RAMINCycle();\n"
        "  }\n"
        "}\n"
        "void loop()\n"
        "{}\n";
    printf("%s",code);

}

int main()
{
    //First Stage er all Arduino INO koden før Arrayet med data

    firstStage();
    // init
    initialize_table();

    // main
    int line_num = 1;
    int read;
    int prev_state;
    while ((read = getchar()) != EOF)
    {
        lexeme_buffer[lexeme_length++] = read;
        lexeme_buffer[lexeme_length] = 0;
        prev_state = state;
        state = transition_table[state][read];

        switch (state)
        {
        case ACCEPT:
            /* code */
            read = getchar();
            if (read != ' ' && read != EOF)
            {
                if(prev_state == 3){
                    lexeme_buffer[lexeme_length++] = read;
                    lexeme_buffer[lexeme_length] = 0;
                    state = transition_table[14][read];
                    break;
                }
                lexeme_buffer[lexeme_length++] = read;
                lexeme_buffer[lexeme_length] = 0;
                state = transition_table[35][read];
                break;
            }
            else
            {
                handle_statement();
                state = START_STATE;
                lexeme_length = 0;
                break;
            }

        case ERROR:
            fprintf(stderr, "error: %d: unrecognized statement: %s, state: %d, read: %d, prev_state: %d /n", line_num, lexeme_buffer, state, read, prev_state);
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    if (state != START_STATE)
    {
        fprintf(stderr, "error: %d: input ended in the middle of a statement: %s\n", line_num, lexeme_buffer);
        exit(EXIT_FAILURE);
    }
    //Last stage er Arduino Koden etter arayet med maskin instruksjoner
    lastStage();
}
