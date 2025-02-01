#include "parser.h"

int _set_token_addresses(TokenArray *tokens)
{
    // instructions take one address
    // immediates take one address
    // labels take one address (basically an immediate)

    int current_address = -1;

    for (int i = 0; i < tokens->size; i++)
    {
        Token *token = &tokens->tokens[i];
        switch (token->ttype)
        {
        case INST:
            token->address = ++current_address;
            break;
        case REG:
            token->address = current_address;
            break;
        case IMM:
            token->address = ++current_address;
            break;
        case DEC_LABEL:
            token->address = current_address + 1; // point to next instruction
            break;
        case LABEL:
            token->address = ++current_address;
            break;
        }
    }
    return current_address;
}

int _get_label_address(TokenArray *declared_labels, char *label)
{
    // check if label is valid
    for (int i = 0; i < declared_labels->size; i++)
    {
        if (!strncmp(label, declared_labels->tokens[i].lexemes, strlen(label)))
        {
            return declared_labels->tokens[i].address;
        }
    }

    return -1;
}

void _resolve_labels(TokenArray *tokens)
{
    // gather all labels
    int number_of_labels = 0;
    for (int i = 0; i < tokens->size; i++)
    {
        Token token = tokens->tokens[i];
        if (token.ttype == DEC_LABEL)
            number_of_labels++;
    }

    int current_declared = 0;
    TokenArray *declared_labels = (TokenArray *)malloc(sizeof(TokenArray));
    declared_labels->tokens = (Token *)malloc(number_of_labels * sizeof(Token));
    declared_labels->size = number_of_labels;
    for (int i = 0; i < tokens->size; i++)
    {
        Token token = tokens->tokens[i];
        if (token.ttype == DEC_LABEL)
            declared_labels->tokens[current_declared++] = token;
    }

    // convert labels to immediates
    for (int i = 0; i < tokens->size; i++)
    {
        Token *token = &tokens->tokens[i];
        if (token->ttype != LABEL)
            continue;

        int address = _get_label_address(declared_labels, token->lexemes);
        if (address == -1)
        {
            fprintf(stderr, "Error: Invalid token %s at line %d\n", token->lexemes, token->line_number);
            exit(1);
        }

        token->ttype = IMM;
        token->lexemes = (char *)malloc(100 * sizeof(char));
        token->size = sprintf(token->lexemes, "%d", address);
    }
}

char *_get_pattern(Token *token, Config *asm_config)
{
    if (token->ttype == INST)
    {
        for (int i = 0; i < asm_config->instruction_formats_count; i++)
        {
            if (!strcmp(token->lexemes, asm_config->instruction_formats[i].name))
            {
                return asm_config->instruction_formats[i].format;
            }
        }
    }
    return NULL;
}

void _validate_tokens(TokenArray *tokenarr, Config *asm_config)
{
    // get next pattern
    char *current_pattern = NULL;
    int current_pattern_index = -1;
    int current_pattern_size = -1;

    Token *last_registry = NULL;

    for (int i = 0; i < tokenarr->size; i++)
    {
        Token *current_token = &tokenarr->tokens[i];
        current_token->is_last = 0;
        if (current_pattern_index == current_pattern_size && current_pattern != NULL)
        {
            if (last_registry != NULL)
            {

                last_registry->is_last = 1;
            }
            current_pattern = NULL;
        }

        if (current_token->ttype == REG)
        {
            last_registry = current_token;
        }

        if (current_pattern == NULL)
        {
            char *token_pattern = _get_pattern(current_token, asm_config);
            if (token_pattern == NULL)
            {
                if (current_token->ttype != DEC_LABEL)
                {
                    fprintf(stderr, "Error: Unexpected token %s at line %d\n", current_token->lexemes, current_token->line_number);
                    exit(1);
                }
                continue;
            }
            if (token_pattern[0] != 'N')
            {
                // we reached an instruction
                current_pattern = token_pattern;
                current_pattern_index = 0;
                current_pattern_size = strlen(current_pattern);
                continue;
            }
            continue;
        }

        TType expected_type = current_pattern[current_pattern_index] == 'I' ? IMM : REG;

        if (current_token->ttype != expected_type)
        {
            fprintf(stderr, "Error: Unexpected token %s at line %d (expected token of type %s)\n", current_token->lexemes, current_token->line_number, expected_type == REG ? "registry" : "immediate");
            exit(1);
        }

        current_token->registry_type = current_pattern[current_pattern_index];

        current_pattern_index++;
    }

    // account for last instruction
    if (last_registry != NULL)
    {
        last_registry->is_last = 1;
    }
}

TokenArray *_resolve_virtual_instructions(TokenArray *tokens, VirtualInstructionConfig *virtual_instruction_config)
{
    int tokens_required_after_virtual_instruction = 0;
    int skip_until_next_instruction = 0;
    for (int i = 0; i < tokens->size; i++)
    {

        if (skip_until_next_instruction)
        {
            if (tokens->tokens[i].ttype == INST || tokens->tokens[i].ttype == DEC_LABEL)
            {

                skip_until_next_instruction = 0;
            }
            else
            {
                continue;
            }
        }
        int tokens_required = is_virtual_instruction(virtual_instruction_config, tokens->tokens[i].lexemes);
        if (!tokens_required)
        {
            tokens_required_after_virtual_instruction++;
            continue;
        }
        tokens_required_after_virtual_instruction += tokens_required;
        skip_until_next_instruction = 1; // we dont want to count the parameters of the virtual instruction
    }

    Token *new_tokens = (Token *)malloc(tokens_required_after_virtual_instruction * sizeof(Token));

    printf("[DEBUG] Total token count: %d\n", tokens_required_after_virtual_instruction);

    int current_token = 0;
    skip_until_next_instruction = 0;
    for (int i = 0; i < tokens->size; i++)
    {
        if (skip_until_next_instruction)
        {

            if (tokens->tokens[i].ttype == INST || tokens->tokens[i].ttype == DEC_LABEL)
            {
                skip_until_next_instruction = 0;
            }
            else
            {
                continue;
            }
        }
        int tokens_required = is_virtual_instruction(virtual_instruction_config, tokens->tokens[i].lexemes);
        if (!tokens_required)
        {
            new_tokens[current_token++] = tokens->tokens[i];
            continue;
        }

        TokenArray *translated_tokens = resolve_virtual_instruction(tokens, virtual_instruction_config, i);
        for (int j = 0; j < translated_tokens->size; j++)
        {
            new_tokens[current_token++] = translated_tokens->tokens[j];
            skip_until_next_instruction = 1;
        }
    }

    TokenArray *new_tokens_array = (TokenArray *)malloc(sizeof(TokenArray));
    new_tokens_array->tokens = new_tokens;
    new_tokens_array->size = tokens_required_after_virtual_instruction;

    return new_tokens_array;
}

TokenArray *parse_tokens(TokenArray *tokens, Config *asm_config, VirtualInstructionConfig *virtual_instruction_config)
{
    // resolve virtual instructions
    tokens = _resolve_virtual_instructions(tokens, virtual_instruction_config);

    // find address of all tokens
    int last_address = _set_token_addresses(tokens);
    printf("[DEBUG] Last instruction address: %d (decimal)\n", last_address);

    // replace all labels with addresses
    _resolve_labels(tokens);

    printf("[DEBUG] Resolved labels\n");

    // validate tokens (raises error if something is wrong)
    _validate_tokens(tokens, asm_config);

    printf("[DEBUG] Validated tokens\n");

    return tokens;
}
