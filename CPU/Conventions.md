# This file specifies naming conventions

Clock -                         '*CLK*'\
Register clock -                '*CLK*'
Registers -                     'R*0*', 'R*1*', 'R*2*', 'R*3*'\
Stack Pointer Register -        '*SP*' (Fysisk identisk, unngå å kalle R7)

ALU In -                        '*ALU*'\
ALU Out -                       '*ALU*'


Instruction Register -          '*I*R'\
Adress Register -               '*MA*R'


Read / In / Load (from R0) -    'R0*in*'\
Write / Out / Store (from R0) - 'R0*out*'\
Unconditional Jump / Branch -   '*J*'\
Conditional Jump / Branch -     '*B*'\
Immediate -                     'INSTR*i*'
