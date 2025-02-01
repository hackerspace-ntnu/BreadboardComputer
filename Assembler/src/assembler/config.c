#include "config.h"

#include <stdlib.h>
#include <stdio.h>

int _count_lines(char *path)
{
    FILE *file = fopen(path, "r");
    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    return count + 1;
}

Generic *_read_config(char *path, int number_of_lines)
{
    Generic *generics = malloc(number_of_lines * sizeof(Generic));
    FILE *file = fopen(path, "r");

    for (int i = 0; i < number_of_lines; i++)
    {
        char *name = (char *)malloc(100 * sizeof(char));
        char *binary = (char *)malloc(100 * sizeof(char));

        fscanf(file, "%s", name);
        fscanf(file, "->");
        fscanf(file, " ");
        fscanf(file, "->%s\n", binary);

        Generic *generic = (Generic *)malloc(sizeof(Generic));
        generic->name = name;
        generic->binary_representation = binary;
        generics[i] = *generic;
    }

    return generics;
}

Config *read_asm_config(char *mnemonic_path, char *register_path, char *instruction_format_path)
{
    Config *config = (Config *)malloc(sizeof(Config));

    // read config file
    int mnemonics_count = _count_lines(mnemonic_path);
    int registers_count = _count_lines(register_path);
    int instruction_formats_count = _count_lines(instruction_format_path);

    config->mnemonics = (Mnemonic *)_read_config(mnemonic_path, mnemonics_count);
    config->mnemonics_count = mnemonics_count;

    config->registers = (Register *)_read_config(register_path, registers_count);
    config->registers_count = registers_count;

    config->instruction_formats = (InstructionFormat *)_read_config(instruction_format_path, instruction_formats_count);
    config->instruction_formats_count = instruction_formats_count;

    return config;
}