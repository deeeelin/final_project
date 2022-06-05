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
    int active; // when active=1 show in the screen,if active =0 then hide
}Character;

typedef struct Bullet
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_b[2];
    int active; // when active=1 show in the screen,if active =0 then hide
}bullet;


ALLEGRO_FONT *font1 = NULL;

int num_of_enemy=4,next=0,ti_me=0,mt=10,count_time,num_of_background=0; //(num_of_enemy didn't use)

//srand(time(NULL)); // set random seed 

// the state of character
enum {STOP = 0, MOVE, ATK}; 

// define main character and enemies
Character chara={0},ene[5]={0};

// define main character bullets and enemies
bullet bu_m[11]={0},bu_e[11]={0};

ALLEGRO_SAMPLE *sample = NULL;
// init main character and enemies

void font_init(){
     // load font 
    font1 = al_load_ttf_font("./font/normalfont.otf",40,0);
    return ;
}

void chara_init(){

    char temp[50];

   
    for(int i = 1 ; i <=2 ; i++){

         // load character images
        sprintf( temp, "./image/char_move%d.png", i );
        chara.img_move[i-1] = al_load_bitmap(temp);

        sprintf( temp, "./image/char_atk%d.png", i );
        chara.img_atk[i-1] = al_load_bitmap(temp);
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
    
    
    return ;
}

void ene_init(){

    char temp[50];

   
    for(int i=1;i<=4;i++){
        
    // load enemy images
        sprintf( temp, "./image/ene%d_move.png",i);
        ene[i].img_move[0] = al_load_bitmap(temp);
    // init enemy picture size

        ene[i].width = al_get_bitmap_width(ene[i].img_move[0]);
        ene[i].height = al_get_bitmap_height(ene[i].img_move[0]);
    }

    // init enemy position
    ene[1].x = 10;
    ene[1].y = HEIGHT/2;
    ene[1].active=1;
    ene[1].dir=true; //down

    ene[2].x = WIDTH-140;
    ene[2].y = HEIGHT;
    ene[2].active=1;
    ene[2].dir=false; // up

    ene[3].x = 0;
    ene[3].y = 0-20;
    ene[3].active=0;

    ene[4].x = WIDTH-150;
    ene[4].y = HEIGHT+20;
    ene[4].active=0;
    
    return ;
    
}

void sound_init(){
    
    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    return ;
}
// initialize main character's bullets and enemy bullets
void bullet_init(){
    
    char temp[50];

    // load chara bullet's picture
    for(int i=1;i<=9;i++){
        sprintf( temp, "./image/bu%d.png",1);
        bu_m[i].img_b[0] = al_load_bitmap(temp);
        bu_m[i].width = al_get_bitmap_width(bu_m[i].img_b[0]);
        bu_m[i].height = al_get_bitmap_height(bu_m[i].img_b[0]);
        bu_m[i].x = chara.x;
        bu_m[i].y=chara.y+30;
        bu_m[i].dir = chara.dir;
        bu_m[i].active=0;
    }
   
    // load enemy bullet's(bomb) picture,there are only one recently  
    for(int i=1;i<=1;i++){
        sprintf( temp, "./image/bu%d.png",2);
        bu_e[i].img_b[0] = al_load_bitmap(temp);
        if(chara.dir) bu_e[i].x =WIDTH-1 ;
        else bu_e[i].x = 0+1;
        bu_e[i].y=chara.y+30;
        bu_e[i].dir = true;
        bu_e[i].active=1;
    }

    return ;

}

void all_object_init(){
    font_init();
    chara_init();
    ene_init();
    sound_init();
    bullet_init();
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

void shot_ene(){//determine whether the enemy has been shot

    for(int i=1;i<=4;i++){ // four enemies 
        for(int j=1;j<=9;j++){ // nine main character's bullet

            if(abs( bu_m[j].x-ene[i].x)<40 && abs(bu_m[j].y-ene[i].y)<70 && bu_m[j].active==1 && ene[i].active==1)
            {
                ene[i].active=1; // enemy hide
                if(ene[i].dir)ene[i].y=1;
                else ene[i].y=HEIGHT;
                bu_m[j].active=0; // bullet hide

                // bullet back to chara
                bu_m[j].x=chara.x; 
                bu_m[j].y=chara.y+30; 
                bu_m[j].dir = chara.dir; 

                //score plus
                sc++; ;

                return; 
            }
        }

    }
    return; 
}

void been_shot() //determine whether the main character has been shot
{
    for(int i=1;i<=1;i++){ // 1 enemy bullet 

        if(abs(bu_e[i].x-chara.x)<50&&abs(bu_e[i].y-chara.y)<70&& bu_e[i].active==1)
        {
            bu_e[i].active=1; // bullet hide
            if(chara.dir) bu_e[i].x =WIDTH-1;
            else bu_e[i].x = 0+1;
            bu_e[i].y=(rand()%(HEIGHT-0+1)+0);
            bu_e[i].dir = chara.dir;
            hp--; //health point minus 1
            return;
        }
    }
    return ;
}

void bullet_active(){ // show new bullet 

    int i=1;
    while(bu_m[i].active==1 && i<9){ // iterate to the available bullet (bullet that is hidden and not flying)
        i++;
    }
    bu_m[i].active=1; // show this bullet,let it start to fly
    return ;
}

void ene_active(int next){
    if(next==1){
        for(int i=1;i<=4;i++){
            ene[i].active=1; // when scene upgrade let the hidden enemy be shown,and start to move
        }
    }
    return ;
}

void object_moving(){

     // chara bullets that is active will keep flying 
    for(int i=1;i<=9;i++){

        if(bu_m[i].x>30&&bu_m[i].x<WIDTH && bu_m[i].active==1){
            if(bu_m[i].dir) bu_m[i].x+=40;
            else bu_m[i].x-=40;
        }
        else if( (bu_m[i].x<30 || bu_m[i].x>WIDTH) && bu_m[i].active==1){
            bu_m[i].active=0; // bullet out of screen ,so hide
            bu_m[i].x=chara.x; // bullet back to chara 
            bu_m[i].y=chara.y;
            bu_m[i].dir = chara.dir;
        }
    }

    // enemy bullets keep flying 
    for(int i=1;i<=1;i++){
        if(bu_e[i].x>0&&bu_e[i].x<(WIDTH)&& bu_e[i].active==1){
        
            if(bu_e[i].dir) bu_e[i].x+=3;
            else bu_e[i].x-=3;
        }
        else if((bu_e[i].x<=0 || bu_e[i].x>=(WIDTH)) && bu_e[i].active==1){
            if(bu_e[i].dir){bu_e[i].dir=false;bu_e[i].x=WIDTH-1;}
            else {bu_e[i].dir=true;bu_e[i].x=1;}
        }

    }
    

    // active enemy go back and forth
    for(int i=1;i<=4;i++){
        if(ene[i].y>0 &&ene[i].y<(HEIGHT)&& ene[i].active==1){
        
            if(ene[i].dir) ene[i].y+=20;
            else ene[i].y-=20;
        }
         else if((ene[i].y<=0 || ene[i].y>=(HEIGHT)) && ene[i].active==1){
            if(ene[i].dir){ene[i].dir=false;ene[i].y=HEIGHT-1;}
            else {ene[i].dir=true;ene[i].y=1;}
        }
    }
    return ;
    
}

void interpreting_keys(){

    if( key_state[ALLEGRO_KEY_W] ){
        chara.y -= 5;
        chara.state = MOVE;
    }
    else if( key_state[ALLEGRO_KEY_S] ){
        chara.y += 5;
        chara.state = MOVE;
    }
    if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        //chara.x -= 5;
        chara.state = MOVE;
    }
    else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        //chara.x += 5;
        chara.state = MOVE;
    }
    if( key_state[ALLEGRO_KEY_SPACE] ){   
        chara.state = ATK;
        bullet_active(); // if space is pressed ,then active a new bullet 

        /* if space is pressed ,play a sound (dont put this in character_draw 
        or else the sound cannot be match to shooting bullets)*/
        al_play_sample_instance(chara.atk_Sound); 
        
        key_state[ALLEGRO_KEY_SPACE]=false; // if you dont add this,then when you long-press key space ,it will be continous shooting
    }

    // bullet that is hidden ,will goes with character
    for(int i=1;i<=9;i++){
        if(bu_m[i].active==0){
            bu_m[i].x=chara.x;
            bu_m[i].y=chara.y+30;
            bu_m[i].dir=chara.dir;
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
    
    // see if charater been shot
    been_shot();

    //see if enemy been shot
    shot_ene();

     // upgrade scene and difficulty
    if((sc%5)==0&&sc>0)hp=3;//when scene==5 will change background and let hp back to full
    
    if(sc>5&&next==0){next++;ene_active(next);}//sc is the score，sc>5 will have two more enemies
    
    // update the movement of things (except the main character)
    object_moving();
    
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

void character_draw(){ 
    
    // count time for scoring
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
        //printf("%d",ene[2].active);
        if(ene[i].active==1){
            al_draw_bitmap(ene[i].img_move[0], ene[i].x, ene[i].y, 0);
        }     
    } 
    
    // draw main character and enmy bullets if active
    for(int i=1;i<=9;i++){
        if(bu_m[i].active==1){
            if(bu_m[i].dir) al_draw_bitmap(bu_m[i].img_b[0], bu_m[i].x, bu_m[i].y, 0);

            else al_draw_bitmap(bu_m[i].img_b[0], bu_m[i].x, bu_m[i].y, ALLEGRO_FLIP_HORIZONTAL);

        }
    }
    for(int i=1;i<=1;i++){
        if(bu_e[i].active==1){
            if(bu_e[i].dir) al_draw_bitmap(bu_e[i].img_b[0], bu_e[i].x, bu_e[i].y, 0);

            else al_draw_bitmap(bu_e[i].img_b[0], bu_e[i].x, bu_e[i].y, ALLEGRO_FLIP_HORIZONTAL);
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
               
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
            }else{
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y,  ALLEGRO_FLIP_HORIZONTAL);
                
            }
        }

    }
}

void character_destory(){ // destroy created objects

    for(int i=0;i<=1;i++){
        al_destroy_bitmap(chara.img_atk[i]);
        al_destroy_bitmap(chara.img_move[i]);
    }
    for(int i=1;i<=4;i++) al_destroy_bitmap(ene[i].img_move[0]);

    for(int i=1;i<=10;i++) al_destroy_bitmap(bu_m[i].img_b[0]);

    for(int i=1;i<=1;i++) al_destroy_bitmap(bu_e[i].img_b[0]);

    al_destroy_sample_instance(chara.atk_Sound);

    return ;

}

