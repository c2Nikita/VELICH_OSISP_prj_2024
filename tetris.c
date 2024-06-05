//
// Created by nikita on 29.04.24.
//

#include "stdlib.h"
#include "tetris.h"

#ifndef TETRIS_C
#define TETRIS_C

FiguresT* createFiguresT(int count, int figures_size, Block* figures_template){
    FiguresT* tetft = (FiguresT*)malloc(sizeof(FiguresT));
    tetft->count = count;
    tetft->size = figures_size;
    tetft->blocks = figures_template;
    return tetft;
};

void freeFiguresT(FiguresT* tetft) {
    if(tetft)
        free(tetft);
}

Field* createField(int width, int height){
    Field* tetf = (Field*)malloc(sizeof(Field));
    tetf->width = width;
    tetf->height = height;
    tetf->blocks = (Block*)malloc(sizeof(Block)*width*height);
    for(int i=0; i<width*height; i++)
        tetf->blocks[i].b=0;

    return tetf;
};

void freeField(Field* tetf){
    if(tetf) {
        if(tetf->blocks)
            free(tetf->blocks);
        free(tetf);
    };
};

void freeGame(Game* tetg){
    if(tetg){
        freeField(tetg->field);
        freeFiguresT(tetg->figurest);
        free(tetg);
    };
};

void moveFigureDown(Game* tetg) {
    tetg->figure->y++;
}
void moveFigureUp(Game* tetg) {
    tetg->figure->y--;
}
void moveFigureRight(Game* tetg) {
    tetg->figure->x++;
}

void moveFigureLeft(Game* tetg) {
    tetg->figure->x--;
}

int collision(Game* tetg) {
    Figure* t = tetg->figure;
    Field* tf = tetg->field;
    for(int i=0;i<t->size;i++)
        for(int j=0;j<t->size;j++){
            if(t->blocks[i*t->size+j].b != 0){
                int fx = t->x+j;
                int fy = t->y+i;
                if(tf->blocks[fy*tf->width+fx].b != 0)
                    return 1;
                if(fx < 0 || fx>= tf->width || fy<0 || fy >= tf->height)
                    return 1;
            }
        }
    return 0;
}

void plantFigure(Game* tetg){
    Figure* t = tetg->figure;
    for(int i=0; i<t->size; i++)
        for(int j=0; j<t->size; j++)
            if(t->blocks[i*t->size+j].b != 0){
                int fx = t->x + j;
                int fy = t->y + i;
                tetg->field->blocks[fy*tetg->field->width+fx].b = t->blocks[i*t->size+j].b;
            }
}

int lineFilled(int i, Field* tfl) {
    for(int j=0;j<tfl->width; j++)
        if(tfl->blocks[i*tfl->width+j].b == 0)
            return 0;
    return 1;
}

void dropLine(int i, Field* tfl){
    if(i == 0) {
        for(int j = 0; j<tfl->width; j++)
            tfl->blocks[j].b = 0;
    } else {
        for(int k=i; k>0;k--)
            for(int j=0; j<tfl->width; j++)
                tfl->blocks[k*tfl->width+j].b = tfl->blocks[(k-1)*tfl->width+j].b;
    }
}

int eraseLines(Game* tetg) {
    Field* tfl = tetg->field;
    int count = 0;
    for(int i=tfl->height-1; i>=0 ;i--) {
        while(lineFilled(i,tfl)) {
            dropLine(i,tfl);
            count++;
        }
    }
    return count;
}

Figure* createFigure(Game* tetg) {
    Figure* t = (Figure*)malloc(sizeof(Figure));
    t->x = 0;
    t->y = 0;
    t->size = tetg->figurest->size;
    t->blocks = (Block*)malloc(sizeof(Block)*t->size*t->size);
    return t;
}

void freeFigure(Figure* tf) {
    if(tf){
        if(tf->blocks)
            free(tf->blocks);
        free(tf);
    }
}

Figure* rotFigure(Game* tetg){
    Figure* t = createFigure(tetg);
    Figure* told = tetg->figure;
    t->x = told->x;
    t->y = told->y;
    for(int i=0; i<t->size ; i++)
        for(int j=0; j<t->size ; j++)
            t->blocks[i*t->size+j].b = told->blocks[j*t->size+t->size-1-i].b;
    return t;
}

void dropNewFigure(Game* tetg) {
    Figure* t = createFigure(tetg);
    t->x = tetg->field->width/2 - t->size/2;
    t->y = 0;
    int fnum = rand() % tetg->figurest->count;
    for(int i=0; i<t->size ; i++)
        for(int j=0; j<t->size; j++) {
            t->blocks[i*t->size+j].b = tetg->figurest->blocks[fnum*t->size*t->size+
                                                              i*t->size+j].b;
        }
    freeFigure(tetg->figure);
    tetg->figure = t;
}

void calculate(Game* tetg){
    if(tetg->ticks_left <= 0){
        tetg->ticks_left = tetg->ticks;
        moveFigureDown(tetg);
        if(collision(tetg)){
            moveFigureUp(tetg);
            plantFigure(tetg);
            tetg->score += eraseLines(tetg);
            dropNewFigure(tetg);
            if(collision(tetg)){
                tetg->playing = GAMEOVER;
                return;
            }
        }
    }
    switch(tetg->player->action){
        case PLAYER_RIGHT:
            moveFigureRight(tetg);
            if(collision(tetg))
                moveFigureLeft(tetg);
            break;
        case PLAYER_DOWN:
            moveFigureDown(tetg);
            if(collision(tetg))
                moveFigureUp(tetg);
            break;
        case PLAYER_LEFT:
            moveFigureLeft(tetg);
            if(collision(tetg))
                moveFigureRight(tetg);
            break;
        case PLAYER_UP: {
            Figure* t = rotFigure(tetg);
            Figure* told = tetg->figure;
            tetg->figure = t;
            if(collision(tetg)) {
                tetg->figure = told;
                freeFigure(t);
            } else {
                freeFigure(told);
            }
        }
        break;
        case PLAYER_NOP:
        default:
            break;
    }
    tetg->ticks_left--;
}
Game* createGame(int field_width, int field_height, int figures_size, int count, Block* figures_template){

    Game* tetg = (Game*)malloc(sizeof(Game));
    tetg->field = createField(field_width, field_height);
    tetg->figurest = createFiguresT(count, figures_size, figures_template);

    tetg->ticks = TICKS_START;
    tetg->ticks_left = TICKS_START;
    tetg->score = 0;
    tetg->playing = PLAYING;

    return tetg;
};



#endif