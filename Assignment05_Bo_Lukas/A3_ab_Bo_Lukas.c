// Author: Boyue, Lukas
#include "base.h"
/*
    It's a function,which can make the negative integer positive and
    make positive integer triple itself.
*/
int f(int i) {
	printf("called f\n");
	if (i < 0) {
        return -i;
	} else {
		return 3 * i;
	}
}