public class VM {
    
    private final static int addressBound = 65536;
    private final static int instructionCount = 32;
    
    private VMMemory memory;
    
    private int pc = 0;
    private int ir = 0;
    
    private int ra = 0;
    private int rb = 0;
    private int rc = 0;
    
    private VMInstruction[] instructions;
    
    public VM() {
        
        memory = new VMMemory();
        instructions = new VMInstruction[instructionCount];
        
        // Fill instructions ...
        
    }
    
    public void clock() {
        
        pc = (pc + 1) % addressBound;
        ir = memory.read(pc) % instructionCount;
        
        VMInstruction instruction = instructions[ir];
        instruction.execute();
        
    }
    
}
