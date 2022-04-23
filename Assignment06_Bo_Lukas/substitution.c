// Author: Boyue, Lukas
#include "base.h"

const String permutation = "SGEMFVYZIDPBUKHARNXQWOLJTC";
// helper
int charAllowed(char inChar);

// encrypts a given input string with the permutationkey
String encrypt(String input) {
    String out = s_copy(input);

    for(int i = 0; i < s_length(out); i++){
        char current = s_get(out, i);
        if(charAllowed(current)) {
            current -= 65;
            char traslated = s_get(permutation, current);
            s_set(out, i, traslated);
        } 
    }
    printf("%s\n", out);
    return out;
}

// helper
int charAllowed(char inChar){
    if (inChar >= 65 && inChar <= 90) {
        return 1;
    } else {
       return 0;
    }   
}

// returns the first position of a given char in a givn string
int index_of(String s, char c) {
    String str = s_copy(s);

    for(int i = 0; i < s_length(str); i++) {
        char currentChar = s_get(str, i);
        if(currentChar == c) {
            return i;
        }
    }
    return -1;
}

// decryps a string with the permutationkey
String decrypt(String input) {
    String out = s_copy(input);

    for(int i = 0; i < s_length(out); i++) {
        char current = s_get(out, i);
        if(charAllowed(current)) {
            char translated = index_of(permutation, current);
            translated += 65;
            s_set(out, i, translated);
        }
    }
    return out;
}

void test_identity(String s) {
    assert("encrypted text must differ from input", !s_equals(s, encrypt(s)));
    test_equal_s(s, decrypt(encrypt(s)));
}

int main(void) {
    test_identity("HELLO WORLD");
    test_identity("SOME SUPER SECRET TEXT");
    test_identity("DOES QUOTATION CAUSE ISSUES? IT SHOULDN'T.");
    test_identity("BE SURE TO DRINK YOUR OVALTINE");

    printsln("The secret text says:");
    printsln(decrypt("VNSKC DSYQ IU PHUABFQQ OFNLSZNBHXQFK QSJI RWFN MWNEZ GSTFNK."));
}
