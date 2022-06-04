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
}Character;

typedef struct Bullet
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_b[2];
}bullet;

ALLEGRO_FONT *font1 = NULL;

int bulletready=0,fly=0,num_of_enemy=4,next=0,ti_me=0,mt=10,count_time,num_of_background=0;
//mt是道具持續時間,bulletready判斷主角子彈有沒有飛,fly判斷炸彈有沒有在飛
// the state of character
enum {STOP = 0, MOVE, ATK};

Character chara,ene1,ene2,ene3,ene4;

Character ene[5];



bullet bu1,bu2,bu3,bu4;//子彈(bu1是主角射出的子彈，bu2是攻擊主角的炸藥，bu3，bu4是只有宣告，還沒開始寫的道具)

bullet bu[5];
ALLEGRO_SAMPLE *sample = NULL;


void character_init(){
    font1 = al_load_ttf_font("./font/pirulen.ttf",25,0);
    // load character images

    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_move%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 2 ; i++){
        char temp[50];
        sprintf( temp, "./image/char_atk%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }
    if(num_of_enemy>0){

        char temp[50];
        sprintf( temp, "./image/ene1_move.png");
        ene[1].img_move[0] = al_load_bitmap(temp);

        sprintf( temp, "./image/ene2_move.png");
        ene[2].img_move[0] = al_load_bitmap(temp);

        num_of_enemy-=2;
    }

    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

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
    
    // init enemy 1
    ene[1].width = al_get_bitmap_width(ene[1].img_move[0]);
    ene[1].height = al_get_bitmap_height(ene[1].img_move[0]);
    ene[1].x = 0;
    ene[1].y = HEIGHT/2;

    // init enemy 2
    ene[2].width = al_get_bitmap_width(ene[2].img_move[0]);
    ene[2].height = al_get_bitmap_height(ene[2].img_move[0]);
    ene[2].x = WIDTH-140;
    ene[2].y = HEIGHT;


}
void character_init_next()//下一個場景時多加兩隻怪獸
{
    if(num_of_enemy>0){
        char temp[50];
        sprintf( temp, "./image/ene3.png");
        ene[3].img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
    }
    if(num_of_enemy>0){
        char temp[50];
        sprintf( temp, "./image/ene4.png");
        ene[4].img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
    }
    ene[3].width = al_get_bitmap_width(ene[3].img_move[0]);
    ene[3].height = al_get_bitmap_height(ene[3].img_move[0]);
    ene[3].x = 0;
    ene[3].y = HEIGHT/2;

    ene[4].width = al_get_bitmap_width(ene[4].img_move[0]);
    ene[4].height = al_get_bitmap_height(ene[4].img_move[0]);
    ene[4].x = WIDTH-150;
    ene[4].y = HEIGHT/2;
}



void bullet_init(){
    // load character images
    char temp[50];
    sprintf( temp, "./image/bu1.png");
    bu[1].img_b[0] = al_load_bitmap(temp);
    bu[1].width = al_get_bitmap_width(bu[1].img_b[0]);
    bu[1].height = al_get_bitmap_height(bu[1].img_b[0]);
    bu[1].x = chara.x;
    bu[1].y=chara.y+30;
    bu[1].dir = chara.dir;

}
void enemyatk_init()
{
    char temp[50];
    sprintf( temp, "./image/bu2.png");
    bu[2].img_b[0] = al_load_bitmap(temp);
    // initial the geometric information of character
    bu[2].width = al_get_bitmap_width(bu[2].img_b[0]);
    bu[2].height = al_get_bitmap_height(bu[2].img_b[0]);
    if(chara.dir)
        bu[2].x =WIDTH-1 ;
    else
        bu[2].x = 0+1;
    bu[2].y=chara.y+30;
    bu[2].dir = chara.dir;
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
}

int shot_ene(bullet n,character m)//判斷敵人有沒有被子彈碰到
{
    if(abs(n.x-m.x)<40 && abs(n.y-m.y)<70&& bulletready==1)
    {
        bulletready=0;//現在子彈已經射傷敵人了，這樣分數只會加一分
        return 1; //&& m.y < n.y
    }
    return 0; //for gcc compiler 
}
int been_shot(bullet n,character m)//判斷主角有沒有被子彈碰到
{
    if(abs(n.x-m.x)<50&&abs(n.y-m.y)<70&&fly==1)
    {
        fly=0;//現在子彈已經射傷主角了，這樣hp只會扣一分
        return 1;
    }
    
    return 0;
}
void moving(){
    if(bu[1].x>30&&bu[1].x<WIDTH)
    {
        if(bu[1].dir)
            bu[1].x+=40;
        else
            bu[1].x-=40;
    }
    else
        bulletready=0;//子彈沒在飛
    if(bu[2].x>0&&bu[2].x<(WIDTH/2)&&!(bu[2].dir))
    {
        bu[2].x+=3;
    }
    else if(bu[2].x>(WIDTH/2-50)&&bu[2].x<WIDTH&&(bu[2].dir))
    {
        bu[2].x-=3;
    }
    else
        fly=0;//炸彈沒在飛

    if(ene1.y<-20)
        ene[1].y=HEIGHT;//讓角色回來畫面裡
    else
        ene[1].y-=3;

    if(ene[2].y<-20)
        ene[2].y=HEIGHT;//讓角色回來畫面裡
    else
        ene[2].y-=3;
    if(sc>5)
    {
        if(ene[3].y<20)
            ene[3].y=HEIGHT;//讓角色回來畫面裡
        else
            ene[3].y-=8;

        if(ene[4].y>900)
            ene[4].y=0;//讓角色回來畫面裡
        else
            ene[4].y+=8;
    }


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
        bullet_init();
        bulletready=1;
        if(fly==0){
            enemyatk_init();
            fly=1;
        }
    }
    return ;
}
void charater_update(){
    // use the idea of finite state machine to deal with different state
    if(num_of_background==1)
    {
        num_of_background=0;
    }
    
    // charater been shot
    if(been_shot(bu[2],chara)) hp--;

    // shot enemy
    if(shot_ene(bu[1],ene[1])==1||shot_ene(bu[1],ene[2])==1||shot_ene(bu[1],ene[3])==1||shot_ene(bu[1],ene[4])==1)
    {
        sc=sc+1;
        bulletready=0;

        
    }
    
    if((sc%5)==0&&sc>0)hp=3;//當sc=5會切換場景，自動回血使主角hp=3

     // upgrade scene and difficulty
    if(sc>5&&next==0) {next=1,character_init_next();}//sc指的是分數，sc>5時會跑出另外兩個敵人。
    

    // character movements setting
    moving();
    
    // interpreting what we should do if a keys is press 
    interpreting_keys();
    
    // about main charater's anime settings 
    if( chara.anime == chara.anime_time-1 )
    {
        chara.anime = 0;
        chara.state = STOP;
    }
    if ( chara.anime == 0 )
    {
        chara.state = STOP;
    }
}
void character_draw_next(){
    for(int i=3;i<=4;i++) {
        if(shot_ene(bu[1],ene[i])!=1){

            al_draw_bitmap(ene[i].img_move[0], ene[i].x, ene[i].y, 0);
        }
    }
    
}
void character_draw(){

    count_time++;
    ti_me=count_time/10;

    if(mt>0) mt-=1;

    if(sc>5)character_draw_next();

    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 1,0,"score: %d",sc);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),750, 1,0,"hp: %d",hp);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 475,0,"time: %d",ti_me);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),650, 475,0,"game props:%d",mt);//計算道具剩餘時間
    //void al_draw_textf(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,float x, float y, int flags,const char *format, ...)
    
    
    for(int i=1;i<=2;i++){
        if(shot_ene(bu[1],ene[i])!=1){
            al_draw_bitmap(ene[i].img_move[0], ene[i].x, ene[i].y, 0);
        }
    } 
    
   
    if(bu[1].x>25&&bu[1].x<960)
    {
        if(bu[1].dir)
            al_draw_bitmap(bu[1].img_b[0], bu[1].x, bu[1].y, 0);
        else
            al_draw_bitmap(bu[1].img_b[0], bu[1].x, bu[1].y, ALLEGRO_FLIP_HORIZONTAL);
    }
    if(bu[2].x>0&&bu[2].x<(WIDTH/2-20)&&!(bu[2].dir))
    {
            al_draw_bitmap(bu[2].img_b[0], bu[2].x, bu[2].y, 0);

    }
    else if(bu[2].x>(WIDTH/2-50)&&bu[2].x<WIDTH&&(bu[2].dir))
    {
            al_draw_bitmap(bu[2].img_b[0], bu[2].x, bu[2].y, ALLEGRO_FLIP_HORIZONTAL);
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
void character_destory(){

    for(int i=0;i<=1;i++){
        al_destroy_bitmap(chara.img_atk[i]);
        al_destroy_bitmap(chara.img_move[i]);
    }
    for(int i=1;i<=4;i++) al_destroy_bitmap(ene[i].img_move[0]);

    for(int i=1;i<=2;i++) al_destroy_bitmap(bu[i].img_b[0]);

    al_destroy_sample_instance(chara.atk_Sound);

}
