public class instruksjoner {
    mikro_instruksjoner mikro = new mikro_instruksjoner();
    String reg1 = "000";
    String reg2 = "100";
    String reg3 = "010";
    String sp = "111";
    String dummy = "110";

    String move = "1000000";
    String add = "0100000";
    String loadi = "1100000";
    String negate = "0010000";
    String store = "1010000";
    String jump = "1110000";
    String sub = "0001000";
    String jumpz = "1001000";
    String jumpzi = "0101000";
    String jumpi = "1101000";
    String loadOffsetImmediate = "0011000";
    String storeOffsetImmediate = "1011000";
    String increment = "0111000";
    String jumpni = "1111000";

    /*int registere[] = new int[8];
    public instruksjoner(){
        for (int i = 0; i < 8; i++) {
        registere[i]=i;
        }
    }*/
    public void instruksjonerr(){
        //System.out.println(mikro.add[0]>>3);
        //System.out.println(registere[5]);

        //her skriver du instruksjonene du vil legge til
        konverter_til_maskinkode(add+reg1+reg2+reg3);
        konverter_til_maskinkode_immediate(loadi + reg1, 0xff);
        konverter_til_maskinkode(sub+reg1+reg2+reg3);
        konverter_til_maskinkode(jumpz+reg1+reg2);

        konverter_til_maskinkode_immediate(storeOffsetImmediate +reg2+ reg2 + reg3, 6);
        //den første reg 2 gjør ingenting fordi vi ikke har en regDW register desitnation write
        //skal lagre innholdet i reg 2 til adressen reg3+6

        System.out.println("Sorterings algoritme / digdat øving 4. Data ligger adresse 20 i hex");
        System.out.println("samme som 32 i 10 tallsystemet, den første adressen = lengden på data");

        konverter_til_maskinkode_immediate(loadi + sp, 32);
        konverter_til_maskinkode(move+reg3+sp);
        konverter_til_maskinkode_immediate(loadOffsetImmediate + reg2+sp, 0);
        konverter_til_maskinkode(sub+reg3+reg3+sp);
        konverter_til_maskinkode(sub+dummy+reg2+reg3); //setter zero flag hvis reg2 = reg3
        konverter_til_maskinkode_immediate(jumpzi, 0); //hopper til starten hvis vi er gjennom
        konverter_til_maskinkode(add+reg3+reg3+sp);
        konverter_til_maskinkode(increment + reg3 + reg3);
        konverter_til_maskinkode_immediate(loadOffsetImmediate+reg1+reg3, 0);
        konverter_til_maskinkode_immediate(loadOffsetImmediate+reg2+reg3, 1);

        System.out.println("etter loada tall");
        konverter_til_maskinkode(sub + dummy + reg1 + reg2); // hvis negativ = riktig rekkefølge
        konverter_til_maskinkode_immediate(jumpni, 3); //hopper til instruksjon 3
        konverter_til_maskinkode_immediate(storeOffsetImmediate + dummy + reg2 + reg3, 0);
        konverter_til_maskinkode_immediate(storeOffsetImmediate + dummy + reg1 + reg3, 1);
        konverter_til_maskinkode_immediate(jumpi, 3);
    }

    public void konverter_til_maskinkode(String... instruksjon){
        for (int i = 0; i < instruksjon.length; i++) {
            StringBuilder output = new StringBuilder(instruksjon[i]);
            Integer output1 = Integer.parseInt(output.reverse().toString(),2);
            System.out.println(Integer.toString(output1, 16));
            //System.out.println("\n"+Integer.parseInt(output.reverse().toString(),2));
        }
    }
    public void konverter_til_maskinkode_immediate(String instruksjon, int data){
        StringBuilder output = new StringBuilder(instruksjon);
        Integer output1 = Integer.parseInt(output.reverse().toString(),2);
        System.out.println(Integer.toString(output1,16));
        System.out.println(Integer.toString(data,16));
    }
}
