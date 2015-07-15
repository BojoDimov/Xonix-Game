#pragma once
#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_color.h>
class Enemy
{
private:
	
public:
	int x;
	int y;
	int oldx;
	int oldy;
	int type; // -1 for not released | 0 for empty territory | 1 for player territory
	int direction; // 0 - upper left | 1 - upper right | 2 - down left | 3 - down right
	Enemy();
	~Enemy(void);
	void release_enemy(int what);
	void move();
	void check_walls(int map[][300]);
	void chase(int what_x , int what_y);
	int getX();
	int getY();
};

