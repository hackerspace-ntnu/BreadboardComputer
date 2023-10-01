public class VM {
    
    /**
     * The upper bound on the CPU's addressable memory.
     */
    private final static int addressBound = 32768;
    
    /**
     * The number of instructions that the CPU's instruction set contains.
     */
    private final static int instructionCount = 32;
    
    /**
     * The {@code memory} field of the Virtual Machine represents the physical computer's RAM and EEPROM/BIOS components.
     */
    private VMMemory memory;
    
    /**
     * The CPU's program counter ({@code PC} register).
     */
    private int pc = 0;
    
    /**
     * The CPU's instruction register ({@code IR} register).
     */
    private int ir = 0;
    
    /**
     * A general-purpose CPU register.
     */
    private int ra = 0;
    
    /**
     * A general-purpose CPU register.
     */
    private int rb = 0;
    
    /**
     * A general-purpose CPU register.
     */
    private int rc = 0;
    
    /**
     * The {@code instructions} array contains all the CPU's instructions, stored as {@code VMInstruction} instances.
     * A subscript into this array, {@code instructions[a]}, represents the action of decoding the opcode {@code a}.
     */
    private VMInstruction[] instructions;
    
    /**
     * Construct a new {@code VM} object.
     */
    public VM() {
        
        memory = new VMMemory(addressBound);
        instructions = new VMInstruction[instructionCount];
        
        // Fill instructions ...
        
    }
    
    /**
     * The {@code clock()} method is not equivalent to a single clock cycle on the physical CPU. Instead, it
     * runs an entire <i>instruction</i> (usually 4 or 5 clock cycles).
     */
    public void clock() {
        
        // Increment the program counter to find the next instruction
        pc = (pc + 1) % addressBound;
        
        // Read from memory what instruction is next
        ir = memory.read(pc) % instructionCount;
        
        // Use the opcode (value in the instruction set) to find what instruction to execute
        VMInstruction instruction = instructions[ir];
        
        // Execute the instruction
        instruction.execute();
        
    }
    
}
