
public class VM {
    
    private VMMemory memory;
    
    private int pc = 0;
    private int ir = 0;
    
    private int ra = 0;
    private int rb = 0;
    private int rc = 0;
    
    public VM() {
        memory = new VMMemory();
    }
    
}
