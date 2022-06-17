#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *game_background = NULL;
ALLEGRO_BITMAP *game_background_2 = NULL;
ALLEGRO_BITMAP *menu_background = NULL;
ALLEGRO_BITMAP *win = NULL;
ALLEGRO_BITMAP *lose= NULL;
// function of menu
void menu_init(){
    font = al_load_ttf_font("./font/normalfont.otf",30,0);
    menu_background=al_load_bitmap("./image/menu_background.jpeg");
}

void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
}

void menu_draw(){
    // al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_scaled_bitmap(menu_background,0, 0,al_get_bitmap_width(menu_background),al_get_bitmap_height(menu_background),0, 0,WIDTH,HEIGHT,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    //al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, HEIGHT/2, al_map_rgb(255, 255, 255), 0);

}

void menu_destroy(){

    al_destroy_bitmap(menu_background);
    al_destroy_font(font);
}

// function of game_scene
void game_scene_init(){
    all_object_init();
    game_background = al_load_bitmap("./image/stage.jpg");
    game_background_2 = al_load_bitmap("./image/stage2.png");
    lose = al_load_bitmap("./image/lose.jpg");
    win= al_load_bitmap("./image/win.jpg");
}
void re_init_game(){
    sc=0;
    hp=5;
    next=0;
    ti_me=0;
    mt=10;
    num_of_background=0;
    num_of_enemy=4;
    count_time=0;
    memset(key_state,false,sizeof(key_state));
    return ;
}
void re_game_process(int temp){
    re_init_game();
    while(1){
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if(temp==1) al_draw_scaled_bitmap(lose,0, 0,al_get_bitmap_width(lose),al_get_bitmap_height(lose),0, 0,WIDTH,HEIGHT,0);
        else if(temp==2) al_draw_scaled_bitmap(win,0, 0,al_get_bitmap_width(win),al_get_bitmap_height(win),0, 0,WIDTH,HEIGHT,0);
        al_draw_text(font, al_map_rgb(255,0,0), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' back to menu");
        al_flip_display();
        if( event.type == ALLEGRO_EVENT_KEY_UP){
            if( event.keyboard.keycode == ALLEGRO_KEY_ENTER)return ;    
        }
    }
}

int game_scene_draw(){
    int t=0;
    if(hp<=0)
    {
          t=1;
          al_draw_scaled_bitmap(lose,0, 0,al_get_bitmap_width(lose),al_get_bitmap_height(lose),0, 0,WIDTH,HEIGHT,0);
          
    }
    else if(sc>=10)
    {
        t=2;
         al_draw_scaled_bitmap(win,0, 0,al_get_bitmap_width(win),al_get_bitmap_height(win),0, 0,WIDTH,HEIGHT,0);
    }
    else if(sc<5)
    {
        al_draw_scaled_bitmap(game_background,0, 0,al_get_bitmap_width(game_background),al_get_bitmap_height(game_background),0, 0,WIDTH,HEIGHT,0);
        object_draw();

    }
    else
    {
        al_draw_scaled_bitmap(game_background_2,0, 0,al_get_bitmap_width(game_background_2),al_get_bitmap_height(game_background_2),0, 0,WIDTH,HEIGHT,0);
        object_draw();

    }
    return t;
}



void game_scene_destroy(){
    al_destroy_bitmap(game_background);
    al_destroy_bitmap(game_background_2);
    al_destroy_bitmap(win);
    al_destroy_bitmap(lose);
    object_destroy();
    menu_destroy();
}
