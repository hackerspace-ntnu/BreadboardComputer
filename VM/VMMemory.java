public class VMMemory {
    
    private int[] bios;
    private int[] ram;
    
    public VMMemory() {
        bios = new int[8192];
        ram = new int[65536];
    }
    
    private int[] component(int address) {
        if (0 < address && address < 8192) {
            return bios;
        } else if (8192 < address && address < 65536) {
            return ram;
        } else {
            throw new IllegalArgumentException("The address " + address + " does not exist.");
        }
    }
    
    public int read(int address) {
        return component(address)[address];
    }
    
    public void write(int value, int address) {
        component(address)[address] = value;
    }
    
}
