#include "spis.h"
int y, x = 0;
bool cursorVisible = false;
int tickDelay = 1024;
vector<runtimeInput *> runtimeInputs;

void editLoop();
void *runtimeInputLoop(void *ptr);
void runtimeLoop();
void redrawContent();
int main(int argc, char *argv[]){
  std::ifstream file;

  
  signal(SIGWINCH, redrawSystem);

  if(argc>=2)
    file.open(argv[1]);
  
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  
  noecho();
  cbreak();
  mousemask(ALL_MOUSE_EVENTS, NULL);
	keypad(stdscr, TRUE);

  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_RED);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  init_pair(5, COLOR_WHITE, COLOR_BLACK);

  setCursor(false);

  refresh();

  initSystem(2, 2);
  
  
  // TEST INPUTS AND OUTPUTS
  
  vector<int> inNums;
  for (int i = 0; i < 20; i++) {
    inNums.push_back((rand() % 500) + 100);
  }
	
  listInput lIn = listInput(128, 1, "IN.A", 20, inNums);
  lIn.initArrow(0, 0);
  inputs.push_back(&lIn);
  
  vector<int> outNums;
  for (int i = 0; i < 20; i++) {
    outNums.push_back(inNums[i] % 10);
  }
  
  listOutput lOut = listOutput(128, 25, "OU.A", 20, outNums);
  lOut.initArrow(15, 2);
  outputs.push_back(&lOut);
  
  consoleOutput cOut = consoleOutput(145, 25, "OU.B", 20);
  cOut.initArrow(14, 2);
  outputs.push_back(&cOut);
  
  consoleInput cIn = consoleInput(145, 1, "IN.B", 18);
  cIn.initArrow(1, 0);
  inputs.push_back(&cIn);
  runtimeInputs.push_back(&cIn);
	  
  state = EDIT;

  endwin();
  while (state != OFF) {
  	switch(state) {
  		case EDIT:
  			redrawContent();
  			editLoop();
  			break;
  		case RUNNING:
  			runtimeLoop();
  			break;
  		default:
  			state = EDIT;
  	}
  }

  endwin();
  return 0;
}

void redrawContent() {
	drawSystemContent();
}

void editLoop() {
	while (state == EDIT) {
		MEVENT event;
		int input = getch();
		if (systemInput(input, event)) {
			continue;
		}
	}
}

void runtimeLoop() {
  pthread_t *thread = new pthread_t;
  int err = pthread_create(thread, NULL, &runtimeInputLoop, NULL);

  for(unsigned int i=0;i<grid.size();i++)
    grid[i].runPrepare();

  //TODO: reset input and output
  if(err!=0){
    return;
  }

  for(int i=0;i<grid.size();i++){
    grid[i].reset();
  }
  for (int i = 0; i < gridArrows.size(); i++) {
  	gridArrows[i]->reset();
  }
  for (int i = 0; i < inputs.size(); i++) {
  	inputs[i]->reset();
  }
	for (int i = 0; i < outputs.size(); i++) {
		outputs[i]->reset();
	}

  int i = 0;
  while (state == RUNNING) {
    redrawContent();

    for (i = 0; i < tickDelay * 1000; i += STOP_CHECK_DELAY) {
    	usleep(STOP_CHECK_DELAY);
    	if (state != RUNNING)
    		break;
    }

    if (state == RUNNING)
    	usleep((tickDelay * 1000) % STOP_CHECK_DELAY);
    else
    	break;

    compute_tick();
  }

  for(int i=0;i<grid.size();i++){
		grid[i].reset();
	}
	for (int i = 0; i < gridArrows.size(); i++) {
		gridArrows[i]->reset();
	}
	for (int i = 0; i < inputs.size(); i++) {
		inputs[i]->reset();
	}
	for (int i = 0; i < outputs.size(); i++) {
		outputs[i]->reset();
	}

  redrawContent();
  pthread_cancel(*thread);
  return;
}

void *runtimeInputLoop(void *ptr) {
	MEVENT event;
	setCursor(true);
	int s = 0;
	while (true) {
		int input = getch();
		if (input == KEY_MOUSE)
			getmouse(&event);

	  runtimeSystemInput(event, input);
	  for (int i = 0; i < runtimeInputs.size(); i++) {
	  	if (runtimeInputs[(i + s) % runtimeInputs.size()]->processInput(input, event)) {
	  		s = i;
	  		break;
	  	}
	  }
	}
}

WINDOW *new_bwin(int height, int width, int starty, int startx){
  WINDOW *win;
  win=newwin(height, width, starty, startx);
  box(win,0,0);
  wrefresh(win);
  return win;
}

WINDOW *winBorder(int height, int width, int starty, int startx) {
  WINDOW *win;
  win = newwin(height, width, starty, startx);

  WINDOW *tmpWin;
  tmpWin = newwin(height + 2, width + 2, starty - 1, startx - 1);

  box(tmpWin,0,0);
  wrefresh(tmpWin);
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

string intToString (int a) {
	ostringstream tmp;
	tmp << a;
	return tmp.str();
}

void setCursor(bool value) {
	if (cursorVisible != value)
		cursorVisible = value;
		curs_set(cursorVisible);
}
