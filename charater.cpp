#include "charater.h"
#include "scene.h"

typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[2];
    ALLEGRO_BITMAP *img_atk[2];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int active;
}Character;

typedef struct Bullet
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_b[2];
    int active;
}bullet;


ALLEGRO_FONT *font1 = NULL;

int num_of_enemy=4,next=0,ti_me=0,mt=10,count_time,num_of_background=0;
//mt是道具持續時間,bulletready判斷主角子彈有沒有飛,fly判斷炸彈有沒有在飛
// the state of character
enum {STOP = 0, MOVE, ATK};

Character chara={0},ene[5]={0};

//子彈(bu1是主角射出的子彈，bu1-9，b10是攻擊主角的炸藥

bullet bu[11]={0};

ALLEGRO_SAMPLE *sample = NULL;


void character_init(){

    char temp[50];

    // load font 
    font1 = al_load_ttf_font("./font/pirulen.ttf",25,0);

    // load character images
    for(int i = 1 ; i <=2 ; i++){
        sprintf( temp, "./image/char_move%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);
    }
    for(int i=1;i<=2;i++){
        sprintf( temp, "./image/char_atk%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }
    for(int i=1;i<=4;i++){
        sprintf( temp, "./image/ene%d_move.png",i);
        ene[i].img_move[0] = al_load_bitmap(temp);
    }

    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = WIDTH/2-100;
    chara.y = HEIGHT/2;
    chara.dir = false;

    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30;
    
    // init enemy picture size
    for(int i=1;i<=4;i++){
        ene[i].width = al_get_bitmap_width(ene[i].img_move[0]);
        ene[i].height = al_get_bitmap_height(ene[i].img_move[0]);
    }

    // init enemy position
    ene[1].x = 10;
    ene[1].y = HEIGHT/2;
    ene[1].active=1;

    ene[2].x = WIDTH-140;
    ene[2].y = HEIGHT;
    ene[2].active=1;

    ene[3].x = 0;
    ene[3].y = HEIGHT/2;
    ene[3].active=0;

    ene[4].x = WIDTH-150;
    ene[4].y = HEIGHT/2;
    ene[4].active=0;


    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    return ;
}

void bullet_init(){
    
    char temp[50];

    // set chara bullet picture size
    for(int i=1;i<=9;i++){
        sprintf( temp, "./image/bu%d.png",1);
        bu[i].img_b[0] = al_load_bitmap(temp);
        bu[i].width = al_get_bitmap_width(bu[i].img_b[0]);
        bu[i].height = al_get_bitmap_height(bu[i].img_b[0]);
        bu[i].x = chara.x;
        bu[i].y=chara.y;
        bu[i].dir = chara.dir;
        bu[i].active=0;
    }
   

    for(int i=10;i<=10;i++){
        sprintf( temp, "./image/bu%d.png",2);
        bu[i].img_b[0] = al_load_bitmap(temp);
        if(chara.dir) bu[i].x =WIDTH-1 ;
        else bu[i].x = 0+1;
        bu[i].y=chara.y+30;
        bu[i].dir = chara.dir;
        bu[i].active=0;
    }

    return ;

}

void charater_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    return ;
}

void shot_ene()//判斷敵人有沒有被子彈碰到
{
    for(int i=1;i<=4;i++){
        for(int j=1;j<=9;j++){
            if(abs( bu[j].x-ene[i].x)<40 && abs(bu[j].y-ene[i].y)<70 && bu[j].active==1)
            {
            ene[i].active=0;
            bu[j].active=0; // bullet hide
            bu[j].x=chara.x; // bullet back to chara
            bu[j].y=chara.y+30;
            bu[j].dir = chara.dir;
            sc++; ;//現在子彈已經射傷敵人了，這樣分數只會加一分
            return; 
            }
        }

    }
    return;  //for gcc compiler 
}

void been_shot()//判斷主角有沒有被子彈碰到
{
    for(int i=10;i<=10;i++){
        if(abs(bu[i].x-chara.x)<50&&abs(bu[i].y-chara.y)<70&& bu[i].active==1)
        {
            bu[i].active=0; // bullet hide
            hp--; //現在子彈已經射傷主角了，這樣hp只會扣一分
            return;
        }
    }
    return ;
}

void bullet_active(){ // show new bullet 
    int i=1;
    while(bu[i].active==1 && i<9){
        i++;
    }
    bu[i].active=1;
    return ;
}

void ene_active(int next){
    if(next==1){
        for(int i=3;i<=4;i++){
            ene[i].active=1;
        }
    }
    return ;
}

void things_moving(){
     // chara bullets that is active will keep flying 
    for(int i=1;i<=9;i++){

        if(bu[i].x>30&&bu[i].x<WIDTH && bu[i].active==1){
            if(bu[i].dir) bu[i].x+=40;
            else bu[i].x-=40;
        }
        else if( (bu[i].x<30 || bu[i].x>WIDTH) && bu[i].active==1){
            bu[i].active=0; // bullet out of screen ,so hide
            bu[i].x=chara.x; // bullet back to chara 
            bu[i].y=chara.y;
            bu[i].dir = chara.dir;
        }
    }

    // enemy bullets keep flying 
    if(bu[10].x>0&&bu[10].x<(WIDTH/2)&&!(bu[10].dir && bu[10].active==1)) bu[10].x+=3;
    
    else if(bu[10].x>(WIDTH/2-50)&&bu[10].x<WIDTH&&(bu[10].dir && bu[10].active==1)) bu[10].x-=3;
    

    // active slow and fast enemy go back and forth
    for(int i=1;i<=4;i++){
        if(ene[i].y<-20)
            ene[i].y=HEIGHT;//讓角色回來畫面裡
        else
            ene[i].y-=3;
    }
    return ;
    
}

void interpreting_keys(){
    if( key_state[ALLEGRO_KEY_W] ){
        chara.y -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        //chara.x -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_S] ){
        chara.y += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        //chara.x += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_SPACE] ){   
        chara.state = ATK;
        bullet_active();
        key_state[ALLEGRO_KEY_SPACE]=false;
    }
    // bullet that is hidden ,goes with character
    for(int i=1;i<=9;i++){
        if(bu[i].active==0){
            bu[i].x=chara.x;
            bu[i].y=chara.y;
        }
    }
    return ;
}

void charater_update()
{
    // use the idea of finite state machine to deal with different state
    if(num_of_background==1)
    {
        num_of_background=0;
    }
    
    // charater been shot
    been_shot();

    //shot enemy
    shot_ene();
     // upgrade scene and difficulty

    if((sc%5)==0&&sc>0)hp=3;//當sc=5會切換場景，自動回血使主角hp=3
    if(sc>5&&next==0){next++; ene_active(next);}//sc指的是分數，sc>5時會跑出另外兩個敵人。
    
    // update the movement of things 
    things_moving();
    
    // interpreting what we should do if a keys is press 
    interpreting_keys();
    
    // about main charater's anime settings 
    if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }
    if ( chara.anime == 0 ){
        chara.state = STOP;
    }
    return;
    
}

void character_draw(){ //checked
    

    count_time++; 
    ti_me=count_time/10; 

    if(mt>0) mt-=1;

    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 1,0,"score: %d",sc);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),750, 1,0,"hp: %d",hp);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 475,0,"time: %d",ti_me);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),650, 475,0,"game props:%d",mt);//計算道具剩餘時間
    //void al_draw_textf(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,float x, float y, int flags,const char *format, ...)
    
    // draw enemy if active 
    for(int i=1;i<=4;i++){
        if(ene[i].active==1){
            al_draw_bitmap(ene[i].img_move[0], ene[i].x, ene[i].y, 0);
        }     
    } 
    
    // draw bullets if active
    for(int i=1;i<=10;i++){
        if(bu[i].active==1){
            if(bu[i].dir) al_draw_bitmap(bu[i].img_b[0], bu[i].x, bu[i].y, 0);

            else al_draw_bitmap(bu[i].img_b[0], bu[i].x, bu[i].y, ALLEGRO_FLIP_HORIZONTAL);

        }
    }
    
    // with the state, draw corresponding image
    if( chara.state == STOP ){
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y,0);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
            }
        }
    }else if( chara.state == ATK ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, 0);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
                al_play_sample_instance(chara.atk_Sound);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk_Sound);
            }
        }

    }
}

void character_destory(){ // checked 

    for(int i=0;i<=1;i++){
        al_destroy_bitmap(chara.img_atk[i]);
        al_destroy_bitmap(chara.img_move[i]);
    }
    for(int i=1;i<=4;i++) al_destroy_bitmap(ene[i].img_move[0]);

    for(int i=1;i<=10;i++) al_destroy_bitmap(bu[i].img_b[0]);

    al_destroy_sample_instance(chara.atk_Sound);

}
