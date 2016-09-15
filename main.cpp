#include "spis.h"
#define TICK_DELAY 1000000
int y, x = 0;
bool cursorVisible = false;

void editLoop();
void *runtimeInputLoop(void *ptr);
void runtimeLoop();
void redrawContent();

int main(int argc, char *argv[]){
	std::ifstream file;

	if(argc>=2)
		file.open(argv[1]);

	setlocale(LC_ALL, "");
	initscr();
	signal(SIGWINCH, NULL);
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

	curs_set(!cursorVisible);

  refresh();

  get_code(&file, grid);
	initSystem(2, 2);

	vector<int> nums;
	for (int i = 0; i < 20; i++) {
		nums.push_back(i + 5);
	}

	listInput lIn = listInput(128, 1, "IN.A", 20, nums);
	lIn.initArrow(0, 0);

	inputs.push_back(&lIn);
	gridArrows.push_back(lIn.getArrow());

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
	curs_set(1);
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

  while (state == RUNNING) {
    redrawContent();
    usleep(TICK_DELAY);
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

  redrawContent();
  pthread_cancel(*thread);
  return;
}

void *runtimeInputLoop(void *ptr) {
	MEVENT event;

	curs_set(0);
	while (true) {
	  runtimeSystemInput(event);
	}
}

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
