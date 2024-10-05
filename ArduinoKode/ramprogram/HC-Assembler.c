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
    transition_table[15]['d'] = 17;     // e
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
    transition_table[3]['d'] = 14;      // ind eller io
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
            printf("\"%s", intToString(imask & final, 16));
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
            printf("\",\"%s", intToString(imask & final, 16));
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

        "const int addressBus[15] = {50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};\n"

        "const int dataBus[16] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23};\n"

        "const int lightPin = 8;\n"
        "const int wePin = 52;\n"

        "int standardDelay = 200;\n"

        "#define antallInstruksjoner 21\n"

        "const unsigned int nop = 0x00;\n"
        "const unsigned int mv = 0x01;\n"
        "const unsigned int li = 0x02;\n"
        "const unsigned int ld = 0x03;\n"
        "const unsigned int ldind = 0x04;\n"
        "const unsigned int ldio = 0x05;\n"
        "const unsigned int stio = 0x06;\n"
        "const unsigned int add = 0x07;\n"
        "const unsigned int neg = 0x09;\n"
        "const unsigned int xorInst = 0x0A;\n"
        "const unsigned int nand = 0x0B;\n"
        "const unsigned int andInst = 0x0C;\n"
        "const unsigned int orInst = 0x0D;\n"
        "const unsigned int notInst = 0x0E;\n"
        "const unsigned int jump = 0x0F;\n"
        "const unsigned int jumpnz = 0x10;\n"
        "const unsigned int jumpimm = 0x11;\n"
        "const unsigned int addimm = 0x12;\n"
        "const unsigned int store = 0x13;\n"
        "const unsigned int jumpz = 0x14;\n"

        "const unsigned int r0 = 0x0;\n"
        "const unsigned int r1 = 0x1;\n"
        "const unsigned int r2 = 0x2;\n"
        "const unsigned int r3 = 0x3;\n"
        "const unsigned int r4 = 0x4;\n"
        "const unsigned int r5 = 0x5;\n"
        "const unsigned int r6 = 0x6;\n"
        "const unsigned int r7 = 0x7;\n"
        "const char* data[1000] = {";
    printf("%s", code);
}
void lastStage(){
    const char *code =
    "};\n"
    "const unsigned int instruksjoner[antallInstruksjoner] = {nop, mv, li, ld, ldind, ldio, stio, add, sub, neg, xorInst, nand, andInst, orInst, notInst, jump, jumpnz, jumpimm, addimm, store, jumpz};\n\n"

    // Change to 1 to use the assembler, this will compile a assembly code text file,\n"
    // endre!!!!!\n"
    "const int localUpload = 0;\n\n"

    "void setup()\n"
    "{\n"
    "    Serial.begin(9600);\n"
    "    pinMode(wePin, OUTPUT);\n"
    "    pinMode(lightPin, OUTPUT);\n\n"

    "    digitalWrite(lightPin, HIGH);\n"
    "    digitalWrite(wePin, HIGH);\n\n"

    "    for (int i = 0; i < 15; i++)\n"
    "    {\n"
    "        pinMode(addressBus[i], OUTPUT);\n"
    "        digitalWrite(addressBus[i], LOW);\n"
    "    }\n"
    "    for (int i = 0; i < 16; i++)\n"
    "    {\n"
    "        pinMode(dataBus[i], OUTPUT);\n"
    "        digitalWrite(dataBus[i], LOW);\n"
    "    }\n\n"

    "    Serial.print(\"Done setting up\");\n"
    "}\n\n"

    "// Returnerer String med minst signifcant bit til venstre aka motsatt av vanlig.\n"
    "char *intToString(int integer, int length)\n"
    "{\n"
    "    char *str = new char[length + 1];\n"
    "    for (int i = 0; i < length; i++)\n"
    "    {\n"
    "        str[i] = (integer & 1) ? '1' : '0';\n"
    "        integer >>= 1;\n"
    "    }\n"
    "    str[length] = '\\0';\n"
    "    return str;\n"
    "}\n\n"

    "// Skriver data til adresse\n"
    "void write(int address, int data)\n"
    "{\n"
    "    char *bitAddr = intToString(address, 15);\n"
    "    char *bitData = intToString(data, 16);\n"
    "    for (int i = 0; i < 15; i++)\n"
    "    {\n"
    "        digitalWrite(addressBus[i], bitAddr[i] == '1' ? HIGH : LOW);\n"
    "    }\n"
    "    for (int i = 0; i < 16; i++)\n"
    "    {\n"
    "        digitalWrite(dataBus[i], bitData[i] == '1' ? HIGH : LOW);\n"
    "    }\n"
    "    // Lagrer informasjonen\n"
    "    digitalWrite(wePin, LOW);\n"
    "    delay(50);\n"
    "    digitalWrite(wePin, HIGH);\n\n"

    "    delete[] bitAddr;\n"
    "    delete[] bitData;\n"
    "    delay(standardDelay);\n"
    "}\n\n"

    "void assembler()\n"
    "{\n"
    "}\n\n"

    "void loop()\n"
    "{\n"
    "    if (localUpload == 0)\n"
    "    {\n"
    "        digitalWrite(lightPin, LOW);\n"
    "        standardDelay = 30;\n"
    "        for (int i = 0; i < 32768; i++)\n"
    "        {\n"
    "            write(i, data[i]);\n"
    "        }\n"
    "        digitalWrite(lightPin, HIGH);\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "    }\n"
    "}\n";
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
    while ((read = getchar()) != EOF)
    {
        lexeme_buffer[lexeme_length++] = read;
        lexeme_buffer[lexeme_length] = 0;
        state = transition_table[state][read];

        switch (state)
        {
        case ACCEPT:
            /* code */
            read = getchar();
            if (read != ' ' && read != EOF)
            {
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
            fprintf(stderr, "error: %d: unrecognized statement: %s, state: %d /n", line_num, lexeme_buffer, state);
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
