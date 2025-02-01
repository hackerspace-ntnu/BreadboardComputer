#ifndef VIRTUAL_INSTRUCTION_H
#define VIRTUAL_INSTRUCTION_H

#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "token.h"
#include <string.h>

typedef struct {
    char* op_code;
    char* parameters;
    InstructionFormat* instructions;
    int instructions_count;
} VirtualInstruction;

typedef struct {
    int instructions_count;
    int* instruction_lengths;
} VirtualInstructionLengths;

typedef struct {
    VirtualInstruction* virtual_instructions;
    int instructions_count;
} VirtualInstructionConfig;

typedef struct {
    char param;
    Token token;
} VirtualInstructionParameter;

int is_virtual_instruction(VirtualInstructionConfig* virtual_instruction_config, char*instruction);
VirtualInstructionConfig* get_virtual_instructions(char* file_path);
TokenArray* resolve_virtual_instruction(TokenArray* tokens, VirtualInstructionConfig* virtual_instruction_config, int offset);

#endif