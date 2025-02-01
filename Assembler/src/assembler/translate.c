#include "translate.h"

void _reverse_string(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void _allocate_and_initialize(char **field, size_t size)
{
    *field = (char *)malloc((size + 1) * sizeof(char));
    memset(*field, '0', size);
    (*field)[size] = '\0';
}

void _clear_instruction(Instruction *instruction)
{
    _allocate_and_initialize(&instruction->immediate, 16);
    _allocate_and_initialize(&instruction->dest, 3);
    _allocate_and_initialize(&instruction->srca, 3);
    _allocate_and_initialize(&instruction->srcb, 3);
    _allocate_and_initialize(&instruction->opcode, 7);
}

void _send_word(Instruction *instruction, char flag, FILE *file, int send_immediate)
{

    char *data = (char *)malloc((MAX_WORD_SIZE + 1) * sizeof(char));

    if (send_immediate)
    {
        strcpy(data, instruction->immediate);
        _reverse_string(data);
    }
    else
    {
        // print all the variables
        _reverse_string(instruction->dest);
        _reverse_string(instruction->srca);
        _reverse_string(instruction->srcb);
        strcpy(data, instruction->dest);
        strcat(data, instruction->srca);
        strcat(data, instruction->srcb);
        strcat(data, instruction->opcode);
    }

    (flag == 'b') ? fwrite(data, 1, MAX_WORD_SIZE, file) : fprintf(file, "%X", (int)strtol(data, NULL, 2));
    fwrite("\n", 1, 1, file);
    _clear_instruction(instruction);
}

void _int_to_binary_string(int value, char *output, int length)
{
    for (int i = length - 1; i >= 0; i--)
    {
        output[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    output[length] = '\0'; // Null-terminate the string
}

char *_registry_to_binary(char *registry, Config *asm_config)
{
    for (int i = 0; i < asm_config->registers_count; i++)
    {
        if (strcmp(registry, asm_config->registers[i].name) == 0)
        {
            return asm_config->registers[i].binary_representation;
        }
    }
    return NULL;
}

char *_instruction_to_binary(char *instruction, Config *asm_config)
{
    for (int i = 0; i < asm_config->mnemonics_count; i++)
    {
        if (strcmp(instruction, asm_config->mnemonics[i].name) == 0)
        {
            return asm_config->mnemonics[i].binary_representation;
        }
    }
    return NULL;
}

char *_immediate_to_binary(char *immediate)
{
    int is_hex = 0;
    if (immediate[0] == '0' && (immediate[1] == 'x' || immediate[1] == 'X'))
    {
        is_hex = 1;
    }

    int value = is_hex ? strtol(immediate, NULL, MAX_WORD_SIZE) : atoi(immediate);

    char *binary_immediate = (char *)malloc((MAX_WORD_SIZE + 1) * sizeof(char));
    _int_to_binary_string(value, binary_immediate, MAX_WORD_SIZE);
    return binary_immediate;
}

void _translate(TokenArray *tokenarr, Config *asm_config, FILE *file, char flag)
{
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
    _clear_instruction(instruction);

    for (int i = 0; i < tokenarr->size; i++)
    {
        Token token = tokenarr->tokens[i];

        switch (token.ttype)
        {
        case REG:
            char *binary_data = _registry_to_binary(token.lexemes, asm_config);
            switch (token.registry_type)
            {
            case 'D':
                strcpy(instruction->dest, binary_data);
                break;
            case 'A':
                strcpy(instruction->srca, binary_data);
                break;
            case 'B':
                strcpy(instruction->srcb, binary_data);
                break;
            case 'C':
                strcpy(instruction->srcc, binary_data);
                break;
            }
            if (token.is_last)
            {
                _send_word(instruction, flag, file, 0);
            }
            break;
        case INST:
            // nop is a special case
            if (strcmp(token.lexemes, "nop") == 0)
            {
                _clear_instruction(instruction);
                _send_word(instruction, flag, file, 0);
                break;
            }

            char *binary_opcode = _instruction_to_binary(token.lexemes, asm_config);
            memcpy(instruction->opcode, binary_opcode, strlen(binary_opcode));

            // jimm is also a special case
            if (strcmp(token.lexemes, "jimm") == 0)
            {
                _send_word(instruction, flag, file, 0);
                break;
            }

            break;
        case IMM:
            char *binary_immediate = _immediate_to_binary(token.lexemes);
            strcpy(instruction->immediate, binary_immediate);

            _send_word(instruction, flag, file, 1);
            break;
        default:
            break;
        }
    }
}

void translate(TokenArray *tokens, Config *asm_config, const char *output_file_path, const char flag)
{
    FILE *file = fopen(output_file_path, (flag == 'b') ? "w" : "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", output_file_path);
        exit(1);
    }
    if (!(flag == 'b' || flag == 'h'))
    {
        fprintf(stderr, "Error, must include flag -b for binary or -h for hex\n");
        exit(1);
    }
    if (flag == 'h')
    {
        printf("[DEBUG] Writing to hex file\n");
    }
    else if (flag == 'b')
    {
        printf("[DEBUG] Writing to binary file\n");
    }
    _translate(tokens, asm_config, file, flag);
}
