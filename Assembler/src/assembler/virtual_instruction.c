#include "virtual_instruction.h"

VirtualInstructionLengths *_count_virtual_instructions(char *path)
{
    FILE *file = fopen(path, "r");
    int number_of_virtual_instructions = 0;
    int current_virtual_instruction_length = 0;
    int skip_newline = 0;

    VirtualInstructionLengths *virtual_instruction_lengths = malloc(sizeof(VirtualInstructionLengths));
    virtual_instruction_lengths->instruction_lengths = (int *)malloc(sizeof(int));
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        switch (ch)
        {
        case '\n':
            if (!skip_newline)
            {
                current_virtual_instruction_length++;
                continue;
            }
            break;
        case '{':
            number_of_virtual_instructions++;
            skip_newline = 0;
            continue;
        case '}':
            if (!number_of_virtual_instructions)
            {
                fprintf(stderr, "Syntax error: '}' without '{' in virtual instruction configuration file\n");
                continue;
            }
            virtual_instruction_lengths->instruction_lengths = realloc(virtual_instruction_lengths->instruction_lengths, number_of_virtual_instructions * sizeof(int));
            virtual_instruction_lengths->instruction_lengths[number_of_virtual_instructions - 1] = current_virtual_instruction_length - 1;
            current_virtual_instruction_length = 0;
            skip_newline = 1;
            continue;
        default:
            break;
        }
    }

    virtual_instruction_lengths->instructions_count = number_of_virtual_instructions;

    return virtual_instruction_lengths;
}

VirtualInstruction *_read_virtual_instructions(char *path, VirtualInstructionLengths *virtual_instruction_lengths)
{
    VirtualInstruction *virtual_instructions = malloc(virtual_instruction_lengths->instructions_count * sizeof(VirtualInstruction));

    FILE *file = fopen(path, "r");

    for (int i = 0; i < virtual_instruction_lengths->instructions_count; i++)
    {
        char *op_code = (char *)malloc(100 * sizeof(char));
        char *parameters = (char *)malloc(100 * sizeof(char));

        if (i > 0)
        {
            fscanf(file, "}\n");
        }

        fscanf(file, "%s", op_code);
        fscanf(file, "->");
        fscanf(file, " ");
        fscanf(file, "->%s", parameters);
        fscanf(file, " ");
        fscanf(file, "->");
        fscanf(file, " ");
        fscanf(file, "{\n");

        VirtualInstruction *virtual_instruction = (VirtualInstruction *)malloc(sizeof(VirtualInstruction));
        virtual_instruction->op_code = op_code;
        virtual_instruction->parameters = parameters;
        virtual_instruction->instructions_count = virtual_instruction_lengths->instruction_lengths[i];
        virtual_instruction->instructions = (InstructionFormat *)malloc(virtual_instruction_lengths->instruction_lengths[i] * sizeof(InstructionFormat));

        for (int j = 0; j < virtual_instruction_lengths->instruction_lengths[i]; j++)
        {
            char *op_code = (char *)malloc(100 * sizeof(char));
            char *parameters = (char *)malloc(100 * sizeof(char));

            fscanf(file, "%s", op_code);
            fscanf(file, " ");
            fscanf(file, "%s\n", parameters);

            InstructionFormat *instruction = (InstructionFormat *)malloc(sizeof(InstructionFormat));
            instruction->name = op_code;
            instruction->format = parameters;
            virtual_instruction->instructions[j] = *instruction;
        }

        virtual_instructions[i] = *virtual_instruction;
    }

    return virtual_instructions;
}

VirtualInstructionConfig *get_virtual_instructions(char *file_path)
{
    VirtualInstructionLengths *virtual_instruction_lengths = _count_virtual_instructions(file_path);

    VirtualInstructionConfig *virtual_instruction_config = malloc(sizeof(VirtualInstructionConfig));
    VirtualInstruction *virtual_instructions = _read_virtual_instructions(file_path, virtual_instruction_lengths);
    virtual_instruction_config->virtual_instructions = virtual_instructions;
    virtual_instruction_config->instructions_count = virtual_instruction_lengths->instructions_count;

    return virtual_instruction_config;
}

int is_virtual_instruction(VirtualInstructionConfig *virtual_instruction_config, char *instruction)
{
    // return number of tokens required for the instruction, or 0 if it is not a virtual instruction
    for (int i = 0; i < virtual_instruction_config->instructions_count; i++)
    {
        if (strcmp(instruction, virtual_instruction_config->virtual_instructions[i].op_code) == 0)
        {
            int token_count = 0;
            for (int j = 0; j < virtual_instruction_config->virtual_instructions[i].instructions_count; j++)
            {
                token_count += strlen(virtual_instruction_config->virtual_instructions[i].instructions[j].format) + 1; // +1 for the instruction itself
            }
            return token_count;
        }
    }
    return 0;
}

VirtualInstructionParameter _get_virtual_instruction_parameter(VirtualInstructionParameter *parameters, int number_of_parameters, char parameter)
{
    for (int i = 0; i < number_of_parameters; i++)
    {
        if (parameters[i].param == parameter)
        {
            return parameters[i];
        }
    }
    fprintf(stderr, "Error: Invalid virtual instruction parameter (error in virtual instruction config)\n");
    exit(1);
}

TokenArray *resolve_virtual_instruction(TokenArray *tokens, VirtualInstructionConfig *virtual_instruction_config, int offset)
{
    int tokens_required = is_virtual_instruction(virtual_instruction_config, tokens->tokens[offset].lexemes);

    TokenArray *translated_tokens = (TokenArray *)malloc(sizeof(TokenArray));

    translated_tokens->tokens = (Token *)malloc(tokens_required * sizeof(Token));
    translated_tokens->size = tokens_required;

    // get virtual instruction
    VirtualInstruction *virtual_instruction = NULL;
    for (int i = 0; i < virtual_instruction_config->instructions_count; i++)
    {
        if (strcmp(tokens->tokens[offset].lexemes, virtual_instruction_config->virtual_instructions[i].op_code) == 0)
        {
            virtual_instruction = &virtual_instruction_config->virtual_instructions[i];
            break;
        }
    }

    // get parameters of the virtual instruction
    int number_of_parameters = strlen(virtual_instruction->parameters);
    VirtualInstructionParameter *parameters = (VirtualInstructionParameter *)malloc(number_of_parameters * sizeof(VirtualInstructionParameter));

    for (int i = 0; i < number_of_parameters; i++)
    {
        Token token = tokens->tokens[offset + i + 1]; // skip the virtual instruction itself (could segfault if the user doesn't provide enough parameters)
        VirtualInstructionParameter *parameter = (VirtualInstructionParameter *)malloc(sizeof(VirtualInstructionParameter));
        parameter->param = virtual_instruction->parameters[i];
        parameter->token = token;
        parameters[i] = *parameter;
    }

    // translate the virtual instruction
    int current_token = 0;
    for (int i = 0; i < virtual_instruction->instructions_count; i++)
    {
        InstructionFormat instruction = virtual_instruction->instructions[i];
        Token *token = (Token *)malloc(sizeof(Token));
        memcpy(token, &tokens->tokens[offset], sizeof(Token)); // copy the virtual instruction itself but change the op code (to keep the line number)
        token->lexemes = instruction.name;

        translated_tokens->tokens[current_token++] = *token;

        for (int j = 0; j < strlen(instruction.format); j++)
        {
            VirtualInstructionParameter parameter = _get_virtual_instruction_parameter(parameters, number_of_parameters, instruction.format[j]);
            Token *temp_token = (Token *)malloc(sizeof(Token));
            memcpy(temp_token, &parameter.token, sizeof(Token));
            if (j == strlen(instruction.format) - 1)
            {
                temp_token->is_last = 1;
            }
            else
            {
                temp_token->is_last = 0;
            }
            translated_tokens->tokens[current_token++] = *temp_token;
        }
    }

    return translated_tokens;
}