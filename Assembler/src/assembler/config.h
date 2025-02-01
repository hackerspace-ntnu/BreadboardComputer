#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
    char *name;
    char *binary_representation;
} Mnemonic;

typedef struct
{
    char *name;
    char *binary_representation;
} Register;

typedef struct
{
    char *name;
    char *format;
} InstructionFormat;

typedef struct
{
    char *name;
    char *binary_representation;
} Generic;

typedef struct
{
    Mnemonic *mnemonics;
    int mnemonics_count;
    Register *registers;
    int registers_count;
    InstructionFormat *instruction_formats;
    int instruction_formats_count;
} Config;

Config *read_asm_config(char *mnemonic_path, char *register_path, char *instruction_format_path);

#endif