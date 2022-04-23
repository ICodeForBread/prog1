// Author: Boyue, Lukas
#include "base.h"

typedef struct draw_options_s {
    bool has_border;
    String fill_pattern;
} DrawOptions;

typedef struct rectangle_s {
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
} Rectangle;

/* Gibt ein DrawOptions Struct zurück, das dafür sorgt dass Rechtecke nur durch
 * Rauten und ohne Rahmen skizziert werden.
 */
DrawOptions make_draw_options_flat() {
    return (DrawOptions){
        .has_border = false,
        .fill_pattern = "#",
    };
}

/* Gibt ein DrawOptions Struct zurück, das dafür sorgt dass Rechtecke mit einem
 * Rahmen skizziert werden. Die Füllung des Rechtecks wird durch den String
 * "fill" bestimmt, der ständig wiederholend in das Rechteck geschrieben wird.
 */
DrawOptions make_draw_options_fill(String fill) {
    require("fill is a non-empty string", s_length(fill) > 0);

    return (DrawOptions){
        .has_border = true,
        .fill_pattern = fill,
    };
}

DrawOptions make_draw_options_range(char from, char to) {
    require("char form needs to be smaler then the char to", from < to);
    require("form needs to be smaler then 126 and larger then 32", from <= 126 && from >= 32);
    require("to needs to be smaler then 126 and larger then 32", to <= 126 && to >= 32);

    // make a placeholder string
    String s = s_range(from, to + 1);

    return (DrawOptions){
        .has_border = true,
        .fill_pattern = s,
    };
}

Rectangle make_rectangle(int x, int y, int w, int h) {
    require("X not negative", x >= 0);
    require("Y not negative", y >= 0);
    require("w not negative", w >= 0);
    require("h not negative", h >= 0);

    Rectangle rect = {x, y, w, h};
    return rect;
}

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point start;
    Point end;
} Line;

// helper
bool comparePoints(Point a, Point b) {
    if (a.x == b.x && a.y == b.y) {
        return true;
    } else {
        return false;
    }
}

bool lineIntersect(Line line1, Line line2) {
    int line1Length = line1.end.x == line1.start.x ? line1.end.y-line1.start.y : line1.end.x-line1.start.x;
    int line1direction = line1.end.x == line1.start.x ? 0 : 1;
    int line2Length = line2.end.x == line2.start.x ? line2.end.y-line2.start.y : line2.end.x-line2.start.x;
    int line2direction = line2.end.x == line2.start.x ? 0 : 1;

    Point a = line1.start;
    Point b = line2.start;

    for (int i = 0; i <= line1Length; ++i) {
        line1direction == 1 ? a.x++ : a.y++;

        b = line2.start;
        for (int j = 0; j <= line2Length; ++j) {
            line2direction == 1 ? b.x++ : b.y++;

            if (comparePoints(a, b)) {
                return true;
            }
        }
    }

    return false;
}

bool intersects(Rectangle a, Rectangle b) {

    Point aCorners[] = {{a.x, a.y}, {a.x + a.w, a.y}, {a.x, a.y + a.h}, {a.x + a.w, a.y + a.h}};
    Point bCorners[] = {{b.x, b.y}, {b.x + b.w, b.y}, {b.x, b.y + b.h}, {b.x + b.w, b.y + b.h}};

    Line aOutline[] = { {aCorners[0], aCorners[1]}, // top
                        {aCorners[0], aCorners[2]}, // left
                        {aCorners[1], aCorners[2]}, // right
                        {aCorners[2], aCorners[3]}  // bottom
                      };
    Line bOutline[] = { {bCorners[0], bCorners[1]},
                        {bCorners[0], bCorners[2]},
                        {bCorners[1], bCorners[2]},
                        {bCorners[2], bCorners[3]}
                       };
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (lineIntersect(aOutline[i], bOutline[j])) {
                return true;
            }
        }
    }

    return false;
}

void insert(int offset) {
    for (int i = 0; i < offset; ++i) {
        prints(" ");
    }
}

void print_rectangle(Rectangle rect, DrawOptions options) {
    for (int i = 0; i < rect.y; ++i) {
        prints("\n");
    }

    int stringPos = 0;
    for (int i = 1; i <= rect.h; ++i) {
        insert(rect.x); 
        for (int j = 1; j <= rect.w; ++j) {
            bool isFirstRow = i == 1;
            bool isLastRow = i == rect.h;
            bool isFirstCol = j == 1;
            bool isLastCol = j == rect.w;

            if (options.has_border) {
                if ((isFirstRow && isFirstCol) || (isLastRow && isFirstCol) || (isFirstRow && isLastCol) || (isLastRow && isLastCol)) {       
                    prints("+");
                } else if (isFirstRow || isLastRow) {
                    prints("-");
                } else if (isFirstCol || isLastCol) {
                    prints("|");
                } else {
                    String s = s_copy(options.fill_pattern);
                    printc(s_get(s, stringPos));
                    stringPos++;
                    if (stringPos == s_length(s)) {
                        stringPos = 0;
                    }
                } 
            } else {
                prints("#");
            }
        }
        prints("\n");
    }
}

void test_intersects(void) {
    test_equal_i(intersects(
        make_rectangle(0, 0, 10, 10),
        make_rectangle(32, 32, 10, 10)
    ), false);

    test_equal_i(intersects(
        make_rectangle(100, 100, 32, 16),
        make_rectangle(50, 32, 50, 10)
    ), false);

    test_equal_i(intersects(
        make_rectangle(50, 32, 50, 10),
        make_rectangle(100, 100, 32, 16)
    ), false);

    test_equal_i(intersects(
        make_rectangle(16, 16, 16, 16),
        make_rectangle(16, 16, 4, 4)
    ), true);

    test_equal_i(intersects(
        make_rectangle(16, 16, 16, 16),
        make_rectangle(8, 20, 32, 4)
    ), true);

    test_equal_i(intersects(
        make_rectangle(16, 16, 16, 16),
        make_rectangle(20, 8, 4, 32)
    ), true);
}

int main(void) {
    test_intersects();

    print_rectangle(make_rectangle(0, 0, 9, 5), make_draw_options_fill(" "));
    puts("===");
    print_rectangle(make_rectangle(3, 3, 10, 5), make_draw_options_fill("123"));
    puts("===");
    print_rectangle(make_rectangle(5, 2, 4, 6), make_draw_options_fill(":_:"));
    puts("===");
    print_rectangle(make_rectangle(1, 2, 4, 4), make_draw_options_flat());
    puts("===");

    print_rectangle(make_rectangle(0, 0, 9, 5), make_draw_options_range('a', 'c'));
    puts("===");
    print_rectangle(make_rectangle(4, 4, 10, 3), make_draw_options_range('1', '9'));
    puts("===");
    print_rectangle(make_rectangle(0, 0, 2, 2), make_draw_options_range('A', 'C'));
    puts("===");
    print_rectangle(make_rectangle(9, 9, 10, 10), make_draw_options_range('Z', 'a'));
    puts("===");
    print_rectangle(make_rectangle(12, 0, 4, 1), make_draw_options_flat());
    puts("===");
    print_rectangle(make_rectangle(0, 0, 9, 9), make_draw_options_fill("ababac"));
    puts("===");

    return 0;
}

/* Beispielausgaben:

print_rectangle(make_rectangle(0, 0, 9, 5), make_draw_options_fill(" "));
+-------+
|       |
|       |
|       |
+-------+

print_rectangle(make_rectangle(3, 3, 10, 5), make_draw_options_fill("123"));



   +--------+
   |12312312|
   |31231231|
   |23123123|
   +--------+

print_rectangle(make_rectangle(5, 2, 4, 6), make_draw_options_fill(":_:"));


     +--+
     |:_|
     |::|
     |_:|
     |:_|
     +--+

print_rectangle(make_rectangle(1, 2, 4, 4), make_draw_options_flat());


 ####
 ####
 ####
 ####

*/
