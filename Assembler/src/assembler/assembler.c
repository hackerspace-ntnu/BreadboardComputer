#include <stdio.h>
#include <time.h>
#include "assembler.h"

void assemble(const char *dest_path, const char *flag)
{
    // flag is either -b for binary or -h for hex
    time_t start = time(NULL);

    // pipeline
    // read_asm_config
    char *mnemonic_path = "./src/assembler/asm_config/mnemonics";
    char *register_path = "./src/assembler/asm_config/registers";
    char *instruction_format_path = "./src/assembler/asm_config/instruction_format";
    char *virtual_instruction_path = "./src/assembler/asm_config/virtual_instructions";

    Config *config = read_asm_config(mnemonic_path, register_path, instruction_format_path);
    VirtualInstructionConfig *virtual_instruction_config = get_virtual_instructions(virtual_instruction_path);

    // read_asm -> lex_asm (generate and confirm tokens)
    TokenArray *tokens = lex(config);
    // -> parse_tokens (validate structure of tokens and resolve labels and virtual instructions)
    tokens = parse_tokens(tokens, config, virtual_instruction_config);
    // -> translate_to_bin (generate machine code from set of tokens)
    // -> write to file
    translate(tokens, config, dest_path, flag[1]);

    printf("[DEBUG] Assembled in %lld seconds\n", time(NULL) - start);
}

int main(int argc, char const *argv[])
{
    assemble(argv[1], argv[2]);
    return 0;
}
