# cursed bubble sort of an array with 5 elements

li r0, 64
li r0, 5 # array size
li r7, 0x1000 # array address

# All this creates an empty array :O
# commonly known as:
# .data: array: .word: 0, 0, 0, 0, 0
li r1, 0 # start index
li r2, 0 # value to fill
intialize_array:
    # load zero into array in ram
    add r5, r1, r1 # index * 2 (word size)
    add r4, r7, r5 # address of array element
    st r4, r2 # store 0 at address in array

    # for loop logic
    addi r1, r1, 1 # increment index
    sub r3, r0, r1, r6 # r6 is reserved for virtual instruction :skull:
    jnz r3, intialize_array

# Fill the array with 5 random values
fill_array:
    li r1, 3 # 
    stio r7, r1, 0 # store 3 at address 0
    li r1, 5 #
    stio r7, r1, 2 # store 5 at address 1
    li r1, 1 #
    stio r7, r1, 4 # store 1 at address 2
    li r1, 4 #
    stio r7, r1, 6 # store 4 at address 3
    li r1, 10 #
    stio r7, r1, 8 # store 10 at address 4

# r7 is the address of the array
# r0 is the size of the array

li r1, 0 # outer loop index
li r2, 0 # inner loop index
bubble_sort:
    # for loop logic
    addi r1, r1, 1 # increment index
    li r2, 0 # reset inner loop index
    jimm inner_loop
    checkpoint:
        sub r3, r0, r1, r6 # r6 is reserved for virtual instruction :skull:
        jnz r3, end

swap:
    st r4, r5 # store r5 at address to r6 in array
    li r3, 2
    sub r4, r4, r3, r3 # address of r5 in array (we give up r3 because we won't need it after)
    st r4, r6 # store r6 at address to r5 in array
    jimm return_point

# iterate through the array and swap elements if needed
inner_loop:
    # load value in array at index r2
    add r3, r1, r1 # index * 2 (word size)
    add r4, r7, r3 # address of array element
    ldind r5, r4 # load value at address in array

    # load value in array at index r2 + 1
    addi r4, r4, 2 # address of next array element
    ldind r6, r4 # load value at address in array

    # compare values
    sub r3, r6, r5, r3 # r3 as destination AND as temporary register :O
    jn r3, swap # if r6 < r5, swap

    return_point:
        addi r2, r2, 1 # increment inner loop index
        sub r3, r0, r2, r6 # r6 is reserved for virtual instruction :skull:
        jnz r3, inner_loop
    
    jimm checkpoint


end:
    # the array is hopefully sorted
    nop