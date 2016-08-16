#include "node.h"

#define MENU_WIDTH 20
#define BOX_WIDTH 10
#define NODE_HEIGHT 30
#define ARROW_WIDTH 10
#define GAP_WIDTH 5
#define NODE_WIDTH 10
#define CODE_WIDTH 4

int main(){
  int c,x=MENU_WIDTH+BOX_WIDTH, y=0, id;
  int max_x, max_y;
  node *tmp_node;
  int nID=0;
  int grid_size[2] = {4,4};
  std::vector<node> grid;
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
      wborder(tmp_node->w_reg, '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
      wborder(tmp_node->w_main, '|', '|', '-', '-', '+', '+', '+', '+');
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
    if(i%grid.size()==0){
      x=MENU_WIDTH+GAP_WIDTH;
      y=y-NODE_HEIGHT-GAP_WIDTH;
    }
    tmp_node->arrows[1]=new(arrow);
    tmp_node->arrows[0].win=new_win(ARROW_HEIGHT, ARROW_WIDTH, y+BOX_HEIGHT/2+ARROW_HEIGHT/2, x+NODE_WIDTH+GAP);
    arrows[2]=new arrow;
    tmp_node->arrows[2].win=new_win(ARROW_HEIGHT, ARROW_WIDTH, y-GAP, x+NODE_WIDTH/2);
    if((int) i<grid_size[0]){
      arrows[0]=new(arrow);
      tmp_node->arrow[0]=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT+GAP,x+NODE_WIDTH/2);
    }
    if(grid_size[0]%i==0){
      arrows[3]=new(arrow);
      tmp_node->arrows[3]=new_win(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP-NODE_WIDTH);
    }
  }
  return 0;
}
