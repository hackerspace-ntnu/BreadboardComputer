#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "virtual_instruction.h"

TokenArray *parse_tokens(TokenArray *tokens, Config *asm_config, VirtualInstructionConfig *virtual_instruction_config);

#endif