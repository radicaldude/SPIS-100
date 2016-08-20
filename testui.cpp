#include "node.h"

#define MENU_WIDTH 20
#define BOX_WIDTH 10
#define BOX_HEIGHT 20
#define NODE_HEIGHT 30
#define ARROW_WIDTH 10
#define ARROW_HEIGHT 10
#define GAP_WIDTH 5
#define NODE_WIDTH 10
#define CODE_WIDTH 4


WINDOW *new_win(int height, int width, int starty, int startx){
  WINDOW *win;

  win=newwin(height, width, starty, startx);
  box(win,0,0);
  wrefresh(win);
  return win;
}

int main(){
  int c,x=MENU_WIDTH+BOX_WIDTH, y=0, id;
  int max_x, max_y;
  node *tmp_node;
  int nID=0;
  int grid_size[2] = {4,4};
  std::vector<node> grid;

  initscr();
  getmaxyx(stdscr, y, max_x);
  max_y=y;
  for(int i=0;i<grid_size[0]; i++){
    y=y+NODE_HEIGHT+2*GAP_WIDTH+ARROW_WIDTH;
    for(int j=0;j<grid_size[1];j++){
      tmp_node = new node(nID);
      nID++;
      tmp_node->w_main=new_win(NODE_HEIGHT, NODE_WIDTH, y, x);
      tmp_node->w_code=new_win(NODE_HEIGHT, CODE_WIDTH-2, y, x+1);
      tmp_node->w_reg =new_win(NODE_HEIGHT,NODE_WIDTH-CODE_WIDTH-2, y, x+CODE_WIDTH+1);
      grid.push_back(*tmp_node);
      wprintw(tmp_node->w_reg, "ACC:%d\nBAK:%d", tmp_node->acc, tmp_node->bak);
      x=x+NODE_WIDTH+ARROW_WIDTH+2*GAP_WIDTH;
    }
    y=y-NODE_HEIGHT-GAP_WIDTH;
  }
  c=0;
  /*for(int i=0;i<grid_size[0];i++){
    if(i==inputs[c].x){
      inputs.arrow = grid[i].arrow[0];
      c++;
    }
    if(c>inputs.size())
      break;
      }*/
  for(int i=0;i<grid.size();i++){
    y=max_y;
    if(i%grid_size[0]==0){
      x=MENU_WIDTH+GAP_WIDTH;
      y=y-NODE_HEIGHT-GAP_WIDTH;
    }
    if((int) i<grid_size[0]){
      tmp_node->arrows[0]=new arrow(nID, nID-grid_size[0]);
      tmp_node->arrows[0]->win=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT+GAP_WIDTH,x+NODE_WIDTH/2);
    }
    if((i%grid_size[0])!=0){
      tmp_node->arrows[3]=new arrow(nID, nID-1);
      tmp_node->arrows[3]->win=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH-NODE_WIDTH);
    }
    if((i/grid_size[0]-1)<grid_size[1]){
      tmp_node->arrows[2]=new arrow(nID, nID+grid_size[0]);
      tmp_node->arrows[2]->win=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH-NODE_WIDTH);
    }
    if(((i+1)%grid_size[0])!=0){
      tmp_node->arrows[1]=new arrow(nID, nID+1);
      tmp_node->arrows[1]->win=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH-NODE_WIDTH);
    }
  }
  endwin();
  return 0;
}
