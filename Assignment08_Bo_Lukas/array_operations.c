// Author: Boyue, Lukas
#include "base.h"

typedef struct better_array_s {
    int length;
    int *array;
} BetterArray;

//todo: a)
bool compare(int *array_a, int length_a, int *array_b, int length_b) {
    if (length_a != length_b) {
        return false;
    } else if (length_a < 0 || length_b < 0) {
        return false;
    }

    for (int i = 0; i < length_a; i++) {
        if (array_a[i] != array_b[i]) {
            return false;
        }
    }
    return true;
}

// helper
void printArr(int arr[], int length) {
    printf("{");
    for (int i = 0; i < length; ++i){
        printf(" %d,", arr[i]);
    }
    printf("}\n");
}

//todo: b)
int remove_negatives(int* array, int length) {
    int helpArr[length];
    for (int i = 0; i < length; ++i) {
        helpArr[i] = array[i];
    }
    int newLength = 0;
    for (int i = 0; i < length; ++i) {
        if (helpArr[i] >= 0) {
            newLength++;
        }
    }
    int offset = 0;
    int pos = 0;
    for (int i = 0; i < length; ++i) {
        if (helpArr[i] < 0) {
            array[newLength + offset] = helpArr[i];
            offset++;
        } else {
            array[pos] = helpArr[i];
            pos++;
        }
        
    }
    return newLength;
}

//todo: c)
BetterArray intersect(int *array_a, int length_a, int* array_b, int length_b) {
    BetterArray output = {0, array_a};

    int pos = 0;
    if (length_a <= length_b) {
        for (int i = 0; i < length_a; ++i) {
            for (int j = 0; j < length_b; ++j) {
                if (array_a[i] == array_b[j]) {
                    array_a[pos] = array_b[j];
                    pos++;
                }
            }  
        }
        output.length = pos;
        output.array = array_a;
    } else {
        for (int i = 0; i < length_b; ++i) {
            for (int j = 0; j < length_a; ++j) {
                if (array_a[j] == array_b[i]) {
                    array_b[pos] = array_a[j];
                    pos++;
                }
            }
        }
        output.length = pos;
        output.array = array_b;
    }
    return output;
}

//todo: d)
void merge_sorted_arrays(int *array_a, int length_a, int* array_b, int length_b, int* result, int length_result) {
    int aPos = 0;
    int bPos = 0;

    for (int i = 0; i < length_result; ++i) {
        if (array_a[aPos] <= array_b[bPos] && aPos < length_a) {
            result[i] = array_a[aPos];
            aPos++;
        } else {
            result[i] = array_b[bPos];
            bPos++;
        }
    }

}

void test_compare(){
    int a1[] = {1, 2, 3, -4, 8, 9};
    int length_a1 = 6;
    int a2[] = {1, 2, 3, -4, 8, 9};
    int length_a2 = 6;
    int a3[] = {1, -2, 3, 4, 8, 9};
    int length_a3 = 6;

    int a4[] = {1};
    int length_a4 = 1;

    int a5[] = {2};
    int length_a5 = 1;

    test_equal_b(compare(a1, length_a1, a2, length_a2), true);
    test_equal_b(compare(a1, length_a1, a2, 1), false);
    test_equal_b(compare(a1, 0, a2, length_a2), false);
    test_equal_b(compare(NULL, 0, NULL, 0), true);
    test_equal_b(compare(a3, 1, a2, 1), true);
    test_equal_b(compare(a3, length_a3, a2, length_a2), false);
    test_equal_b(compare(a3, 2, a2, 2), false);
    test_equal_b(compare(a1, -2, a2, 2), false);
    test_equal_b(compare(a1, -2, a2, -2), false);
    test_equal_b(compare(a1, -2, a2, -1), false);
    test_equal_b(compare(a1, 2, a2, -2), false);
    test_equal_b(compare(a4, length_a4, a4, length_a4), true);
    test_equal_b(compare(a1, 1, a4, length_a4), true);
    test_equal_b(compare(a1, length_a1, a4, length_a4), false);
    test_equal_b(compare(a4, length_a4, a5, length_a5), false);
}

void test_remove_negatives(){
    int a1[] = {1, 2, 3, -4, 8, 9};
    int length_a1 = 6;
    int a1_expected[] = {1, 2, 3, 8, 9};

    int a2[] = {-1, -2, -3, -4, -8, 9};
    int length_a2 = 6;
    int a2_expected[] = {9};

    int a3[] = {-1, -2, -3, -4, -8, -9}; 
    int length_a3 = 6;
    int a3_expected[] = {};

    int new_length = remove_negatives(a1, length_a1);
    test_equal_i(new_length, 5);
    test_equal_b(compare(a1, new_length, a1_expected, 5), true);

    new_length = remove_negatives(a2, length_a2);
    test_equal_i(new_length, 1);
    test_equal_b(compare(a2, new_length, a2_expected, 1), true);

    new_length = remove_negatives(a3, length_a3);
    test_equal_i(new_length, 0);
    test_equal_b(compare(a3, new_length, a3_expected, 0), true);
}

void test_intersect(){
    int a1[] = {1, 2, 3, -4, 8, 9};
    int length_a1 = 6;
    
    int a2[] = {2, -2, 6, -4, -8, 10};
    int length_a2 = 6;
    
    int a2_a1_intersected[] = {2, -4};
    
    
    int a3[] = {1, 17, 3, 5, 9, 8};
    int length_a3 = 6;
    
    int a4[] = {1, 17, 3, 5, 9};
    int length_a4 = 5;
    
    int a3_a4_intersected[] = {1, 17, 3, 5, 9};
    
    
    int a5[] = {1, 3 , -18};
    int length_a5 = 3;
    
    int a6[] = {2, -3, -19};
      int length_a6 = 3;
    
    int a5_a6_intersected[] = {};
    
    BetterArray result = intersect(a1, length_a1, a2, length_a2);
    
    test_equal_i(result.length, 2);
    test_equal_b(compare(result.array, result.length, a2_a1_intersected, 2), true);
    
    result = intersect(a3, length_a3, a4, length_a4);
    
    test_equal_i(result.length, 5);
    test_equal_b(compare(result.array, result.length, a3_a4_intersected, 5), true);
    
    result = intersect(a5, length_a5, a6, length_a6);
    test_equal_i(result.length, 0);
    test_equal_b(compare(result.array, result.length, a5_a6_intersected, 0), true);
}

void test_merge_sorted(){
    int a1[] = {1, 2, 3, 4, 8, 9};
    int length_a1 = 6;
    
    int a2[] = {-2, -2, 6, 14, 15, 100};
    int length_a2 = 6;
    
    int a1_a2_merged[] = {-2, -2, 1, 2, 3, 4, 6, 8, 9, 14, 15, 100};
    
    int result[12];
    
    merge_sorted_arrays(a1, length_a1, a2, length_a2, result, 12);
    test_equal_b(compare(result, 12, a1_a2_merged, 12), true);

    int a3[] = {1};
    int length_a3 = 1;
    
    int a4[] = {15, 100};
    int length_a4 = 2;
    
    int a3_a4_merged[] = {1, 15, 100};
    
    int result2[3];
    
    merge_sorted_arrays(a3, length_a3, a4, length_a4, result2, 3);
    test_equal_b(compare(result2, 3, a3_a4_merged, 3), true);
    
    
    int a5[] = {1, 3, 17};
    int length_a5 = 3;
    
    int a6[] = {};
    int length_a6 = 0;
    
    int a5_a6_merged[] = {1, 3, 17};
    
    int result3[3];
    
    merge_sorted_arrays(a5, length_a5, a6, length_a6, result3, 3);
    test_equal_b(compare(result3, 3, a5_a6_merged, 3), true);
    
    int result4[3];
    
    merge_sorted_arrays(a6, length_a6, a5, length_a5, result4, 3);
    test_equal_b(compare(result4, 3, a5_a6_merged, 3), true);
    
    
}

int main(void) {
    test_compare();
    test_remove_negatives();
    test_intersect();
    test_merge_sorted();

    return 0;
}

/*
better array save the length of a array. c does that not by default.
To get the length of an array you have to use the sizeof() funktion on an array and divide it by the size of the type
*/
