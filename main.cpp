#include "spis.h"
#include <signal.h>
#include <unistd.h>
#define NODE_HEIGHT 11
#define ARROW_WIDTH 1
#define ARROW_HEIGHT 1
#define GAP_WIDTH_H 1
#define GAP_WIDTH_V 0
#define NODE_WIDTH 25
#define CODE_WIDTH 20


pthread_t inputThread;

int grid_size[2] = {4,4};
std::vector<node> grid;

void drawContent();
void inputLoop();
void *runtimeInputLoop(void *ptr);
bool pointInWindow(WINDOW *win, int x, int y);


WINDOW *new_bwin(int height, int width, int starty, int startx){
  WINDOW *win;
  win=newwin(height, width, starty, startx);
  box(win,0,0);
  wrefresh(win);
  return win;
}
void redraw(int n){
  int x, y=0, max_x, max_y;
  int nID=0;

  endwin();
  refresh();
  clear();
  
  printf("%d %d %d %d\n", x,y,max_x,max_y);
  for(int i=0;i<grid_size[0]; i++){
    x=GAP_WIDTH_H;
    for(int j=0;j<grid_size[1];j++){
      werase(grid[nID].w_main);
      werase(grid[nID].w_code);
      werase(grid[nID].w_reg);
      refresh();
      for(int k =0; k<4;k++){
	if(grid[nID].arrows[k]){
	  werase(grid[nID].arrows[k]->win);
	  grid[nID].arrows[k]->win=newwin(ARROW_HEIGHT,ARROW_WIDTH,y+NODE_HEIGHT/2,x-GAP_WIDTH_H-NODE_WIDTH);
	}
	}
      grid[nID].w_main=new_bwin(NODE_HEIGHT, NODE_WIDTH, y, x);
      grid[nID].w_code=newwin(NODE_HEIGHT - 2, CODE_WIDTH - 2, y + 1 , x + 1);
      new_bwin(NODE_HEIGHT, CODE_WIDTH, y, x);
      grid[nID].w_reg =newwin(NODE_HEIGHT-2,NODE_WIDTH-CODE_WIDTH-2, y+1, x+CODE_WIDTH+1);
      grid[nID].inputCode.push_back("");
      
      wprintw(grid[nID].w_reg, "ACC%d\nBAK%d", grid[nID].acc, grid[nID].bak);
      wrefresh(grid[nID].w_reg);
      
      x=x+NODE_WIDTH+ARROW_WIDTH+2*GAP_WIDTH_H;
      refresh();
      }
    y=y+(NODE_HEIGHT+2*GAP_WIDTH_V+ARROW_WIDTH);
    nID++;
  }
  refresh();
  drawContent();
  return;
}
int main(int argc, char *argv[]){
  int c,x=GAP_WIDTH_H, y=0, id;
  int max_x, max_y;
  node*tmp_node;
  int nID=0;
  WINDOW *menu;
  std::ifstream file;
  /*    if(argc!=2){
	printf("No file specified! Exiting.\n");
	return 1;
	}*/
  file.open(argv[1]);
  
  /*if(!file.is_open()){
    printf("Couldn't open file! Exiting.\n");
    return 1;
    }*/
  initscr();
  signal(SIGWINCH, redraw);
  getmaxyx(stdscr, max_y, max_x);
  y=0;
  refresh();
  for(int i=0;i<grid_size[0]; i++){
    x=GAP_WIDTH_H;
    for(int j=0;j<grid_size[1];j++){
      tmp_node = new node(nID);
      nID++;
      tmp_node->w_main=new_bwin(NODE_HEIGHT, NODE_WIDTH, y, x);
      tmp_node->w_code=newwin(NODE_HEIGHT - 2, CODE_WIDTH - 2, y + 1 , x + 1);
      new_bwin(NODE_HEIGHT, CODE_WIDTH, y, x);
      tmp_node->w_reg =newwin(NODE_HEIGHT-2,NODE_WIDTH-CODE_WIDTH-2, y+1, x+CODE_WIDTH+1);
      if(tmp_node->inputCode.size()==0)
	tmp_node->inputCode.push_back("");
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
    //int err = pthread_create( &inputThread, NULL, inputLoop, NULL);
  endwin();
  if(get_code(&file, grid)){
    printf("%d\n", get_code(&file, grid));
    return 1;
  }
  drawContent();
  inputLoop();
  getch();
  endwin();
  return 0;
}

void drawNode(int nodeIndex) {
	node *tmp_node = &grid[nodeIndex];
	werase(tmp_node->w_code);
	for(int y = 0; y < tmp_node->inputCode.size(); y++) {
	  if(tmp_node->w_code)
	    mvwprintw(tmp_node->w_code, y, 0, tmp_node->inputCode[y].c_str());
	  else
	    return;
	}
	wrefresh(tmp_node->w_code);
}

void drawContent() {
	for(int i = 0; i < grid.size(); i++) {
		drawNode(i);
	}
}

void inputLoop() {
	MEVENT event;
	int y = 0;
	int x = 0;

	int selectedNode = 0;
	int selectedLine = 0;
	int selectedIndex = 0;

  noecho();
  cbreak();
	mousemask(ALL_MOUSE_EVENTS, NULL);
	keypad(stdscr, TRUE);

	while(true) {
	  int input = getch();
		if ((input >= 65 && input <= 90) || (input >= 97 && input <= 122)
				|| (input >= 48 && input <= 57) || input == 44
				|| input == 32) {
			if (input >= 97) {
				input -= 32;
			}

			if (grid[selectedNode].inputCode[selectedLine].length() < 17) {
				grid[selectedNode].inputChar(selectedLine, selectedIndex, static_cast<char>(input));
				selectedIndex++;
				x++;
				move(y, x);
				drawNode(selectedNode);
			}
		} else if (input == KEY_ENTER || input == 11 || input == 10) {
			if (grid[selectedNode].inputCode.size() < NODE_HEIGHT - 2) {
				grid[selectedNode].newLine(selectedLine, selectedIndex);
				x -= selectedIndex;
				y++;
				selectedIndex = 0;
				selectedLine++;
				move(y, x);
				drawNode(selectedNode);
			}
		} else if(input == KEY_BACKSPACE || input == 127) {
			// BACKSPACE
			int tmpLength = grid[selectedNode].inputCode[selectedLine].length();
		  int status = grid[selectedNode].backspace(selectedLine, selectedIndex);


		  switch(status) {
		  case 1:
		  	selectedIndex--;
		  	x--;
		  	break;
		  case 2:
		  	selectedLine--;
		  	y--;
				selectedIndex = grid[selectedNode].inputCode[selectedLine].length() - tmpLength;
				x = getbegx(grid[selectedNode].w_code) + selectedIndex;
				break;
		  }

		  drawNode(selectedNode);
		  move(y, x);

		} else if (input == KEY_MOUSE && getmouse(&event) == OK) {

		  //if (event.bstate & BUTTON1_RELEASED) {
		  for (int i = 0; i < grid.size(); i++) {
		    if (pointInWindow(grid[i].w_code, event.x, event.y)) {
		      int begY, begX = 0;
				  getbegyx(grid[i].w_code, begY, begX);
				  
				  selectedNode = i;
				  selectedLine = event.y - begY;
				  selectedIndex = event.x - begX;
				  
				  if (selectedLine >= grid[i].inputCode.size()) {
				    selectedLine = grid[i].inputCode.size() - 1;
				    
				  }
				  
				  if (selectedIndex >= grid[i].inputCode[selectedLine].length()) {
				    selectedIndex = grid[i].inputCode[selectedLine].length();
				  }
				  
				  y = selectedLine + begY;
				  x = selectedIndex + begX;
				  
				  move(y, x);
				  break;
				  				}
			}
			//}
		} else {
			//mvwprintw(stdscr, 0, 0, to_string(input).c_str());
			return;
		}
	}
}

bool pointInWindow(WINDOW *win, int x, int y) {
	int begX, begY, maxX, maxY = 0;

	getbegyx(win, begY, begX);
	if (x < begX || y < begY) {
		return false;
	}

	getmaxyx(win, maxY, maxX);
	if (x > begX + maxX || y > begY + maxY) {
		return false;
	}

	return true;
}

void *runtimeInputLoop(void *ptr) {
	// TODO
}
