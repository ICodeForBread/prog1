// Author: Boyue, Lukas
#include "base.h"

// 1 lb = 0.45359237 kg

typedef enum unit_e {
    G,
    KG,
    T,
    LB
} Unit;

typedef struct weight_s {
    double amount;
    Unit unit;
} Weight;

Weight make_weight(double amount, Unit unit) {
    Weight thisWeight = {amount, unit};
    return thisWeight;
}

bool test_within_weight(int line, Weight a, Weight e, double tolerance) {
    bool x = base_test_within_d(__FILE__, line, a.amount, e.amount, tolerance);
    bool u = base_test_equal_i(__FILE__, line, a.unit, e.unit);
    return x && u;
}

void printUnit(Unit unit){
    switch(unit) {
        case G:
            prints("g");
            break;
        case KG:
            prints("kg");
            break;
        case T:
            prints("t");
            break;
        case LB:
            prints("lb");
            break;
    }
}

void print_weight(Weight w) {
    printf("%.2f ", w.amount);
    printUnit(w.unit);
    printf("\n");
}

void print_weight_test() {
    print_weight(make_weight(1234, G));
    print_weight(make_weight(4.749, KG));
    print_weight(make_weight(3.1001, T));
    print_weight(make_weight(5.40006, LB));
}

// helper function
// takes a weight and turns it into gramms
Weight toGram(Weight w) {
    switch(w.unit) {
        case G:
            break;
        case KG:
            w.amount *= 1000;
            w.unit = G;
            break;
        case T:
            w.amount *= 1000000;
            w.unit = G;
            break;
        case LB:
            w.amount *= 453.59237;
            w.unit = G;
            break;
        default:
            break;
    }
    return w;
}

// Weight, Unit -> Weight
// Convert weight to the given target unit.

Weight to_unit(Weight w, Unit target_unit) {
    // to gramms (base unit)
    w = toGram(w);

    // to targent unit
    switch(target_unit) {
        case G:
            break;
        case KG:
            w.amount /= 1000;
            w.unit = KG;
            break;
        case T:
            w.amount /= 1000000;
            w.unit = T;
            break;
        case LB:
            w.amount /= 453.59237;
            w.unit = LB;
            break;
        default:
            break;
    }
    return w;
}

void to_unit_test(void) {
    test_within_weight(__LINE__, to_unit(make_weight(1000, G), KG), make_weight(1, KG), 1e-6);
    test_within_weight(__LINE__, to_unit(make_weight(1000, KG), G), make_weight(1000000, G), 0);
    test_within_weight(__LINE__, to_unit(make_weight(10, LB), G), make_weight(4535.92, G), 1e-2);
    test_within_weight(__LINE__, to_unit(make_weight(1, G), KG), make_weight(0.001, KG), 1e-6);
    test_within_weight(__LINE__, to_unit(make_weight(1000000, G), T), make_weight(1, T), 1e-6);
    test_within_weight(__LINE__, to_unit(make_weight(1000, LB), KG), make_weight(453.592, KG), 1e-3);
    
}

// Weight, Weight -> int
// Compares two weights. Returns 0 if the two weights are equal, 
// -1 if w is smaller than v and +1 otherwise.
int compare(Weight w, Weight v) {
    w = toGram(w);
    v = toGram(v);

    if (w.amount == v.amount) {
        return 0;
    } else if (w.amount < v.amount) {
        return -1;
    } else if (w.amount > v.amount) {
        return 1;
    } else {
        return 400; // otherwise i would get a warning
    }
}

void compare_test(void) {
    test_equal_i(compare(make_weight(1000, G), make_weight(1, KG)), 0);
    test_equal_i(compare(make_weight(1, G), make_weight(0.001, KG)), 0);
    test_equal_i(compare(make_weight(1000, G), make_weight(1, G)), 1);
    test_equal_i(compare(make_weight(100, G), make_weight(1, KG)), -1);
    test_equal_i(compare(make_weight(1000000, G), make_weight(1, T)), 0);
    test_equal_i(compare(make_weight(1000000, G), make_weight(0.999, T)), 1);
}

int main(void) {
    print_weight_test();
    to_unit_test();
    compare_test();
    return 0;
}
