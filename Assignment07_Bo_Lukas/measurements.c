// Author: Boyue, Lukas
#include "base.h"

typedef enum Tech_e {
    m = 1,
    t,
    k
} Tech;

typedef struct statistics_s {
    double avgAge;
    double ageVariance;
    double avgMouseTime;
    double avgTouchscreenTime;
    double avgKeybordTime;
} Statistics;

typedef struct {
    int age;
    String tech;
    double time;
} LineContents;

Statistics make_statistics(void) {
    Statistics stats = {.avgAge = 0, .ageVariance = 0.0, .avgMouseTime = 0.0, .avgTouchscreenTime = 0.0, .avgKeybordTime = 0.0};
    return stats;
}

// helper
int getLineAmmount(String s) {
    int textLength = s_length(s);
    int lines = 0;
    for (int i = 0; i < textLength; ++i) {
        if (s_get(s, i) == '\n') {
            lines++;
        }
    }
    return lines;
}

String getLine(String s, int line) {
    int textLength = s_length(s);
    int start;
    int end;

    int lines = 0;
    for (int i = 0; i < textLength; ++i) {
        if (s_get(s, i) == '\n') {
            lines++;
        }
        if (lines == line) {
            start = i + 2; // i dont know why +2 but it needs to be there
        } else if (lines == line + 1) {
            end = i - 1;
        }
    }

    return s_sub(s, start, end);
}

LineContents getLineContents(String file, int line) {
    String s = getLine(file, line);
    int age = i_of_s(s_sub(s, 0, 2));
    String tech = s_sub(s, 3, 4);
    double time = d_of_s(s_sub(s, 5, 13));

    LineContents cont = {.age = age, .tech = tech, .time = time};
    return cont;
}

void print_statistics(Statistics s) {
    printf("Average age: %.2f years\n", s.avgAge);
    printf("Age deviation: %.2f years\n", s.ageVariance);
    printf("Average mouse time: %.2f s\n", s.avgMouseTime);
    printf("Average touchscreen time: %.2f s\n", s.avgTouchscreenTime);
    printf("Average keybord time: %.2f s\n", s.avgKeybordTime);

}

Statistics compute_statistics(String table) {
    String s = s_copy(table);
    Statistics stats = make_statistics();

    int lines = getLineAmmount(s);

    // age avg
    double avgAge = 0;
    for (int i = 0; i < lines - 1; ++i) {
        LineContents line = getLineContents(s, i);
        avgAge += line.age;
    }
    avgAge /= lines - 1;

    double ageVariance = 0;
    int m = 0;
    int t = 0;
    int k = 0;
    double mTime = 0;
    double tTime = 0;
    double k_Time = 0;

    for (int i = 0; i < lines - 1; ++i) {
        LineContents line = getLineContents(s, i);
        ageVariance += (line.age - avgAge) * (line.age - avgAge);

        if (s_equals(line.tech, "m")) {
            m++;
            mTime += line.time;
        } else if (s_equals(line.tech, "t")) {
            t++;
            tTime += line.time;
        } else if (s_equals(line.tech, "k")) {
            k++;
            k_Time += line.time;
        }
    }
    stats.avgAge = avgAge;
    stats.ageVariance = sqrt(ageVariance/lines);
    stats.avgMouseTime = mTime / m;
    stats.avgTouchscreenTime = tTime / t;
    stats.avgKeybordTime = k_Time / k;

    return stats;
}

int main(void) {
    String table = s_read_file("measurements.txt");
    Statistics statistics = compute_statistics(table);
    print_statistics(statistics);
    return 0;
}
