// Author: Boyue, Lukas
#include "base.h"

/*
calculates the sum of the digits in a number(base 10). if alternating is true the alternating sum is calculated
*/
int digit_sum(unsigned number, bool alternating) {
    int length = 1;
    // get length
    int numberCopy = number;
    while(numberCopy >= 10) {
        length++;
        numberCopy /= 10;
    }

    int output = 0;
    int digitValue;
    while(number) {
        digitValue = number % 10;
        if(length % 2 != 0 && alternating){
            output -= digitValue;
            length--;
        } else {
            output += digitValue;
            length--;
        }
        number -= digitValue;
        number /= 10;
    }
    return output;
}

// checks if a number is devisible by 11. returns true if number is devible or returns false if not
bool divisible_by_eleven(unsigned number) {
    for (unsigned i = 0; i <= number; i += 11) {
        if(i == number) {
            return true;
        }
    }
    return false;
}

void digit_sum_test() {
    // TODO
    test_equal_i(digit_sum(10, false), 1);
    test_equal_i(digit_sum(333, false), 9);
    test_equal_i(digit_sum(1234, false), 10);
    test_equal_i(digit_sum(10, true), -1);
    test_equal_i(digit_sum(333, true), -3);
    test_equal_i(digit_sum(1234, true), 2);
    test_equal_i(digit_sum(113, true), -3);

    // TODO
    test_equal_i(divisible_by_eleven(12), false);
    test_equal_i(divisible_by_eleven(11), true);
    test_equal_i(divisible_by_eleven(999), false);
    test_equal_i(divisible_by_eleven(990), true);
}

int main(void) {
    digit_sum_test();
    return 0;
}
