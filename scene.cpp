#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *background2 = NULL;
ALLEGRO_BITMAP *win = NULL;
ALLEGRO_BITMAP *lose= NULL;
// function of menu
void menu_init(){
    font = al_load_ttf_font("./font/pirulen.ttf",30,0);
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
}
void menu_draw(){
    al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    //al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, HEIGHT/2, al_map_rgb(255, 255, 255), 0);

}
void menu_destroy(){
    al_destroy_font(font);
}

// function of game_scene
void game_scene_init(){
    character_init();
    background = al_load_bitmap("./image/stage.jpg");
    background2 = al_load_bitmap("./image/stage2.png");
    lose = al_load_bitmap("./image/lose.jpg");
    win= al_load_bitmap("./image/win.jpg");
}
void game_scene_draw(){
    if(hp<=0)
    {
         al_draw_bitmap(lose, 0, 0, 0);
    }
    else if(sc>=10)
    {
         al_draw_bitmap(win, 0, 0, 0);
    }
    else if(sc<5)
    {
        al_draw_bitmap(background, 0, 0, 0);
         character_draw();

    }
    else
    {
        al_draw_bitmap(background2, 0, 0, 0);
        character_draw();

    }
}



void game_scene_destroy(){
    al_destroy_bitmap(background);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(win);
    al_destroy_bitmap(lose);
    character_destory();
}
