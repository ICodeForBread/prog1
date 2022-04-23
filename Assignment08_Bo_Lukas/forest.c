// Author: Boyue, Lukas
#include "base.h"

#define WORLD_WIDTH 32
#define WORLD_HEIGHT 16
const String STRING_PLAYER = "☺";

typedef enum tiletype_e {
    TT_GRASS,
    TT_TREE,
    TT_EMERALD,
} TileType;

typedef struct gamestate_s {
    TileType world[WORLD_WIDTH * WORLD_HEIGHT];
    struct {
        unsigned x;
        unsigned y;
    } player;
    int collected_emeralds;
} Gamestate;

void clear() {
    printf("\33[2J");
}

String string_for_tile(TileType tile) {
    switch(tile) {
    case TT_GRASS:
        return " ";
    case TT_TREE:
        return "#";
    case TT_EMERALD:
        return "V";
    default:
        return "X";
    }
}

Gamestate make_gamestate() {
    return (Gamestate) {
        .player.x = WORLD_WIDTH / 2,
        .player.y = WORLD_HEIGHT / 2,
        .collected_emeralds = 0,
    };
}



TileType get_tile(Gamestate* gamestate, unsigned x, unsigned y) {
    require("x to large", x < WORLD_WIDTH);
    require("y to large", y < WORLD_HEIGHT);
    int currTileID = (*gamestate).world[y * WORLD_WIDTH + x];
    return currTileID;
}

void set_tile(Gamestate* gamestate, unsigned x, unsigned y, TileType tile) {
    require("x to large", x < WORLD_WIDTH);
    require("y to large", y < WORLD_HEIGHT);
    (*gamestate).world[y * WORLD_WIDTH + x] = tile;
}

void generate_world(Gamestate* gamestate) {
    for (int i = 0; i < WORLD_WIDTH; ++i) {
        for (int j = 0; j < WORLD_HEIGHT; ++j) {
            int chance = i_rnd(100);
            if (chance < 5) {
                set_tile(gamestate, i, j, TT_EMERALD);
            } else if (chance < 20) {
                set_tile(gamestate, i, j, TT_TREE);
            } else {
                set_tile(gamestate, i, j, TT_GRASS);
            }
        }
    }
}

void draw(Gamestate* gamestate) {
    printf("Player x:%d y:%d \n", (*gamestate).player.x, (*gamestate).player.y);
    for (int i = 0; i < WORLD_HEIGHT; ++i) {
        for (int j = 0; j <= WORLD_WIDTH; ++j) {
            if(j == (*gamestate).player.x && i == (*gamestate).player.y) {
                prints(STRING_PLAYER);
            } else if (j == WORLD_WIDTH) {
                prints("\n");
            } else {
                prints(string_for_tile(get_tile(gamestate, j, i)));
            }
        }
    }
}

void move_player(Gamestate* gamestate, int dx, int dy) {
    if (dx == WORLD_WIDTH) {
        dx -= WORLD_WIDTH;
    } else if (dx == -1) {
        dx += WORLD_WIDTH;
    } else if (dy == WORLD_HEIGHT) {
        dy -= WORLD_HEIGHT;
    } else if (dy == -1) {
        dy += WORLD_HEIGHT;
    }
    TileType target = get_tile(gamestate, dx, dy);
    if (target == TT_GRASS) {
        gamestate->player.x = dx;
        gamestate->player.y = dy;
    } else if (target == TT_EMERALD) {
        gamestate->player.x = dx;
        gamestate->player.y = dy;
        gamestate->collected_emeralds++;
        set_tile(gamestate, dx, dy, TT_GRASS);
    } else {
        printf("Not allowed move\n");
    }
}

int main(void) {
    Gamestate gamestate = make_gamestate();
    generate_world(&gamestate);

    while (gamestate.collected_emeralds < 5) {
        printf("Collected Emeranlds: %d/5\n", gamestate.collected_emeralds);
        draw(&gamestate);
        int move = i_input();
        switch (move) {
            case 1:
                move_player(&gamestate, gamestate.player.x, gamestate.player.y - 1); //up
                break;
            case 2:
                move_player(&gamestate, gamestate.player.x + 1, gamestate.player.y); //right
                break;
            case 3:
                move_player(&gamestate, gamestate.player.x, gamestate.player.y + 1); //down
                break;
            case 4:
                move_player(&gamestate, gamestate.player.x - 1, gamestate.player.y); //left
                break;
        }
        clear();
    }

    printf("You got 5/5 Gems!\n");

    return 0;
}
