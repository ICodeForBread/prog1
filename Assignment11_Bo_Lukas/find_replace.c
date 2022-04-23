// Author: Boyue, Lukas
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool test_equal_s(const char * a, const char * b);
bool test_equal_i(int a, int b);

int find_substring(const char * str, const char * substr, int starting) {
    char * result = strstr(str + starting, substr);
    if (result) {
        return result - str;
    } else {
        return -1;
    }
}

int count_substrings(const char * str, const char * substr) {
    // TODO
	int i;
	int count = 0;
	int substr_len = strlen(substr);
	for (i = 0; str[i] != '\0'; i++) {
		if (strstr(&str[i], substr) == &str[i]) {
			count++;
			
			//jumping to the index which locates after the substring.
			i += substr_len - 1;
		}
	}
    return count;
}

char * string_replace(const char * str, const char * rep, const char * new) {
    // TODO
	int i = 0;
	char* N_Str;
	int rep_len = strlen(rep);
	int new_len = strlen(new);
    N_Str = (char*)malloc(strlen(str) + count_substrings(str, rep) * (new_len - rep_len) + 1);
	while (*str) {
		if (strstr(str, rep) == str) {
            strcpy(&N_Str[i], new);
            i += new_len;
            str += rep_len;
        }
        else {
            N_Str[i++] = *str++;
		}	
    }
  
    N_Str[i] = '\0';
    return N_Str;
}

// Helper function to test the string_replace function which also takes care of
// memory management.
bool test_replace(const char * str, const char * rep, const char * new, const char * expected) {
    char * s = string_replace(str, rep, new);
    bool passed = test_equal_s(s, expected);
    free(s);
    return passed;
}

int main(void) {
    bool passed = true;

    passed &= test_equal_i(find_substring("testtexttesttext", "test", 0), 0);
    passed &= test_equal_i(find_substring("testtexttesttext", "test", 1), 8);
    passed &= test_equal_i(find_substring("testtexttesttext", "Test", 0), -1);
    passed &= test_equal_i(find_substring("testtexttesttext", "test", 9), -1);
    passed &= test_equal_i(find_substring("0", "String", 0), -1);

    passed &= test_equal_i(count_substrings("", "Beispiel"), 0);
    passed &= test_equal_i(count_substrings("Beispieltext", "text"), 1);
    passed &= test_equal_i(count_substrings("testtexttesttext", "test"), 2);

    passed &= test_replace("Hello World!", "World", "Welt",
            "Hello Welt!");
    passed &= test_replace("Hello Hello!", "Hello", "Hallo",
            "Hallo Hallo!");
    passed &= test_replace("Hahahaha!", "a", "o",
            "Hohohoho!");
    passed &= test_replace("Das ist ein Test.", "ein Test", "ein langer Test",
            "Das ist ein langer Test.");
    passed &= test_replace("Das ist ein kurzer Test.", "kurzer ", "",
            "Das ist ein Test.");
    passed &= test_replace("Soup Soup Soup.", "Soup", "Suppe",
            "Suppe Suppe Suppe.");
    passed &= test_replace("SuppeSuppeSuppe.", "Suppe", "Soup",
            "SoupSoupSoup.");

    if(passed) {
        puts("all tests passed");
        return EXIT_SUCCESS;
    } else {
        puts("some tests didn't pass");
        return EXIT_FAILURE;
    }
}

bool test_equal_i(int a, int b) {
    if(a != b) {
        printf("test failed: %d != %d\n", a, b);
        return false;
    } else {
        return true;
    }
}

bool test_equal_s(const char * a, const char * b) {
    if(strlen(a) != strlen(b) || strcmp(a, b) != 0) {
        printf("test failed: \"%s\" != \"%s\"\n", a, b);
        return false;
    } else {
        return true;
    }
}

