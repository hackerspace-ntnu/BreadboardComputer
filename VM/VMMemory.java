/**
 * The {@code VMMemory} represents the physical computer's main memory. It consist of a BIOS (EEPROM) part and a RAM part.
 */
public class VMMemory {
    
    /**
     * The memory's BIOS part is a small amount of memory that is not lost when the system loses power. The first
     * 8192 addresses point to BIOS. This is 
     */
    private int[] bios;
    
    /**
     * The memory's RAM part is the computer's main memory. 
     */
    private int[] ram;
    
    /**
     * Construct a new {@code VMMemory} instance representing a computer's physical memory module.
     * @param addressBound The number of addresses that the {@code VMMemory} instance has space for.
     */
    public VMMemory(int addressBound) {
        bios = new int[8192];
        ram = new int[addressBound];
    }
    
    /**
     * The memory module contains a BIOS part and a RAM part. Whenever the address is in [0 , 8191], we refer
     * to the BIOS part. Whenever the address is ≥ 8192, we refer to the RAM part.
     * @param address The address to reference
     * @return Either the {@code bios} array or the {@code ram} array, depending on what address was passed in
     */
    private int[] component(int address) {
        if (0 < address && address < 8192) {
            return bios;
        } else if (8192 < address && address < ram.length) {
            return ram;
        } else {
            throw new IllegalArgumentException("The address " + address + " does not exist.");
        }
    }
    
    /**
     * Read data from the memory module at a specified address.
     * @param address The address to read from
     * @return Data at the given address
     */
    public int read(int address) {
        return component(address)[address];
    }
    
    /**
     * Write data to the memory module at a specified address.
     * @param value The value to write
     * @param address The address to write to
     */
    public void write(int value, int address) {
        // TODO: Det bør ikke være lov å skrive til BIOS.
        component(address)[address] = value;
    }
    
}
