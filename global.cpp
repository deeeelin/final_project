#include "global.h"

// variables for global usage
const float FPS = 60.0;
const int WIDTH = 960;//400;
const int HEIGHT = 540;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool judge_next_window = false;
int sc=0;
int hp=3;
