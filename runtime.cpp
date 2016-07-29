#include "spis.h"
#include "ui.h"
#include <cncurses>
  
int compute_tick(std::vector<node> nodes, std::vector<io> inputs, std::vector<io> outputs){
  int i;
  
  for(i=0;i<inputs.size();i++){
    if(inputs[0].arrow[0].status==WAIT){ //get next input

    }
  }
  for(i=0; i<nodes.size();i++){
    node[i].runline();
  }
  for(i=0;i<outputs.size();i++){
    //get output
  }
}

int main(int argc, char **argv){
  WINDOW *menu_win;
  std::vector<WINDOW*> grid;
  int max_y, max_x;

  grid = malloc(sizeof(WINDOW)*grid_size[0]*grid_size[1]);
  initscr();
  curs_set(TRUE);
  getmaxyx(stdscr, max_y, max_x);  
  menu_win = new_win(max_y, menu_width, starty, startx);
  init_level(&grid, grid_size, inputs, outputs);
}

void init_level(std::vector<node> grid, int *grid_size, std::vector<io> inputs, std::vector<io> outputs){
  int c,x=MENU_WIDTH+BOX_WIDTH, y=0, id;
  int max_x;
  node tmp_node;
  
  getmaxyx(stdscr, y, max_x);
  max_y=y;
  for(int i=0, i<grid_size[0]; i++){
    y=y+NODE_HEIGHT+2*GAP_WIDTH+ARROW_WIDTH;
    for(int j=0, j<grid_size[1];j++){
      grid.push_back(tmp_node());
      tmp_node = grid[i*grid_size[0]+j];
      tmp_node.w_main=new_win(NODE_HEIGHT, NODE_WIDTH, y, x);
      tmp_node.w_code=new_win(NODE_HEIGHT, CODE_WIDTH-2, y, x+1);
      tmp_node.w_reg=new_win(NODE_HEIGHT,NODE_WIDTH-CODE_WIDTH-2, y, x+CODE_WIDTH+1);
      wprintw(tmp_node.w_reg, "ACC:%d\nBAK:%d", tmp_node.acc, tmp_node.bak);
      wborder(tmp_node.w_reg, '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
      wborder(tmp_node.w_main, '|', '|', '-', '-', '+', '+', '+', '+');
      x=x+NODE_WIDTH+ARROW_WIDTH+2*GAP_WIDTH;
    }
    y=y-NODE_HEIGHT-GAP_WIDTH;
  }
  c=0;
  for(int i=0;i<grid_size[0];i++){
    if(i=inputs[c].x){
      inputs.arrow = grid[i].arrow[0];
      c++;
    }
    if(c>inputs.size())
      break;
  }
  for(int i=0;i<grid.size();i++){
    y=max_y;
    if(i%grid.size()==0){
      x=MENU_WIDTH+GAP_WIDTH;
      y=y-NODE_HEIGHT-GAP_WIDTH;
    }
    arrow[1]=new(arrow);
    tmp_node.arrows[0].win=new_win(ARROW_HEIGHT, ARROW_WIDTH, y+BOX_HEIGHT/2+ARROW_HEIGHT/2, x+NODE_WIDTH+GAP);
    arrow[2]=new(arrow);
    tmp_node.arrow[2].win=new_win(ARROW_HEIGHT, ARROW_WIDTH, y-GAP, x+NODE_WIDTH/2);
    if((int) i<grid_size[0]){
      arrow[0]=new(arrow);
      tmp_node.arrow[0]=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT+GAP,x+NODE_WIDTH/2);
    }
    if(grid_size[0]%i==0){
      arrow[3]=new(arrow);
      tmp_node.arrow[3]=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP-NODE_WIDTH);
    }
  }
  return;
}

void update(){
  
  

}
