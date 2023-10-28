import java.util.ArrayList;

public class mikro_instruksjoner {
    //eeprom 1
    byte regdW = 0b01;
    byte regdO = 0b10;
    byte regs1W = 0b100;
    byte regs1O = 0b1000;
    byte regs2W = 0b10000;
    byte regs2O = 0b100000;

    //eeprom 2
    int pcW = 0b1 *0x100;
    int pcO = 0b10 *0x100;
    int adrW = 0b100 * 0x100;
    int adrO = 0b1000 * 0x100;
    int rW = 0b10000 * 0x100;
    int rO = 0b100000 * 0x100;
    int irW = 0b100000 *0x100;
    int pcINC = 0b10000000 <<8;

    //eeprom3
    int nand_xor = 0b1 <<16;
    int add_xor = 0b10 <<16;
    int sumO = 0b100 <<16;
    int aluW = 0b1000 <<16;
    int carryIN = 0b10000 <<16;

    //instruksjoner opcode er første elementeet (index 0)
    int move[]={0b1000,regdW|regs1O};
    int add[] = {0b10000, pcINC|regs1O|aluW, regs2O|add_xor, regdW|sumO};

    public void test_binary_in_java(){
        print_tabell(add);
    }

    public String binary_to_hexadecimal_string(int binary){
        StringBuilder out = new StringBuilder(Integer.toString(binary,16));
        while (out.length()<8) {
            out.insert(0,"0");
        }
        return out.toString();
    }

    public void print_tabell(int[] instruksjon){
        String tabellmatrise[][][] = new String[4][8][16];
        String matrise[][] = new String[8][16];

        for (int i = 0; i < tabellmatrise.length; i++) {
            for (int j = 0; j < tabellmatrise[i].length; j++) {
                for (int j2 = 0; j2 < tabellmatrise[i][i].length; j2++) {
                    tabellmatrise[i][j][j2]="00";
                }
            }
        }
        
        for (int i = 1; i < instruksjon.length; i++) {
            int addresse = instruksjon[0]+i+1;
            String mikro_instruksjon = binary_to_hexadecimal_string(instruksjon[i]);

            matrise[addresse/16][addresse%16]=mikro_instruksjon;
            for (int j = 0; j < tabellmatrise.length; j++) {
                tabellmatrise[j][addresse/16][addresse%16]=
                mikro_instruksjon.substring(mikro_instruksjon.length()-(j+1)*2,
                mikro_instruksjon.length()-(j*2));
            }
        }
        /*for (int i = 0; i < matrise.length; i++) {
            System.out.println("\n");
            for (int j = 0; j < matrise[0].length; j++) {
                System.out.print(matrise[i][j] +" ");
            }
        }*/

        for (int i = 0; i < tabellmatrise.length; i++) {
            System.out.println("");
            System.out.print("\nEEprom"+(i+1));
            for (int j = 0; j < tabellmatrise[i].length; j++) {
                System.out.println("\n");
                for (int j2 = 0; j2 < tabellmatrise[i][i].length; j2++) {
                    System.out.print(tabellmatrise[i][j][j2] + " ");
                }
            }
        }
    }
}
