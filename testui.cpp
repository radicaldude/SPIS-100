#include "node.h"
#include <locale.h>

#define NODE_HEIGHT 11
#define ARROW_WIDTH 1
#define ARROW_HEIGHT 1
#define GAP_WIDTH_H 1
#define GAP_WIDTH_V 0
#define NODE_WIDTH 25
#define CODE_WIDTH 20


int grid_size[2] = {4,4};
std::vector<node> grid;

void drawContent();

WINDOW *new_win(int height, int width, int starty, int startx){
    WINDOW *win;
    win=newwin(height, width, starty, startx);
    box(win,0,0);
    wrefresh(win);
    return win;
}

int main(){
    int c,x=GAP_WIDTH_H, y=0, id;
    int max_x, max_y;
    node *tmp_node;
    int nID=0;
    WINDOW *menu;
    
    initscr();
    getmaxyx(stdscr, max_y, max_x);
    y=0;
    refresh();
    for(int i=0;i<grid_size[0]; i++){
        x=GAP_WIDTH_H;
        for(int j=0;j<grid_size[1];j++){
            tmp_node = new node(nID);
            nID++;
            tmp_node->w_main=new_win(NODE_HEIGHT, NODE_WIDTH, y, x);
            tmp_node->w_code=newwin(NODE_HEIGHT - 2, CODE_WIDTH - 2, y + 1 , x + 1);
            new_win(NODE_HEIGHT, CODE_WIDTH, y, x);
            tmp_node->w_reg =newwin(NODE_HEIGHT-2,NODE_WIDTH-CODE_WIDTH-2, y+1, x+CODE_WIDTH+1);
            grid.push_back(*tmp_node);
            
            wprintw(tmp_node->w_reg, "ACC%d\nBAK%d", tmp_node->acc, tmp_node->bak);
            wrefresh(tmp_node->w_reg);
            x=x+NODE_WIDTH+ARROW_WIDTH+2*GAP_WIDTH_H;
            refresh();
            
            if((int) i<grid_size[0]){
                tmp_node->arrows[0]=new arrow(nID, nID-grid_size[0]);
                tmp_node->arrows[0]->win=newwin(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT+GAP_WIDTH_V,x+NODE_WIDTH/2);
            }
            if((i%grid_size[0])!=0){
                tmp_node->arrows[3]=new arrow(nID, nID-1);
                tmp_node->arrows[3]->win=newwin(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH_H-NODE_WIDTH);
            }
            if((i/grid_size[0]-1)<grid_size[1]){
                tmp_node->arrows[2]=new arrow(nID, nID+grid_size[0]);
                tmp_node->arrows[2]->win=newwin(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH_H-NODE_WIDTH);
            }
            if(((i+1)%grid_size[0])!=0){
                tmp_node->arrows[1]=new arrow(nID, nID+1);
                tmp_node->arrows[1]->win=newwin(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH_H-NODE_WIDTH);
            }
        }
        y=y+(NODE_HEIGHT+2*GAP_WIDTH_V+ARROW_WIDTH);
    }
    grid[0].inputCode.push_back("cucks!");
    grid[0].inputCode.push_back("Hello, World");
    drawContent();
    getch();
    endwin();
    return 0;
}

void drawContent() {
    for(int i = 0; i < grid.size(); i++) {
        node tmp_node = grid[i];
        for(int y = 0; y < tmp_node.inputCode.size(); y++) {
            mvwprintw(tmp_node.w_code, y, 0, tmp_node.inputCode[y].c_str());
            wrefresh(tmp_node.w_code);
        }
    }
}
