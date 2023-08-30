
public class VM {
    
    private int[] bios = new int[8092];
    private int[] ram;
    
    private int pc = 0;
    private int ir = 0;
    
    private int ra = 0;
    private int rb = 0;
    private int rc = 0;
    
    public VM() {
        ram = new int[1 << 16];
    }
    
    private int memory(int address) {
        if (address < 8092) {
            return bios[address];
        } else {
            return ram[address];
        }
    }
    
}
