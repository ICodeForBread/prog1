// Author: Boyue, Lukas
/* 
 * Compile:            make guess_my_number
 * Run:                ./guess_my_number
 * Compile and Run: make guess_my_number && ./guess_my_number
*/
 
#include "base.h"
/*
    guess the randome number which is given by the Computer,while
    the people can get the feedback with "too large,too small,match"
    during the process.
 */
 int main(void)
 {
	 printf("Guess My Number!\n");
	 int i = i_rnd(101);
	 int a;
	 do {
		a = i_input();//give a number people want
		if (a > i) {
		 printf("Too Large!\n");
	 } else if (a < i) {
		 printf("Too Small!\n");
	 } else {
		 printf("Match!\n");
	 } 
	 } while (a != i);//when the number is matched,exit Loop.
	 getchar();
	 return 0;
 }
  
  
  