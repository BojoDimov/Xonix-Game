#include "Enemy.h"
#include<ctime>
#include<random>
#include<cmath>
Enemy::Enemy()
{
	x = -1;
	oldx = -1;
	y = -1;
	oldy = -1;
	type = -1;
}

void Enemy::release_enemy(int what)
{
	type = what;
	srand(time(0));
	if(type == 0){
		x = rand()%200 + 11;
		y = rand()%200 + 11;
	}
	if(type == 1){
		x = rand()%289 + 11;
		y = 295;
	}
	direction = rand()%4;
}
void Enemy::check_walls(int map[][300])
{
	
	if(direction == 0 && map[y-7][x-7] == 1){
		if((map[y-7 ][x-7 -1] == 1 || map[y-7][x-7 +1] == 1) && map[y-7 +1][x-7] == 0){
			direction = 2;
			return ;
			
		}
		if((map[y-7 -1][x-7] == 1 || map[y-7 +1][x-7] == 1) && map[y-7][x-7+1] == 0) {
			direction = 1;
			return ;
		}
	}
	if(direction == 1 && map[y-7][x+7] == 1){
		if((map[y-7][x+7 -1] == 1 || map[y-7][x+7 +1] == 1) && map[y-7+1][x+7] == 0){
			direction = 3 ;
			return ;
		}
		if((map[y-7 -1][x+7] == 1 || map[y-7 +1][x+7] == 1) && map[y-7][x+7-1] == 0){
			direction = 0 ;
			return ;
		}
	}
	if(direction == 2 && map[y+7][x-7] == 1){
		if((map[y+7 -1][x-7] == 1 || map[y+7 +1][x-7] == 1) && map[y+7][x-7 +1] == 0){
			direction = 3 ;
			return ;
		}
		if((map[y+4][x-7 -1] == 1 || map[y+4][x-7 +1] == 1) && map[y+4 -1][x-7] == 0){
			direction = 0 ;
			return ;
		}
		
	}
	if(direction == 3 && map[y+5][x+5] == 1){
		if(map[y+5 -1][x+5 ] == 1 && map[y+5 +1][x+5] == 1){
			direction = 2;
			return ;
		}
		if(map[y+5][x+5 -1] == 1 && map[y+5][x+5 +1] == 1){
			direction = 1;
			return ;
		}
		
	}
}
void Enemy::move()
{
	oldx = x;
	oldy = y;

	if(direction == 0){
		x--;
		y--;
	}
	if(direction == 1){
		x++;
		y--;
	}
	if(direction == 2){
		x--;
		y++;
	}
	if(direction == 3){
		x++;
		y++;
	}
}

void Enemy::chase(int what_x , int what_y){
	float dist[4];
	dist[0] = sqrt(static_cast<float>((what_x-(x-1))*(what_x - (x-1)) + (what_y - (y-1) )*(what_y - (y-1))));
	dist[1] = sqrt(static_cast<float>((what_x-(x+1))*(what_x - (x+1)) + (what_y - (y-1) )*(what_y - (y-1))));
	dist[2] = sqrt(static_cast<float>((what_x-(x-1))*(what_x - (x-1)) + (what_y - (y+1) )*(what_y - (y+1))));
	dist[3] = sqrt(static_cast<float>((what_x-(x+1))*(what_x - (x+1)) + (what_y - (y+1) )*(what_y - (y+1))));
	float min = dist[0];
	for(int i =0 ; i < 4 ; i++)
	{
		if(dist[i]<=min)
		{
			min = dist[i];
			direction = i; 
		}
	}

}

Enemy::~Enemy(void)
{
}
