#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "token.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD_SIZE 16

void translate(TokenArray *tokens, Config *asm_config, const char *output_file_path, const char flag);

#endif