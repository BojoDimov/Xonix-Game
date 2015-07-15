#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_color.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include<iostream>
#include<cstdlib>
#include<queue>
#include<cstring>
#include<string>
#include<fstream>
#include"Enemy.h"

#if       _WIN32_WINNT < 0x0500
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT   0x0500
#endif
#include<windows.h>
using namespace std;
const int screenSizeW = 1024;
const int screenSizeH = 640;
const int speed = 2;
const int cube = 8 ;
const int mapSize = 300 ;
const int FPS = 60;
int position_x = (screenSizeW-mapSize*2)/2 ;
int position_y = (screenSizeH-mapSize*2)/2 ;
int map_x = (screenSizeW-mapSize*2)/2;
int map_y  = (screenSizeH-mapSize*2)/2;
// 0 is for empty , 1 is for my territory , 2 is for player , 3 is for trail , 4 is for enemy
enum Map{ EMPTY , TERRITORY , ME  , ENEMY , MARKED , TRAIL};


void string_to_char(string text ,char char_container[])
{
    int ln = text.size();
    for(int i = 0 ; i<= ln ;i++)
    {
        char_container[i] = text[i] ;
    }
    char_container[ln+1] = '\0';
}
void extractScores(string lines[10] , int container[10] , int numLines)
{
    int degree;
    for(int i = 0 ; i < numLines - 1 ;i++)
    {

        degree = 1;
        for(int j = lines[i].size() - 1 ; true ;j-- )
        {
			char c = lines[i][j];
			if(c == ' ')break;
           container[i] += (c - '0')*degree;
           degree*=10;
        }
    }
}



void draw_cube(int x , int y , ALLEGRO_COLOR color)
{
	al_draw_filled_rectangle(x , y , x + cube , y + cube , color);
}
void clear_map(int map[mapSize][mapSize]){
	for(int i = 0 ; i < mapSize ; i++){
		for(int  j = 0 ;  j < mapSize ; j++){
			if(map[i][j] == TRAIL)map[i][j] = EMPTY;
			if(map[i][j] == ME)map[i][j] == TERRITORY ;
		}
	}
}
void make_map(int map[mapSize][mapSize] , long int& score){
	for(int i = 0 ; i < mapSize ; i++){
		for(int  j = 0 ;  j < mapSize ; j++){
			if(i<=10 || j<= 10 || i>= mapSize - cube || j>= mapSize - cube){
				map[i][j] = TERRITORY;
				score++;
			}
			else map[i][j] = EMPTY;

		}
	}

}

void print_map(int map[mapSize][mapSize]){
	for(int i = 0 ; i < mapSize ; i++){
		for(int  j = 0 ;  j < mapSize ; j++){
			cout<<map[i][j]<<" ";
		}
		cout<<endl;
	}
}
void draw_map(int map[mapSize][mapSize] , ALLEGRO_COLOR terr  , ALLEGRO_COLOR enemy){
	for(int i =  0 ; i < mapSize ; i++){
		for(int j = 0 ; j < mapSize ; j++){
			if(map[i][j] == TERRITORY || map[i][j] == TRAIL){
				al_draw_filled_rectangle(map_x + j*2 ,map_y + i*2 , map_x + (j+1)*2 , map_y + (i+1)*2 , terr );
			}
			if(map[i][j] == ENEMY){
				al_draw_filled_circle(map_x + j*2 ,map_y + i*2 , cube/2 , enemy );
			}
		}
	}

}
void flood_fill(int map[mapSize][mapSize] , int ey , int ex){
	queue<int> x;
	queue<int> y;
	x.push(ex);
	y.push(ey);
	int temp_x , temp_y;
	while(!x.empty()){
		temp_x = x.front();
		temp_y = y.front();
		x.pop();
		y.pop();
		if(map[temp_y - 1][temp_x] == EMPTY){
			x.push(temp_x);
			y.push(temp_y - 1);
			map[temp_y - 1][temp_x] = MARKED;
		}
		if(map[temp_y][temp_x - 1] == EMPTY){
			x.push(temp_x - 1);
			y.push(temp_y );
			map[temp_y ][temp_x - 1] = MARKED;
		}
		if(map[temp_y + 1][temp_x] == EMPTY){
			x.push(temp_x);
			y.push(temp_y + 1);
			map[temp_y + 1][temp_x] = MARKED;
		}
		if(map[temp_y][temp_x + 1] == EMPTY){
			x.push(temp_x + 1);
			y.push(temp_y);
			map[temp_y ][temp_x + 1] = MARKED;
		}
	}
}
void make_terr(int map[mapSize][mapSize] , ALLEGRO_COLOR terr , long int& score)
{
	for(int i = 0 ; i < mapSize ; i++){
		for(int j = 0 ; j < mapSize ; j++){
			if(map[i][j] == EMPTY){
				map[i][j]  = TERRITORY;
				al_draw_filled_rectangle(map_x + j*2 ,map_y + i*2 , map_x + (j+1)*2 , map_y + (i+1)*2 , terr );
				score++;
			}
			if(map[i][j] == MARKED)map[i][j] = EMPTY;
			if(map[i][j] == TRAIL){
				map[i][j]  = TERRITORY;
				al_draw_filled_rectangle(map_x + j*2 ,map_y + i*2 , map_x + j*2 + cube  , map_y + i*2 + cube , terr );
				score++;
			}
		}
	}
	al_flip_display();
}
void Menu(ALLEGRO_FONT *font)
{
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 500,350 , ALLEGRO_ALIGN_CENTRE,"Press SPACE to start");
	al_flip_display();
}
void leveldone(ALLEGRO_FONT *font , int score)
{
	char buffer[33];
	itoa(score,buffer,10);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 500,200 , ALLEGRO_ALIGN_CENTRE,"Level done");
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 400,300 , ALLEGRO_ALIGN_CENTRE,"SCORE : ");
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 600,300 , ALLEGRO_ALIGN_INTEGER,buffer);
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 500,450 , ALLEGRO_ALIGN_CENTRE,"Press any key to continue . . . ");
	al_flip_display();
}
void end(ALLEGRO_FONT *font , int score)
{
	char buffer[33];
	itoa(score,buffer,10);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 500,200 , ALLEGRO_ALIGN_CENTRE,"YOU LOST ! ");
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 400,300 , ALLEGRO_ALIGN_CENTRE,"SCORE : ");
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 600,300 , ALLEGRO_ALIGN_INTEGER,buffer);
	al_draw_text(font , al_map_rgb(43 , 224 , 224), 500,450 , ALLEGRO_ALIGN_CENTRE,"Press SPACE to continue . . . ");
	al_flip_display();
}
void Highscores(ALLEGRO_FONT *fontingame  )
{
	ALLEGRO_FONT *superBig = al_load_font("3Dumb.ttf" ,48 ,NULL );
	ALLEGRO_FONT *smaller = al_load_font("3Dumb.ttf" ,24 ,NULL );
	al_clear_to_color(al_map_rgb(0,0,0));
	ifstream myfile ("Highscores.txt" , ios::in);
	int lineCounter = 0;
	char container[50];
	if (myfile.is_open())
	 {
		 al_draw_text(superBig,al_map_rgb(43 , 224 , 224),500,50,ALLEGRO_ALIGN_CENTRE,"HIGHSCORES");
		 al_draw_line(250,100,750,100,al_map_rgb(43 , 224 , 224),2);
		 while ( myfile.getline(container , 50) )
			{

				 al_draw_text(fontingame,al_map_rgb(43 , 224 , 224),500,105 + 50*lineCounter,ALLEGRO_ALIGN_CENTRE,container);
				 lineCounter++;
			}
		 al_draw_text(smaller,al_map_rgb(43 , 224 , 224),500,610,ALLEGRO_ALIGN_CENTRE,"press any key to continue. . .");
		 al_flip_display();
		myfile.close();
	}
}
void insertScore(int score , ALLEGRO_FONT *font )
{
	string line;
  ifstream myfile ("Highscores.txt" , ios::in);
  int lineCounter = 0;
  string fileLines[10];
  int container[10];
  for(int i = 0 ; i < 10 ; i++)
  {
      container[i] = 0;
  }
  if (myfile.is_open())
  {
    while ( getline (myfile,fileLines[lineCounter]) )
    {
      lineCounter++;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
  extractScores(fileLines,container ,lineCounter);
	bool scorePlaced = false;
	char name[15];
    ofstream writeInFile;
    writeInFile.open("Highscores.txt" , ios::out);
    for(int i = 0; i<lineCounter -1 ; i++)
    {
        if(score>=container[i] && !scorePlaced)
        {
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font,al_map_rgb(43 , 224 , 224),150,200,NULL,"New high score please enter your ");
			al_draw_text(font,al_map_rgb(43 , 224 , 224),320,250,NULL," name in the console");
			ShowWindow (GetConsoleWindow(), SW_SHOW);
			al_flip_display();
			cout<<"Name: ";
			cin.getline(name , 15);
			ShowWindow (GetConsoleWindow(), SW_HIDE);
            writeInFile<<name<<" "<<score<<endl;
            lineCounter--;
            scorePlaced = true;
        }
         writeInFile<<fileLines[i]<<endl;
    }
    writeInFile<<fileLines[lineCounter - 1];
	if(scorePlaced)Highscores(font);
    writeInFile.close();

}
int main()
{
	ShowWindow (GetConsoleWindow(), SW_HIDE);
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();

	ALLEGRO_DISPLAY		*window;
	ALLEGRO_TIMER		*timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();//опашка от събития
	window = al_create_display(screenSizeW,screenSizeH);
	al_set_window_title(window,"XONIX");
	al_register_event_source(event_queue,al_get_keyboard_event_source());
	al_register_event_source(event_queue,al_get_timer_event_source(timer));
	al_register_event_source(event_queue,al_get_display_event_source(window));
	al_reserve_samples(2);

	ALLEGRO_COLOR screen	= al_map_rgb (0 , 0 , 0 );
	ALLEGRO_COLOR my_terr	= al_map_rgb(43 , 224 , 224);
	ALLEGRO_COLOR player	= al_map_rgb(232 , 232 , 212);
	ALLEGRO_COLOR track		= al_map_rgb(166 , 12 , 102);
	ALLEGRO_COLOR enemy		= al_map_rgb( 253 , 176 , 52);
	ALLEGRO_COLOR boost		= al_map_rgb(242 , 53 , 53);
	al_clear_to_color(screen);
	ALLEGRO_FONT *font = al_load_font("3Dumb.ttf",48,NULL);
	ALLEGRO_FONT *fontingame = al_load_font("3Dumb.ttf",24,NULL);

	//ALLEGRO_SAMPLE *song ;
	//song = al_load_sample("Trance_River_Flows_In_You_HD.ogg");

	ALLEGRO_SAMPLE *loserSong = al_load_sample("Sad_Trombone-Joe_Lamb-665429450.wav");
	ALLEGRO_SAMPLE *fill_terr_song = al_load_sample("242501__gabrielaraujo__powerup-success.wav");
	ALLEGRO_SAMPLE *winLevel = al_load_sample("162395__zut50__yay.wav");
	//ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
	bool sounds = true;

	/*al_set_sample_instance_playmode(songInstance , ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance ,al_get_default_mixer());*/

	/*keyboard direction reg and speed*/
	enum Direction {UP, DOWN , RIGHT, LEFT,BOOST};
	int movement_x = 0 , movement_y = 0  , direction = 15;
	bool is_moving = false , draw = false ;
	int pl_x = 0 , pl_y = 0 , localScore= 0 , localScore1 =0;
	Enemy enemy1;
	/*Game variables ~ there will be special class*/
	int map[mapSize][mapSize] ; long int  score = 0 ;


	/* ALLEGRO COLORS and some other fancy stuff */


	bool EOG = false , pressing_key = false , ingame = false;
	bool on_my_terr = true ;
	bool is_dead = false;
	bool playSong = true;
	bool hardMode = false;
	int target_score = 70;
	int global_score = 0;
	bool level = false;
	char container[33];

	bool  startgame = false;


	//al_play_sample_instance(songInstance);

	/*Game loop starts */
	al_start_timer(timer);
	while(!EOG ){
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		while( !startgame && !EOG)
		{
		Menu(font);
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if(events.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(events.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				EOG = true;
				break;
			case ALLEGRO_KEY_H:
				Highscores(font);
				while(true)
				{
					ALLEGRO_EVENT events;
					al_wait_for_event(event_queue, &events);
					if(events.type == ALLEGRO_EVENT_KEY_DOWN)break;

				}
				Menu(font);
			case ALLEGRO_KEY_SPACE:
				startgame = true;
				al_clear_to_color(al_map_rgb(0,0,0));
			position_x = (screenSizeW-mapSize*2)/2 ;
			position_y = (screenSizeH-mapSize*2)/2 ;
			pl_x = 0 ; pl_y = 0;
			score = 0;
			hardMode = false;
			target_score = 75;
			is_dead = false;
			global_score = 0;
			make_map(map , score);
			enemy1.release_enemy(0);
			map[enemy1.y][enemy1.x] = ENEMY;
			draw_map(map , my_terr , enemy );
			al_draw_text(fontingame , my_terr,15 , 40 ,NULL,"Target ");
			al_draw_text(fontingame , my_terr ,130 ,40 , NULL ,itoa(target_score , container ,10));
			al_draw_text(fontingame , my_terr ,155 ,40 , NULL ,"%");
			al_draw_text(fontingame , my_terr ,15, 70 , NULL ,"Surface  ");
			al_draw_text(fontingame , my_terr ,130 ,70 , NULL ,itoa(score/910 , container ,10));
			al_draw_text(fontingame , my_terr ,155 ,70 , NULL ,"%");
			al_draw_text(fontingame , my_terr ,15,  100, NULL ,"Score  ");
			al_draw_text(fontingame , my_terr ,120 ,100 , NULL ,itoa(global_score , container ,10));
			al_draw_text(fontingame , my_terr ,15 ,130 ,NULL ,"Hard Mode OFF");
			al_draw_text(fontingame , my_terr ,15 ,600 ,NULL ,"Sound ON");
			draw_cube(position_x , position_y , player);
			al_flip_display();
				break;
			/*case ALLEGRO_KEY_M:
				if(playSong)
				{
					al_stop_sample_instance(songInstance);
				}
				break; */
			}
		}

		}

		if(score / 910 >=target_score)
		{
			level = true;
			if(sounds)al_play_sample(winLevel,1.0,1.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
			leveldone(font , global_score);
			position_x = (screenSizeW-mapSize*2)/2 ;
			position_y = (screenSizeH-mapSize*2)/2 ;
			pl_x = 0 ; pl_y = 0;
			score = 0;
			hardMode = false;
			if(target_score < 95)target_score+=2;
			make_map(map , score);
			enemy1.release_enemy(0);
			map[enemy1.y][enemy1.x] = ENEMY;
			al_flip_display();
			//al_rest(2);
		    bool keypres = false;
			while(!keypres)
			{
				ALLEGRO_EVENT events;
				al_wait_for_event(event_queue, &events);
				if(events.type == ALLEGRO_EVENT_KEY_DOWN){
					switch(events.keyboard.keycode){
					case ALLEGRO_KEY_ESCAPE:
						EOG = true;
					default: keypres = true;
					}
				}
			}
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(fontingame , my_terr,15 , 40 ,NULL,"Target ");
			al_draw_text(fontingame , my_terr ,130 ,40 , NULL ,itoa(target_score , container ,10));
			al_draw_text(fontingame , my_terr ,155 ,40 , NULL ,"%");
			al_draw_text(fontingame , my_terr ,15, 70 , NULL ,"Surface  ");
			al_draw_text(fontingame , my_terr ,130 ,70 , NULL ,itoa(score/910 , container ,10));
			al_draw_text(fontingame , my_terr ,155 ,70 , NULL ,"%");
			al_draw_text(fontingame , my_terr ,15,  100, NULL ,"Score  ");
			al_draw_text(fontingame , my_terr ,120 ,100 , NULL ,itoa(global_score , container ,10));
			if(sounds)al_draw_text(fontingame , my_terr ,15 ,600 ,NULL ,"Sound ON");
			else al_draw_text(fontingame , my_terr ,15 ,600 ,NULL ,"Sound OFF");
			al_draw_text(fontingame , my_terr ,15 ,130 ,NULL ,"Hard Mode OFF");
			draw_map(map , my_terr , enemy );
			draw_cube(position_x , position_y , player);


		}

		if(is_dead)
		{
			if(sounds)al_play_sample(loserSong,1.0 ,1.0 ,1.0 ,ALLEGRO_PLAYMODE_ONCE , 0);
			global_score += score /910 ;
			end(font , global_score);
			insertScore(global_score , font);
			bool startnew = false;
			while(!startnew && !EOG)
			{
				ALLEGRO_EVENT events;
				al_wait_for_event(event_queue, &events);
				if(events.type == ALLEGRO_EVENT_KEY_DOWN){
					switch(events.keyboard.keycode){
					case ALLEGRO_KEY_ESCAPE:
						EOG = true;
						break;
					case ALLEGRO_KEY_SPACE:
						startnew = true;
					}
				}
			}
			if(startnew){
				startgame = false;
			}
		}


		if(events.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(events.keyboard.keycode){
			case ALLEGRO_KEY_W:
				is_moving = true ;pressing_key = true;
				direction = UP ;
				break ;
			case ALLEGRO_KEY_S:
				is_moving = true ;pressing_key = true;
				direction = DOWN ;
				break;
			case ALLEGRO_KEY_A:
				is_moving = true ;pressing_key = true;
				direction = LEFT ;
				break ;
			case ALLEGRO_KEY_D:
				is_moving = true ;pressing_key = true;
				direction = RIGHT;
				break ;
			case ALLEGRO_KEY_ESCAPE:
				EOG = true;
				break;
			/*case ALLEGRO_KEY_M:
				if(playSong)
				{
					al_stop_sample_instance(songInstance);
				}
				break; */
			case ALLEGRO_KEY_N:
				if(sounds)
				{
					sounds = false;
					al_draw_filled_rectangle(0,500,200,640,screen);
					al_draw_text(fontingame , my_terr ,15 ,600 ,NULL ,"Sound OFF");
					al_flip_display();
					break;
				}
				else
				{
					sounds = true;
					al_draw_filled_rectangle(0,500,200,640,screen);
					al_draw_text(fontingame , my_terr ,15 ,600 ,NULL ,"Sound ON");
					al_flip_display();
					break;
				}
			case ALLEGRO_KEY_G:
				if(hardMode)
				{
					hardMode = false;
					al_draw_filled_rectangle(0,130,200,200,screen);
					al_draw_text(fontingame , my_terr ,15 ,130 ,NULL ,"Hard Mode OFF");
					al_flip_display();
					break;

				}
				else
				{
					hardMode = true;
					al_draw_filled_rectangle(0,130,200,200,screen);
					al_draw_text(fontingame , my_terr ,15 ,130 ,NULL ,"Hard Mode ON");
					al_flip_display();
					break;
				}
			default: is_moving = false;
			}
		}

		else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			EOG = true;

		if(events.type == ALLEGRO_EVENT_KEY_UP && on_my_terr){
			pressing_key = false;
		}



		if(events.type == ALLEGRO_EVENT_TIMER){


			/*Enemy movement */
			al_draw_filled_circle(map_x + enemy1.x*2 ,map_y + enemy1.y*2 , cube/2 , screen );
			map[enemy1.y][enemy1.x] = EMPTY;
			if(!on_my_terr && hardMode)
			{
				enemy1.chase(pl_x,pl_y);
			}
			enemy1.check_walls(map);
				enemy1.move();

			al_draw_filled_circle(map_x + enemy1.x*2 ,map_y + enemy1.y*2 , cube/2 , enemy );
			if(map[enemy1.y][enemy1.x] == ME || map[enemy1.y][enemy1.x] == TRAIL)is_dead = true;
			map[enemy1.y][enemy1.x ] = ENEMY;
			al_flip_display();
			/* Enemy movement */

			switch(direction){
			case UP:
				if(pressing_key){
					if(pl_y - 1 >= 0){
						movement_y = -1*speed;
					}
				}
				break ;
			case DOWN:
				if(pressing_key){
					if(pl_y + 1 < mapSize-4){
						movement_y = speed;
					}
				}
				break ;
			case LEFT:
				if(pressing_key){
					if(pl_x  - 1 >= 0){
						movement_x = -1*speed;
					}
				}
				break ;
			case RIGHT:
				if(pressing_key){
					if(pl_x + 1 < mapSize-4){
						movement_x = speed;
					}
				}
				break ;
			}
			draw = true ;
		}
		int temp_x = 0 , temp_y  =0;
		if(draw){
			if(movement_x != 0 || movement_y != 0){
				if(!on_my_terr){
					draw_cube(position_x , position_y , track);
					map[pl_y][pl_x] = TRAIL;
				}
				else{
					draw_cube(position_x , position_y , my_terr);
					map[pl_y][pl_x] == TERRITORY;
				}
				position_x += movement_x ;
				position_y += movement_y ;
				pl_x += movement_x /2 ;
				pl_y += movement_y /2 ;
				if(map[pl_y][pl_x] == EMPTY){
					on_my_terr = false;
				}
				if(map[pl_y][pl_x] == TERRITORY && !on_my_terr){
					on_my_terr = true;
					if(sounds)al_play_sample(fill_terr_song,1.0 ,1.0 ,1.0 ,ALLEGRO_PLAYMODE_ONCE , 0);
					localScore = score / 910;
					flood_fill(map, enemy1.y ,enemy1.x);
					make_terr(map , my_terr , score);
					localScore1 = score / 910;
					global_score+=localScore1 - localScore;
					if(hardMode)
					{
						global_score+=(localScore1 - localScore)*4;

					}
					al_draw_filled_rectangle(0 ,0 ,200 , 125 ,screen);
					al_draw_text(fontingame , my_terr,15 , 40 ,NULL,"Target ");
					al_draw_text(fontingame , my_terr ,130 ,40 , NULL ,itoa(target_score , container ,10));
					al_draw_text(fontingame , my_terr ,155 ,40 , NULL ,"%");
					al_draw_text(fontingame , my_terr ,15, 70 , NULL ,"Surface  ");
					al_draw_text(fontingame , my_terr ,130 ,70 , NULL ,itoa(score/910 , container ,10));
					al_draw_text(fontingame , my_terr ,155 ,70 , NULL ,"%");
					al_draw_text(fontingame , my_terr ,15,  100, NULL ,"Score  ");
					al_draw_text(fontingame , my_terr ,120 ,100 , NULL ,itoa(global_score , container ,10));
					al_flip_display();
					pressing_key = false;
				}
				if(map[pl_y][pl_x] == TRAIL && !on_my_terr){
				//	cout<<"Ouch!"<<endl;
					is_dead = true;
				}
				if(!on_my_terr)
				{
					map[pl_y][pl_x] = ME ;
				}
				movement_x = 0;
				movement_y = 0;
				draw_cube(position_x , position_y , player);
				al_flip_display();
			}
		}
	}
	return 0;
}
