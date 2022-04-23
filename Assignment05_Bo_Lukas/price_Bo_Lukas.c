// Author: Boyue, Lukas
/*
 * Compile:         make X
 * Run:             ./X
 * Compile and Run: make X && ./X
 */
#include "base.h" // prog1lib

// todo: define constants
int ORDER_AMMOUNT_0 = 0;
int ORDER_AMMOUNT_0_PRICE = 0;
int ORDER_AMMOUNT_1 = 1;
int ORDER_AMMOUNT_1_PRICE = 100; // in cents
int ORDER_AMMOUNT_10 = 10;
int ORDER_AMMOUNT_10_PRICE = 90; // in cents
int ORDER_AMMOUNT_50 = 50;
int ORDER_AMMOUNT_50_PRICE = 75; // in cents

int SHIPPING_FREE_MIN_PRICE = 2000; // in cents
int SHIPPING = 500;

// todo: write function price
/*
a function to calcuate the total price for an order
args: ammount of orderd items
returns:  the total price
*/
int priceTotal(int ammount) {
    int price = 0;

    if (ammount <= ORDER_AMMOUNT_0) {
        return price;
    } else if (ammount < ORDER_AMMOUNT_10) {
        price = ammount * ORDER_AMMOUNT_1_PRICE;
    } else if (ammount < ORDER_AMMOUNT_50) {
        price = ammount * ORDER_AMMOUNT_10_PRICE;
    } else {
        price = ammount * ORDER_AMMOUNT_50_PRICE;
    }

    if (price < SHIPPING_FREE_MIN_PRICE) {
        price += SHIPPING;
    }

    return price;
}

// todo: write function price_test
// test returned values of price using test_equal_i
void priceTest(void) {
    test_equal_i(priceTotal(0), 0);
    test_equal_i(priceTotal(5), 1000);
    test_equal_i(priceTotal(10), 1400);
    test_equal_i(priceTotal(49), 4410);
    test_equal_i(priceTotal(100), 7500);
}

int main(void) {
    // todo: call price_test function
    priceTest();
    return 0;
}
 