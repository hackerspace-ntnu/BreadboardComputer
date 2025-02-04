jimm main
# memory space 0-16kB
# ram starts at 16kB-32kB = 0x1FFF
# vram starts at 32kB-64kB = 0x3FFF

# function to write a letter to the display
# 4x4 pixel font, 60 letters
# this allows for 32 letter per row, 24 rows
# each call takes 35 instructions
write_letter:
    # arguments: r0 = return address, r1 = letter code (0-59), r2 = position offset to top left of letter (y*160 + x), r3 = color
    li r4, 0x8000 # start of vram
    add r4, r4, r2 # where to write the letter in ram

    # algorithm to map letter code to correct code
    # 0-25: A-Z
    # 26-35: 0-9
    # 36-58: !-^%*[]?"+/.,=:;<>_\'|* (23 special characters)
    # 59: space

    # hash map for letters
    # we need to multiply the letter code by 18 to get the correct offset in rom
    shiftl r6, r1, 4 # r6 = r1 * 16
    shiftl r7, r1, 1 # r7 = r1 * 2
    add r6, r6, r7 # r6 = r1 * 18
    add r6, r6, r1 # r6 = r1 * 18 + r1 = r1 * 19
    mv r1, r6 # r1 = r1 * 19

    li r6, 0xFF # bitmask to remove first pixel of position
    li r7, 0xFF00 # bitmask to remove second pixel of position

    # jump to correct letter
    add r1, r1, LETTERS
    j r1

LETTERS:
    # each letter takes a maximum of 19 words (38 bytes) in rom
    # this means that all the letters take up 1140 words (2280 bytes = 2.2265625kB) in rom
    # or roughly 14% of the rom space
    # this is a lot of space, but it is necessary to have a font that is easy to read
    write_A:
        # 19 instructions (10 for the operations, 8 for the immediate values, 1 for the jump)
        stio r4, r3, 0
        stio r4, r3, 1

        and r5, r3, r7 
        stio r4, r5, 160 # second row 

        stio r4, r5, 480 # fourth row

        and r5, r3, r6 # second row
        stio r4, r5, 161

        stio r4, r3, 320 # third row
        stio r4, r3, 321

        stio r4, r3, 481 # fourth row
        
        j r0

    write_B:
        nop

    write_C:
        nop

    write_D:
        nop

    write_E:
        nop

    write_F:
        nop

    write_G:
        nop

    write_H:
        nop

    write_I:
        nop

    write_J:
        nop

    write_K:
        nop

    write_L:
        nop

    write_M:
        nop

    write_N:
        nop

    write_O:
        nop

    write_P:
        nop

    write_Q:
        nop

    write_R:
        nop

    write_S:
        nop

    write_T:
        nop

    write_U:
        nop

    write_V:
        nop

    write_W:
        nop

    write_X:
        nop

    write_Y:
        nop

    write_Z:
        nop

    write_0:
        nop

    write_1:
        nop

    write_2:
        nop

    write_3:
        nop

    write_4:
        nop

    write_5:
        nop

    write_6:
        nop

    write_7:
        nop

    write_8:
        nop

    write_9:
        nop

    write_exclamation:
        nop

    write_question:
        nop

    write_dot:
        nop

    write_comma:
        nop

    write_colon:
        nop

    write_semicolon:
        nop

    write_equals:
        nop

    write_plus:
        nop

    write_minus:
        nop

    write_underscore:
        nop

    write_slash:
        nop

    write_backslash:
        nop

    write_quote:
        nop

    write_double_quote:
        nop

    write_asterisk:
        nop

    write_left_bracket:
        nop

    write_right_bracket:
        nop

    write_caret:
        nop

    write_percentage:
        nop

    write_pipe:
        nop

    write_space:
        nop


# display resolution 160 x 120, two pixels per word
fill_display:
    # arguments: r0 = return address, r1 = value to fill

    li r2, 0 # start index
    li r4, 0 # address in vram
    li r5, 0 # index in words (2bytes), aka index * 2
    li r6, 9600 # number of pixels left to fill = 160 * 120 / 2 (resolution over two pixels per word)
    li r7, 0 # temp register to check for end of loop
    li r3, 0x8000 # start of vram
    loop:
        add r5, r2, r2 # index * 2 (word size)
        add r4, r3, r5 # address in vram
        stio r4, r1, 0 # store pixel values at address in array
        addi r2, r2, 1 # increment index
        sub r7, r6, r2, r7 # this might cause a bug
        jnz r7, loop

    j r0 # return to caller

main:
    li r0, exit # return address
    li r1, 0x0F0F # value to fill
    jimm fill_display # fill display with value


exit:
    nop 
    