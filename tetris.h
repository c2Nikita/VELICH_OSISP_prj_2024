//
// Created by nikita on 29.04.24.
//

#ifndef TETRIS_H
#define TETRIS_H

#define TICKS_START 30

typedef struct Block {
    int b;
} Block;

typedef struct Figure {
    int x;
    int y;
    int size;
    Block* blocks;
} Figure;

typedef struct FiguresT {
    int count;
    int size;
    Block* blocks;
} FiguresT;

typedef struct Field {
    int width;
    int height;
    Block* blocks;
}Field;
enum {
    GAMEOVER = 0,
    PLAYING
};
enum{
    PLAYER_NOP = 0,
    PLAYER_RIGHT,
    PLAYER_DOWN ,
    PLAYER_UP ,
    PLAYER_LEFT
};
typedef struct Player {
    int action;
}Player;

typedef struct Game {
    Field* field;
    Figure* figure;
    FiguresT* figurest;
    Player* player;
    int ticks;
    int ticks_left;
    int playing;
    int score;
}Game;

Game* createGame(int field_width, int field_height, int figures_size, int count, Block* figures_template);
void calculate(Game* tetg);
void freeFiguresT(FiguresT* tetft);
Field* createField(int width, int height);
void freeField(Field* tetf);
void freeGame(Game* tetg);
void moveFigureDown(Game* tetg);
void moveFigureUp(Game* tetg);
void moveFigureRight(Game* tetg);
void moveFigureLeft(Game* tetg);
int collision(Game* tetg);
void plantFigure(Game* tetg);
int lineFilled(int i, Field* tfl) ;
void dropLine(int i, Field* tfl);
int eraseLines(Game* tetg);
Figure* createFigure(Game* tetg);
void freeFigure(Figure* tf);
Figure* rotFigure(Game* tetg);
void dropNewFigure(Game* tetg);
void calculate(Game* tetg);









#endif
