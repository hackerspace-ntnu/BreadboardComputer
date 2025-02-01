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
    int pcW = 0b1 <<8;
    int pcO = 0b10 <<8;
    int adrW = 0b100 <<8;
    int adrO = 0b1000 <<8;
    int rW = 0b10000 <<8;
    int rO = 0b100000 <<8;
    int irW = 0b1000000 <<8;
    int pcINC = 0b10000000 <<8;

    //eeprom3
    int nand_xor = 0b1 <<16;
    int add_xor = 0b10 <<16;
    int sumO = 0b100 <<16;
    int aluW = 0b1000 <<16;
    int carryIN = 0b10000 <<16;
    int jump_zero = 0b100000 <<16;
    int jump_negative = 0b1000000 <<16;
    int set_flag = 0b10000000 <<16;

    //instruksjoner opcode er første elementeet (index 0)
    int move[]={0b1000,regdW|regs1O};
    int add[] = {0b10000, regs1O|aluW, regs2O|add_xor, regdW|sumO}; //2 out register legges sammen skrives til regd
    int loadi[]={0b11000, pcO|adrW, rO|regdW|pcINC}; //legger til neste i minnet skriver til regd
    int negate[]={0b100000, regs1O|aluW, regs1O|nand_xor|carryIN, sumO|regdW};
    int store[]={0b101000,regs1O|adrW, rW|regs2O};
    //hoppet over adresse 6...
    int addi[]={0b110000, pcO|adrW, rO|aluW|pcINC, regs1O|add_xor, sumO|regdW};
    int jump[] = {0b111000, regs1O|pcW};
    int sub[] = {0b1000000, regs2O|aluW, regs2O|nand_xor, sumO|aluW,regs1O|add_xor|set_flag|carryIN, sumO|regdW};
    int jumpZ[]={0b1001000, jump_zero|regs1O};
    int jumpZI[]={0b1010000,pcO|adrW|pcINC ,rO|jump_zero};
    int jumpI[]={0b1011000,pcO|adrW|pcINC, rO|pcW};
    int load_offset_immediate[]={0b1100000, pcO|adrW, rO|aluW|pcINC, regs1O|add_xor, sumO|adrW, rO|regdW};
    int store_offset_immediate[]={0b1101000, pcO|adrW, rO|aluW|pcINC, regs2O|add_xor, sumO|adrW, regs1O|rW};
    int inc[]={0b1110000, regs1O|aluW, carryIN|add_xor, sumO|regdW};
    int jumpNI[]={0b1111000, pcO|adrW|pcINC, rO|jump_negative};
    //load offset laster in fra adresse regs1 + immideate dvs pc+1 til regd
    //store_offset_immediate lagdrer data i regs1O til adresse regs2O + immediate

    public void test_binary_in_java(){
        //print_tabell(add);
        print_tabell_array(add, move, loadi, negate,store,jump,sub,jumpZ, jumpZI, jumpI,
        load_offset_immediate, store_offset_immediate, addi, inc, jumpNI);
    }

    public String binary_to_hexadecimal_string(int binary){
        StringBuilder out = new StringBuilder(Integer.toString(binary,16));
        while (out.length()<8) {
            out.insert(0,"0");
        }
        return out.toString();
    }

    public void print_tabell_array(int[]... instruksjon){
        //4 8 og 16 er størellsen på arraysa
        String tabellmatrise[][][] = new String[4][8][16];
        //String matrise[][] = new String[8][16];
    
        // iniatilserer tabellen putter 00 i alle elementene
        for (int i = 0; i < tabellmatrise.length; i++) {
            for (int j = 0; j < tabellmatrise[i].length; j++) {
                for (int j2 = 0; j2 < tabellmatrise[i][j].length; j2++) {
                    tabellmatrise[i][j][j2]="00";

                    //fetch instruksjon for eeprom2
                    if(i==1){
                        if(j2==0 || j2==8){
                        tabellmatrise[i][j][j2] = "0"+(Integer.toString((pcO|adrW)>>8,16));
                    }
                        if(j2==1 ||j2==9){
                            tabellmatrise[i][j][j2]=Integer.toString((rO|irW|pcINC)>>8,16);
                        }
                    }
                }
            }
        }

        //Leger til alle instruksjonsdataen inn i tabellene
        for (int i = 0; i < instruksjon.length; i++) {
            for (int j = 1; j < instruksjon[i].length; j++) {
                String mikro_instruksjon = binary_to_hexadecimal_string(instruksjon[i][j]);
                int addresse = instruksjon[i][0]+j+1;

                //matrise[addresse/16][addresse%16]=mikro_instruksjon;
                for (int k = 0; k < tabellmatrise.length; k++) {
                    tabellmatrise[k][addresse/16][addresse%16]=
                    mikro_instruksjon.substring(mikro_instruksjon.length()-(k+1)*2,
                    mikro_instruksjon.length()-(k*2));
                }
            }
        }

        //legacy code skriver ut den originale tabellen til konsollen
        /*for (int i = 0; i < matrise.length; i++) {
            System.out.println("\n");
            for (int j = 0; j < matrise[0].length; j++) {
                System.out.print(matrise[i][j] +" ");
            }
        }*/

        //skriver ut tabellene til konsollen
        for (int i = 0; i < tabellmatrise.length; i++) {
            System.out.println("");
            System.out.print("\nEEprom"+(i+1));
            for (int j = 0; j < tabellmatrise[i].length; j++) {
                System.out.println("\n");
                System.out.print(Integer.toString(j, 16)+"  ");
                for (int j2 = 0; j2 < tabellmatrise[i][i].length; j2++) {
                    System.out.print(tabellmatrise[i][j][j2] + " ");
                }
            }
        }
    }
}
