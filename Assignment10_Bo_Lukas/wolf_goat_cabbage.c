// Author: Boyue, Lukas
#include "base.h"

/**
List of objects on the left river bank.
*/
List left;

/**
List of objects on the right river bank.
*/
List right;

/**
List of objects in the boat. The boat has a capacity of one object only.
*/
List boat;

/**
The boat may either be at the left or right river bank. We don't care for the transition (boat crossing the river).
*/
enum Position {
	LEFT = -1, RIGHT = 1
};

// enum for the objects to carray around
enum obj {
	cabbage,
	goat,
	wolf
};

/**
Current boat position.
*/
enum Position position = LEFT;

void changeSide(void) {
	position *= -1;
}

void print_situation(void) {
	sl_print(left);
	if (position == RIGHT) prints("     ");
	sl_print(boat);
	if (position == LEFT)  prints("     ");
	sl_println(right);
}

void finish(void) {
	l_free(left);
	l_free(right);
	l_free(boat);
	exit(0);
}

void checkFailedState(List list) {
	if (sl_contains(list, "wolf") && sl_contains(list, "goat")) {
		printf("Wolf eats Goat!\n");
		finish();
	} else if (sl_contains(list, "goat") && sl_contains(list, "cabbage")) {
		printf("Goat eats Cabbage!\n");
		finish();
	} else if (l_length(right) == 3) {
		printf("YOU WON!\n");
		finish();
	}
}

void evaluate_situation(void) {
	print_situation();
	if (position == LEFT) {
		checkFailedState(right);
	} else {
		checkFailedState(left);
	}
}

bool starts_with(String element, int index, String x) {
	return s_starts_with(element, x);
}

void loadUnloadBoat(char* str) {
	bool isOnboat = sl_contains(boat, str);
	if (position == LEFT && isOnboat) { // unload to left
		sl_remove(boat, 0);
		sl_append(left, str);
	} else if (position == RIGHT && isOnboat) { // unload to right
		sl_remove(boat, 0);
		sl_append(right, str);
	} else if (position == LEFT && l_length(boat) < 1) { // load from left
		if (sl_contains(right, str)) {return;}
		int index = sl_index(left, str);
		sl_remove(left, index);
		sl_append(boat, str);
	} else if (position == RIGHT && l_length(boat) < 1) { // load from right
		if (sl_contains(left, str)) {return;}
		int index = sl_index(right, str);
		sl_remove(right, index);
		sl_append(boat, str);
	}
}

void play_wolf_goat_cabbage(void) {
	//init game
	left = sl_create();
	boat = sl_create();
	right = sl_create();
	sl_append(left, "wolf");
	sl_append(left, "goat");
	sl_append(left, "cabbage");

	print_situation();

	// player interaction
	char c;
	char str[10];
	int off = 0;
	while((c = getchar()) != 'q') {
		if (c != '\n') {
			str[off] = c;
			off++;
		} else {
			str[off] = '\0';
			off = 0;

			char* s = str;

			if (!s_contains("wWgGcClr", (String) s)) {
				continue;
			}

			char wolf[10] = "wolf";
			char goat[10] = "goat";
			char cabbage[10] = "cabbage";

			if (*s == 'w' || *s == 'W') {
				s = wolf;
			} else if (*s == 'g' || *s == 'G') {
				s = goat;
			} else if (*s == 'c' || *s == 'C') {
				s = cabbage;
			}

			if (*s == 'l' || *s == 'r') {
				changeSide();
			} else {
				loadUnloadBoat(s);
			}
			evaluate_situation();
		}
	}
	finish();
}

int main(void) {
	report_memory_leaks(true);
	play_wolf_goat_cabbage();
	return 0;
}
