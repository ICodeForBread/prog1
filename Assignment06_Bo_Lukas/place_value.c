// Author: Boyue, Lukas
#include "base.h"

String characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//todo: write purpose statement inclusive @param and @return
/*
calculates how long a number is for a given base
takes a number and a base
returns the lenght
*/
int length_for_base(int number, int base){
    if(number == 0)
        return 1;
    double needed_chars = log(number + 1)/log(base);
    int length = ceil(needed_chars);

    return length;
}

/*
    Gibt einen dynamisch allokierte String zurueck, der so viele Zeichen hat wie noetig sind um die Zahl in der Zielbasis darzustellen. 
    @param number Zahl, die in der Zielbasis dargestellt wird.
    @param base Zielbasis, in der die Zahl number dargestellt werden soll.  
    @return Eine Zeichenkette mit der noetigen Anzahl an Stellen mit '#' initialisiert.
*/
String get_string_for_number_and_base(int number, int base){
    int length = length_for_base(number, base);
    String s = xcalloc(sizeof(char), length + 1);
    //printf("length: %d\n", length);
    for(int i = 0; i< length; i++){
        s[i] = '#';
    }
    return s;
}

// todo
// converts a base 10 number to a string of the number in the given base
String convert_to_base(int number, int base){
    if (base < 2) {
        return "";
    }
    String s = get_string_for_number_and_base(number, base);

    int currentValue;
    for (int i = length_for_base(number, base) - 1; i >= 0; i--) {
        currentValue = number % base;
        s_set(s, i, s_get(characters, currentValue));

        number -= currentValue;
        number /= base;
    }
    
    return s;
}

/*
    Schoen formatierte Ausgabe der Zahl, die zu konvertieren ist, sowie des Ergebnisses.
    @param number Zahl, die in der Zielbasis dargestellt wird.
    @param base Zielbasis, in der die Zahl number dargestellt werden soll.  
*/
void print_convert(int number, int base){
    printf("Convert %4d to base %2d: %s\n", number, base, convert_to_base(number, base));
}
/*
    Testfaelle fuer convert_to_base.
*/
void test_convert_to_base(){
    print_convert(16, -1);
    test_equal_s(convert_to_base(16, -1), "");
    
    print_convert(10, 1);
    test_equal_s(convert_to_base(10, 1), "");
    
    print_convert(0, 2);
    test_equal_s(convert_to_base(0, 2), "0");
    
    print_convert(16, 3);
    test_equal_s(convert_to_base(16, 3), "121");
    
    print_convert(16, 16);
    test_equal_s(convert_to_base(16, 16), "10");
    
    print_convert(100, 10);
    test_equal_s(convert_to_base(100, 10), "100");
    
    print_convert(1000, 10);
    test_equal_s(convert_to_base(1000, 10), "1000");
    
    print_convert(0, 2);
    test_equal_s(convert_to_base(0, 2), "0");
    
    print_convert(10, 16);
    test_equal_s(convert_to_base(10, 16), "A");
    
    print_convert(100, 2);
    test_equal_s(convert_to_base(100, 2), "1100100");
    
    print_convert(20, 5);
    test_equal_s(convert_to_base(20, 5), "40");
    
    print_convert(1300300300, 36);
    test_equal_s(convert_to_base(1300300300, 36), "LI5Y4S");
}

int main(void){
    test_convert_to_base();
    return 0;
}
