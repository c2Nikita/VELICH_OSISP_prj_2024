#include <stdlib.h>
#include <stdio.h>
#include "tetris.h"
#include "time.h"
#include <ncurses.h>
#include <unistd.h>
Block tet_templates[] = {
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,

        0,0,0,0,0,
        0,0,1,0,0,
        0,1,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,

        0,0,0,0,0,
        0,0,1,1,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,0,0,0,

        0,0,0,0,0,
        0,1,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,0,0,0,

        0,0,0,0,0,
        0,0,1,1,0,
        0,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,

        0,0,0,0,0,
        0,1,1,0,0,
        0,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,

        0,0,0,0,0,
        0,1,1,0,0,
        0,1,1,0,0,
        0,1,1,0,0,
        0,0,0,0,0,

};
void printGame(Game* tetg){
    Field* tf = tetg->field;
    Figure* t = tetg->figure;
    for(int i=0; i<tf->height; i++) {
        for(int j=0;j<tf->width; j++) {
            int sym =1;
           if(tf->blocks[i*tf->width+j].b!=0)
               sym = 2;
           else {
               int x = j - t->x;
               int y = i - t->y;
               if(x >=0 && x < t->size && y >= 0 && y < t->size)
                   if(t->blocks[y*t->size+x].b != 0)
                       sym = 2;
           }
           attron(COLOR_PAIR(sym));
           mvaddch(i,j, ' ');
           attroff(COLOR_PAIR(sym));
        }
    }
}
int main(int argc, char* argv[]) {

    struct timespec sp_start, sp_end, ts1,ts2 ={0,0};


    FILE *file;
    int record;
    file = fopen("/home/nikita/CLionProjects/SimpleTetris/src/file.txt","r");
    if(file== NULL)
        return 1;

    if(fscanf(file,"%d", &record) !=1){
        fclose(file);
        return 1;
    }
    fclose(file);
    initscr();
    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLUE);
    init_pair(2,COLOR_GREEN,COLOR_GREEN);
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);
    scrollok(stdscr, TRUE);


    Game* tetg = createGame(20,15,5,7,tet_templates);//7
    Player player;
    player.action = PLAYER_NOP;
    tetg->player = &player;
    dropNewFigure(tetg);
    while(tetg->playing != GAMEOVER){
        clock_gettime(CLOCK_MONOTONIC,&sp_start);

        int ch = getch();
        switch (ch) {
            case ERR:
                player.action=PLAYER_NOP;
                break;
            case 'w':
                player.action=PLAYER_UP;
                break;
            case 's':
                player.action=PLAYER_DOWN;
                break;
            case 'a':
                player.action=PLAYER_LEFT;
                break;
            case 'd':
                player.action=PLAYER_RIGHT;
                break;
            default:
                player.action=PLAYER_NOP;
                break;
        }
        
        
        
        calculate(tetg);
        printGame(tetg);

        attron(COLOR_PAIR(1));
        mvprintw(0,0,"Score: %d", tetg->score);
        mvprintw(0,10,"Record: %d", record);
        move(tetg->field->height+1,tetg->field->width+1);
        attroff(COLOR_PAIR(1));



        clock_gettime(CLOCK_MONOTONIC,&sp_end);
        if(sp_end.tv_sec-sp_start.tv_sec<=0 && (ts2.tv_nsec=10000000 - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)
            nanosleep(&ts2,&ts1);

    }
    freeGame(tetg);
    if(record < tetg->score){
        truncate("file.txt", 0);
        file = fopen("/home/nikita/CLionProjects/SimpleTetris/src/file.txt","w");
        if(file == NULL)
            return 1;

        if(fprintf(file,"%d", tetg->score) !=1){
            fclose(file);
            return 1;
        }
        fclose(file);

    }
    endwin();
    return 0;
}
