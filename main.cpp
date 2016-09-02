#include "spis.h"
#include <pthread.h>
#include <locale.h>

#define NODE_HEIGHT 11
#define ARROW_H_WIDTH 4
#define ARROW_H_HEIGHT 4
#define ARROW_V_WIDTH 12
#define ARROW_V_HEIGHT 1
#define GAP_WIDTH_H 1
#define GAP_WIDTH_V 0
#define NODE_WIDTH 26
#define CODE_WIDTH 20
#define BUTTON_WIDTH 5
#define BUTTON_HEIGHT 3


#ifdef _ascii_only
#define H_ARROW "\n---><---"
#define V_ARROW "     ^v"
#else
#define V_ARROW "   ⇧⇩"
#define H_ARROW "\n ⇨\n ⇦"
#endif

pthread_t inputThread;

void drawContent();
void inputLoop();
void runtimeLoop();
bool pointInWindow(WINDOW *win, int x, int y);
WINDOW *new_bwin(int height, int width, int starty, int startx);
string makeThreeDigit(int n);

WINDOW *playButton;
WINDOW *stopButton;
WINDOW *pauseButton;

int  grid_size[2] = {4,4};
std::vector<node> grid;
int  state = 0;
bool stop;

int main(int argc, char *argv[]){
  io tmp_io;
  int c,x=GAP_WIDTH_H, y=0, id;
  int max_x, max_y;
  node *tmp_node;
  int nID=0;
  WINDOW *menu;
  std::ifstream file;

  setlocale(LC_ALL, "");
  file.open(argv[1]);
  initscr();
  start_color();
  init_pair(5, COLOR_WHITE, COLOR_BLACK);
  signal(SIGWINCH, NULL);
  getmaxyx(stdscr, max_y, max_x);
  y=0;
  refresh();
  for(int i=0;i<grid_size[0]; i++){
    x=GAP_WIDTH_H;
    for(int j=0;j<grid_size[1];j++){
      free(tmp_node);
      tmp_node = new node(nID);
      grid.push_back(*tmp_node);
      grid[nID].w_main=new_bwin(NODE_HEIGHT, NODE_WIDTH, y, x);
      grid[nID].w_code=newwin(NODE_HEIGHT - 2, CODE_WIDTH - 2, y + 1 , x + 1);
      new_bwin(NODE_HEIGHT, CODE_WIDTH, y, x);
      grid[nID].w_reg =newwin(NODE_HEIGHT-2,NODE_WIDTH-CODE_WIDTH-2, y+1, x+CODE_WIDTH+1);

      wprintw(grid[nID].w_reg, " ACC%d\n BAK%d", grid[nID].acc, grid[nID].bak);
      wrefresh(grid[nID].w_reg);
      refresh();

      if (i != 0) {
      	grid[nID].arrows[0] = grid[nID - grid_size[1]].arrows[2];
      }

      if (j != grid_size[1] - 1) {
      	grid[nID].arrows[1] = new arrow(nID, nID + 1);
      	grid[nID].arrows[1]->win=newwin(ARROW_H_HEIGHT, ARROW_H_WIDTH, y + floor(NODE_HEIGHT / 2) - floor(ARROW_H_HEIGHT / 2),x + NODE_WIDTH + GAP_WIDTH_H);
      	wprintw(grid[nID].arrows[1]->win, H_ARROW);
				wrefresh(grid[nID].arrows[1]->win);
      }

      if (i != grid_size[0] - 1) {
      	grid[nID].arrows[2] = new arrow(nID, nID + grid_size[0] - 1);
				grid[nID].arrows[2]->win=newwin(ARROW_V_HEIGHT, ARROW_V_WIDTH, y + NODE_HEIGHT + GAP_WIDTH_V, x + floor(NODE_WIDTH / 2) - floor(ARROW_V_WIDTH / 2));
				wprintw(grid[nID].arrows[2]->win, V_ARROW);
				wrefresh(grid[nID].arrows[2]->win);
      }

      if (j != 0) {
      	grid[nID].arrows[3] = grid[nID - 1].arrows[1];
      }
      nID++;
      x = x + NODE_WIDTH + ARROW_H_WIDTH + 2 * GAP_WIDTH_H;
    }
    y=y+(NODE_HEIGHT+2*GAP_WIDTH_V+ARROW_V_HEIGHT);
  }

  x=GAP_WIDTH_H;
  playButton = newwin(BUTTON_HEIGHT, BUTTON_WIDTH, y, x);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  wbkgd(playButton, COLOR_PAIR(2));
  wrefresh(playButton);
  x += BUTTON_WIDTH + GAP_WIDTH_H;
  
  stopButton = newwin(BUTTON_HEIGHT, BUTTON_WIDTH, y, x);
  init_pair(3, COLOR_BLACK, COLOR_RED);
  wbkgd(stopButton, COLOR_PAIR(3));
  wrefresh(stopButton);
  
  x += BUTTON_WIDTH + GAP_WIDTH_H;
  pauseButton = newwin(BUTTON_HEIGHT, BUTTON_WIDTH, y, x);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  wbkgd(pauseButton, COLOR_PAIR(4));
  wrefresh(pauseButton);
 
  get_code(&file, grid);
  
    for(int i=0; i < grid.size(); i++){
      if(grid[i].inputCode.size() == 0)
	grid[i].inputCode.push_back("");
    }

  state = 1;

  noecho();    
  cbreak();
  mousemask(ALL_MOUSE_EVENTS, NULL);
  keypad(stdscr, TRUE);
  inputs.push_back(tmp_io);
  inputs[0].arr = new arrow(-1, 0);
  grid[0].arrows[0]=inputs[0].arr;
  outputs.push_back(tmp_io);
  grid.back().arrows[2]= new arrow(-2, grid.size()-1);
  outputs[0].arr=grid.back().arrows[2];
  inputs[0].get();
  endwin();
  while (state != 0) {
  	switch(state) {
  		case 1:
  			drawContent();
  			inputLoop();
  			break;
  		case 2:
  			runtimeLoop();
  			break;
  		default:
  			state = 1;
  	}
  }
  endwin();
  outputs[0].put();
  return 0;
}

// DRAWING

void drawNode(int nodeIndex) {
  node *tmp_node =&grid[nodeIndex];
  int beg_x, beg_y;
  getbegyx(tmp_node->w_code, beg_y, beg_x);
  if(state==2){
      drawHighlight(nodeIndex);
  }
  else{
    werase(tmp_node->w_code);
    for(int y = 0; y < tmp_node->inputCode.size(); y++)
      mvwprintw(tmp_node->w_code, y, 0, tmp_node->inputCode[y].c_str());
    wrefresh(tmp_node->w_code);
  }
    return;
}
void drawHighlight(int nodeIndex){
  node *tmp_node =&grid[nodeIndex];
  int beg_x, beg_y;
  if(tmp_node->pc==0){
    mvwprintw(tmp_node->w_code, tmp_node->inputCode.size()-1, 0, tmp_node->inputCode[tmp_node->inputCode.size()-1].c_str());
  }
  else{
    mvwprintw(tmp_node->w_code, tmp_node->pc-1, 0, tmp_node->inputCode[tmp_node->pc-1].c_str());
  }
  getbegyx(tmp_node->w_code, beg_y, beg_x);
  werase(tmp_node->w_highlight);
  delwin(tmp_node->w_highlight);
  tmp_node=&grid[nodeIndex];
  start_color();
  tmp_node->w_highlight = newwin(1, CODE_WIDTH - 2, beg_y+tmp_node->pc, beg_x);
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  wattron(tmp_node->w_highlight,COLOR_PAIR(1));
  wbkgd(tmp_node->w_highlight, COLOR_PAIR(1));
  wbkgd(tmp_node->w_code, COLOR_PAIR(5));
  mvwprintw(tmp_node->w_highlight, 0, 0, tmp_node->inputCode[tmp_node->pc].c_str());
  wrefresh(tmp_node->w_code);
  wrefresh(tmp_node->w_highlight);
}

void updateReg(int nodeIndex) {
  node *tmp_node = &grid[nodeIndex];

  mvwprintw(tmp_node->w_reg, 1, 0, makeThreeDigit(tmp_node->acc).c_str());
  mvwprintw(tmp_node->w_reg, 3, 0, makeThreeDigit(tmp_node->bak).c_str());
  wrefresh(tmp_node->w_reg);
}

void updateArrow(int nodeIndex, int arrowIndex) {
	node  *tmp_node  = &grid[nodeIndex];
	arrow *tmp_arrow = grid[nodeIndex].arrows[arrowIndex];
	if(!tmp_arrow){
	  endwin();
	  printf("ERROR: tmp_arrow was NULL!!\n");
	  return;
	}
	if (arrowIndex % 2 == 0) {
		mvwprintw(tmp_arrow->win, 0, 0, makeThreeDigit(tmp_arrow->value[0]).c_str());
		mvwprintw(tmp_arrow->win, 0, 7, makeThreeDigit(tmp_arrow->value[1]).c_str());
	} else {
		mvwprintw(tmp_arrow->win, 0, 0, makeThreeDigit(tmp_arrow->value[0]).c_str());
		mvwprintw(tmp_arrow->win, 3, 0, makeThreeDigit(tmp_arrow->value[1]).c_str());
	}
	wrefresh(tmp_arrow->win);
	return;
}

void updateArrowsForNode(int nodeIndex) {
  if (nodeIndex >= grid_size[1])
  	updateArrow(nodeIndex, 0);

  if ((nodeIndex + 1) % grid_size[0] != 0)
  	updateArrow(nodeIndex, 1);

  if (nodeIndex < grid_size[0] - grid_size[1])
  	updateArrow(nodeIndex, 2);

  if (nodeIndex % grid_size[0] != 0)
  	updateArrow(nodeIndex, 3);
}

void drawContent() {
  for(int i = 0; i < grid.size(); i++) {
    drawNode(i);
    updateReg(i);
    updateArrowsForNode(i);
  }
}

// INPUT AND RUNTIME
void inputLoop() {
	MEVENT event;

	int selectedNode = 0;
	int selectedLine = 0;
	int selectedIndex = 0;

	curs_set(1);

	int y, x = 0;
	getbegyx(grid[selectedNode].w_code, y, x);

	move(y, x);

	while(true) {
	  int input = getch();
		if ((input >= 65 && input <= 90) || (input >= 97 && input <= 122)
				|| (input >= 48 && input <= 57) || input == 44
				|| input == 32 || input == 58) {
			if (input >= 97) {
				input -= 32;
			}

			if (grid[selectedNode].inputCode[selectedLine].length() < CODE_WIDTH - 3) {
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

		} else if (input == KEY_LEFT) {
			if (selectedIndex == 0) {
				if (selectedLine != 0) {
					selectedLine--;
					y--;
					selectedIndex = grid[selectedNode].inputCode[selectedLine].length();
					x = getbegx(grid[selectedNode].w_code) + selectedIndex;

					move(y, x);
				}
			} else {
				selectedIndex--;
				x--;

				move(y, x);
			}
		} else if (input == KEY_RIGHT) {
			if (selectedIndex != grid[selectedNode].inputCode[selectedLine].length()) {
				selectedIndex++;
				x++;

				move(y, x);
			} else if (selectedLine < grid[selectedNode].inputCode.size() - 1) {
				selectedLine++;
				y++;
				selectedIndex = 0;
				x = getbegx(grid[selectedNode].w_code);

				move(y, x);
			}
		} else if (input == KEY_UP) {
			if (selectedLine > 0) {
				selectedLine--;
				y--;
				if (selectedIndex > grid[selectedNode].inputCode[selectedLine].length()) {
					selectedIndex = grid[selectedNode].inputCode[selectedLine].length();
					x = getbegx(grid[selectedNode].w_code) + selectedIndex;
				}
				move(y, x);
			}
		} else if (input == KEY_DOWN) {
			if (selectedLine < grid[selectedNode].inputCode.size() - 1) {
				selectedLine++;
				y++;
				if (selectedIndex > grid[selectedNode].inputCode[selectedLine].length()) {
					selectedIndex = grid[selectedNode].inputCode[selectedLine].length();
					x = getbegx(grid[selectedNode].w_code) + selectedIndex;
				}
				move(y, x);
			}
		} else if (input == KEY_MOUSE && getmouse(&event) == OK) {
			bool pointFound = false;

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
				  pointFound = true;
				  break;
		    }
			}

		  if (!pointFound && pointInWindow(playButton, event.x, event.y)) {
		  	state = 2;
		  	return;
		  } else if (!pointFound && pointInWindow(pauseButton, event.x, event.y)) {
		  	state = 0;
		  	return;
		  }


		} else {
			//mvwprintw(stdscr, 0, 0, to_string(input).c_str());
			//return 0;
		}
	}
}

void *runtimeInputLoop(void *ptr) {
	MEVENT event;

	curs_set(0);
	while (true) {
	  int input = getch();
	  if (getmouse(&event) == OK && pointInWindow(stopButton, event.x, event.y)) {
	    state = 1;
	    return NULL;
	  }
	}
}

void runtimeLoop() {
  pthread_t *thread = new pthread_t;
  int err = pthread_create(thread, NULL, &runtimeInputLoop, NULL);
  
  if(err!=0){
    return;
  }
  for(int i;i<grid.size();i++){
    grid[i].reset();
  }
  //reset output and input and arrows
  while (state == 2) {
    drawContent();
    sleep(1);
    compute_tick();
  }
  drawContent();
  pthread_cancel(*thread);
  printf("Done");
  return;
}

// UTILLITY

WINDOW *new_bwin(int height, int width, int starty, int startx){
  WINDOW *win;
  win=newwin(height, width, starty, startx);
  box(win,0,0);
  wrefresh(win);
  return win;
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

string makeThreeDigit(int n) {
  std::string nToText = "";
  char tmp_str[4];
  if(n>999||n<-999)
    n=999-(n<0)*1998;
  if(n>=0)
    sprintf(tmp_str, " %03d", n);
  else
    sprintf(tmp_str, "%04d", n);
  nToText = string(tmp_str);
  return nToText;
}
