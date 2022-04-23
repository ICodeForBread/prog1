// Author: Boyue, Lukas
/* 
 * Compile:            make leap_years
 * Run:                ./leap_years
 * Compile and Run: make leap_years && ./leap_years
*/

#include "base.h"
/*
    Return the number of days in the given year.
*/
int number_of_days(int year) {
	int result;
	bool multiple_of_four;
	bool multiple_of_hundred;
	bool multiple_of_fourhundred;
    if (year % 4 == 0) {//Leap Years are any year that can be exactly divided by 4
        multiple_of_four = true;
    } else {
		multiple_of_four = false;		
	}
	if (year % 100 == 0) {//except if it can be exactly divided by 100, then it isn't
		multiple_of_hundred = true;
	} else {
		multiple_of_hundred = false;
	}
	if (year % 400 == 0) {//except if it can be exactly divided by 400, then it is
		multiple_of_fourhundred = true;
	} else {
		multiple_of_fourhundred = false;
	}
	if ((!multiple_of_four) || (multiple_of_hundred && (!multiple_of_fourhundred))){
		result = 365;//this year is not a leap year.
	} else {
		result = 366;//this year is a leap year.
	}
	
	return result;
 }
 
 void number_of_days_test() {
	 test_equal_i(number_of_days(1800), 365);
	 test_equal_i(number_of_days(1900), 365);
	 test_equal_i(number_of_days(2000), 366);
	 test_equal_i(number_of_days(2020), 366);
 }
 
 int main(void) {
    // todo: call test function
    number_of_days_test();
	
    return 0;
}