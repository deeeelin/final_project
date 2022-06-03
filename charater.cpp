#include "charater.h"
#include "scene.h"

ALLEGRO_FONT *font1 = NULL;

int bulletready=0,fly=0,num_of_enemy=4,next=0,ti_me=0,mt=10,count_time;
//mt�O�D�����ɶ�,bulletready�P�_�D���l�u���S����,fly�P�_���u���S���b��
// the state of character
enum {STOP = 0, MOVE, ATK};

int num_of_background=0;

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
Character chara,ene1,ene2,ene3,ene4;
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
        ene1.img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
    }
    if(num_of_enemy>0){
        char temp[50];
        sprintf( temp, "./image/ene2.png");
        ene2.img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
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

    ene1.width = al_get_bitmap_width(ene1.img_move[0]);
    ene1.height = al_get_bitmap_height(ene1.img_move[0]);
    ene1.x = 0;
    ene1.y = HEIGHT/2;

    ene2.width = al_get_bitmap_width(ene2.img_move[0]);
    ene2.height = al_get_bitmap_height(ene2.img_move[0]);
    ene2.x = WIDTH-140;
    ene2.y = HEIGHT;


}
void character_init_next()//�U�@�ӳ����ɦh�[�Ⱖ���~
{
    if(num_of_enemy>0){
        char temp[50];
        sprintf( temp, "./image/ene3.png");
        ene3.img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
    }
    if(num_of_enemy>0){
        char temp[50];
        sprintf( temp, "./image/ene4.png");
        ene4.img_move[0] = al_load_bitmap(temp);
        num_of_enemy--;
    }
    ene3.width = al_get_bitmap_width(ene3.img_move[0]);
    ene3.height = al_get_bitmap_height(ene3.img_move[0]);
    ene3.x = 0;
    ene3.y = HEIGHT/2;

    ene4.width = al_get_bitmap_width(ene4.img_move[0]);
    ene4.height = al_get_bitmap_height(ene4.img_move[0]);
    ene4.x = WIDTH-150;
    ene4.y = HEIGHT/2;
}


typedef struct Bullet
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_b[2];
}bullet;
bullet bu1,bu2,bu3,bu4;//�l�u(bu1�O�D���g�X���l�u�Abu2�O�����D�������ġAbu3�Abu4�O�u���ŧi�A�٨S�}�l�g���D��)
void bullet_init(){
    // load character images
    char temp[50];
    sprintf( temp, "./image/bu1.png");
    bu1.img_b[0] = al_load_bitmap(temp);
    bu1.width = al_get_bitmap_width(bu1.img_b[0]);
    bu1.height = al_get_bitmap_height(bu1.img_b[0]);
    bu1.x = chara.x;
    bu1.y=chara.y+30;
    bu1.dir = chara.dir;

}
void enemyatk_init()
{
    char temp[50];
    sprintf( temp, "./image/bu2.png");
    bu2.img_b[0] = al_load_bitmap(temp);
    // initial the geometric information of character
    bu2.width = al_get_bitmap_width(bu2.img_b[0]);
    bu2.height = al_get_bitmap_height(bu2.img_b[0]);
    if(chara.dir)
        bu2.x =WIDTH-1 ;
    else
        bu2.x = 0+1;
    bu2.y=chara.y+30;
    bu2.dir = chara.dir;
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
int collapse(bullet n,character m)//�P�_�ĤH���S���Q�l�u�I��
{
    if(abs(n.x-m.x)<40 && abs(n.y-m.y)<70&& bulletready==1)
    {
        bulletready=0;//�{�b�l�u�w�g�g�˼ĤH�F�A�o�ˤ��ƥu�|�[�@��
        return 1; //&& m.y < n.y
    }
    return 0; // for gcc compiler defalut 
}
int collapse2(bullet n,character m)//�P�_�D�����S���Q�l�u�I��
{
    if(abs(n.x-m.x)<50&&abs(n.y-m.y)<70&&fly==1)
    {
        fly=0;//�{�b�l�u�w�g�g�˥D���F�A�o��hp�u�|���@��
        return 1;
    }

    return 0;
}

void charater_update(){
    // use the idea of finite state machine to deal with different state
    if(num_of_background==1)
    {
        num_of_background=0;
    }
    if(bu1.x>30&&bu1.x<WIDTH)
    {
        if(bu1.dir)
            bu1.x+=40;
        else
            bu1.x-=40;
    }
    else
        bulletready=0;//�l�u�S�b��
    if(bu2.x>0&&bu2.x<(WIDTH/2)&&!(bu2.dir))
    {
        bu2.x+=3;
    }
    else if(bu2.x>(WIDTH/2-50)&&bu2.x<WIDTH&&(bu2.dir))
    {
        bu2.x-=3;
    }
    else
        fly=0;//���u�S�b��
    if(collapse(bu1,ene1)==1||collapse(bu1,ene2)==1||collapse(bu1,ene3)==1||collapse(bu1,ene4)==1)
    {
        sc=sc+1;
        bulletready=0;
        if((sc%5)==0&&sc>0)//��sc=5�|���������A�۰ʦ^��ϥD��hp=3
            hp=3;
    }

    if(collapse2(bu2,chara))
        hp--;
    if(sc>5&&next==0)
        next=1,character_init_next();//sc�����O���ơAsc>5�ɷ|�]�X�t�~��ӼĤH�C

    if(ene1.y<20)
        ene1.y=HEIGHT;//������^�ӵe����
    else
        ene1.y-=3;

    if(ene2.y<20)
        ene2.y=HEIGHT;//������^�ӵe����
    else
        ene2.y-=3;
    if(sc>5)
    {
        if(ene3.y<20)
            ene3.y=HEIGHT;//������^�ӵe����
        else
            ene3.y-=8;

        if(ene4.y>900)
            ene4.y=0;//������^�ӵe����
        else
            ene4.y+=8;
    }
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
        if(fly==0)
        {
            enemyatk_init();
            fly=1;
        }
    }else if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }else if ( chara.anime == 0 ){
        chara.state = STOP;
    }
}
void character_draw_next(){

    al_draw_bitmap(ene3.img_move[0], ene3.x, ene3.y, 0);
    al_draw_bitmap(ene4.img_move[0], ene4.x, ene4.y, 0);
}
void character_draw(){

    count_time++;
    ti_me=count_time/10;
    if(mt>0)
        mt-=1;
    if(sc>5)
        character_draw_next();
    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 1,0,"score: %d",sc);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),750, 1,0,"hp: %d",hp);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 475,0,"time: %d",ti_me);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),650, 475,0,"game props:%d",mt);//�p��D��Ѿl�ɶ�
    //void al_draw_textf(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,float x, float y, int flags,const char *format, ...)
    al_draw_bitmap(ene1.img_move[0], ene1.x, ene1.y, 0);
    al_draw_bitmap(ene2.img_move[0], ene2.x, ene2.y, 0);
    if(bu1.x>25&&bu1.x<960)
    {
        if(bu1.dir)
            al_draw_bitmap(bu1.img_b[0], bu1.x, bu1.y, 0);
        else
            al_draw_bitmap(bu1.img_b[0], bu1.x, bu1.y, ALLEGRO_FLIP_HORIZONTAL);
    }
    if(bu2.x>0&&bu2.x<(WIDTH/2-20)&&!(bu2.dir))
    {
            al_draw_bitmap(bu2.img_b[0], bu2.x, bu2.y, 0);

    }
    else if(bu2.x>(WIDTH/2-50)&&bu2.x<WIDTH&&(bu2.dir))
    {
            al_draw_bitmap(bu2.img_b[0], bu2.x, bu2.y, ALLEGRO_FLIP_HORIZONTAL);
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
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_bitmap(chara.img_move[0]);
    al_destroy_bitmap(chara.img_move[1]);
    al_destroy_bitmap(ene1.img_move[0]);
    al_destroy_bitmap(ene2.img_move[0]);
    al_destroy_bitmap(ene3.img_move[0]);
    al_destroy_bitmap(ene4.img_move[0]);
    al_destroy_bitmap(bu1.img_b[0]);
    al_destroy_bitmap(bu2.img_b[0]);
    al_destroy_sample_instance(chara.atk_Sound);

}
