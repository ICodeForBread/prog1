// Author: Boyue, Lukas
#include "base.h"

enum dam_mode_e {
    DM_IDLE,
    DM_PUMP_ONE,
    DM_PUMP_TWO,
    DM_PUMP_EMERGENCY,
};

const int WATERLEVEL_LOW = 20;
const int WATERLEVEL_MEDIUM = 40;
const int WATERLEVEL_HIGH = 67.5;

typedef enum dam_mode_e DamMode;

DamMode dam_control(double water_level) {
    if(water_level < WATERLEVEL_LOW) {
        return DM_IDLE;
    } else if(water_level < WATERLEVEL_MEDIUM) {
        return DM_PUMP_ONE;
    } else if(water_level < WATERLEVEL_HIGH) {
        return DM_PUMP_TWO;
    } else {
        return DM_PUMP_EMERGENCY;
    }
}

void dam_control_test(void) {
    // TODO
    test_equal_i(dam_control(0.), DM_IDLE);
    test_equal_i(dam_control(20.), DM_PUMP_ONE);
    test_equal_i(dam_control(40.), DM_PUMP_TWO);
    test_equal_i(dam_control(123557.), DM_PUMP_EMERGENCY);
}

int main(void) {
    dam_control_test();
    return 0;
}
