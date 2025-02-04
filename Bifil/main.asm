# memory space 0-16kB
# ram starts at 16kB-32kB = 0x1FFF
# vram starts at 32kB-64kB = 0x3FFF

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

    j r0 # return address

main:
    li r0, exit # return address
    li r1, 0x0F0F # value to fill
    jimm fill_display # fill display with value


exit:
    nop 
    