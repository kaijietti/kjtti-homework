#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

//游戏中横向为y，竖向为x 
//整条蛇为snake，除蛇头外，每一节蛇的下一节为snake_nextx与snake_nexty 
//标记为restart为重置游戏用，记录初始状态
//标记为playgame为游戏进行时记录蛇位置用
 
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define SNAKE_FOOD '$'
#define WALL_CELL '*'

char if_start_game = 'y';//用于判断是否是用户意愿停止游戏 
int if_end_game = 0;//用于判断是否是撞墙或者自食而停止游戏 
int food[2] = {0,0};
char map_restart[12][13] = {//初始状态地图 
	"************",      //可用于重置游戏
	"*XXXXH *   *",
	"*      *   *",
	"*  ***     *",
	"*     * *  *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"************"
};
int headx = 1,heady = 5,tailx = 1,taily = 1;//创建表示蛇头、蛇尾的坐标变量 
char map_playgame[12][13];//游戏中用于记录蛇、食物等坐标的“游戏时 ”地图

char snake_nextx_restart[12][12] = {//初始状态时用于记录除蛇头外每一节蛇的下一节的x坐标 
	{0,0,0,0,0,0,0,0,0,0,0,0},   //可用于重置游戏 
	{0,1,1,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0}
};
char snake_nextx_playgame[12][12];//游戏时用于记录除蛇头外每一节蛇的下一节的x坐标  

char snake_nexty_restart[12][12] = {//初始状态时用于记录除蛇头外每一节蛇的下一节的y坐标
	{0,0,0,0,0,0,0,0,0,0,0,0},   //可用于重置游戏
	{0,2,3,4,5,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
char snake_nexty_playgame[12][12];//游戏时用于记录除蛇头外每一节蛇的下一节的y坐标

void print_map_playgame();//打印地图 
void put_food(int *food);//投放食物 
void snake_move(int dx,int dy,int *food);//蛇移动 
void restart();//重置游戏
void playgame();//进行游戏 

int main() { 
	
	while(if_start_game) {
		restart();//重置游戏 ，在此处选择是否运行游戏，y运行，其余键退出 
		playgame();//玩游戏 
	}
	return 0;
}

void print_map_playgame() {
	int i;
	for(i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
    
} 
void put_food(int *food){    //  随机投放食物 

    srand(time(0)); //  初始化伪随机
    int x,y;
    while (1) { //  一直循环随机直到出现可行坐标 
        x=rand() % 12; y=rand() % 12;    //  随机一个坐标 
        if (map_playgame[x][y]==BLANK_CELL) {    //  若此坐标为空白格 
            map_playgame[x][y] = SNAKE_FOOD ;   //  改为食物 
            food[0] = x;
    		food[1] = y;
            return;     //  跳出 
        }  
    }
    
}
void snake_move(int dx,int dy,int *food) {//蛇移动 
    
	int is_eat = 0;//bool变量判断是否吃到食物
	int x,y;//用于暂存坐标 
	if(map_playgame[headx + dx][heady + dy] == SNAKE_FOOD) {//判断头部进行move时是否会碰到食物 
		is_eat = 1;//如果吃到食物则更新bool 
	}
	if(!is_eat) {//如果没吃到食物，尾巴将往下一节移动 
		usleep(400000);
		map_playgame[tailx][taily] = BLANK_CELL;//尾部变成空格
		x = snake_nextx_playgame[tailx][taily];
		y = snake_nexty_playgame[tailx][taily];//获取新蛇尾的坐标 
		tailx = x; taily = y;
	}
	if (map_playgame[headx+dx][heady+dy] == WALL_CELL || map_playgame[headx+dx][heady+dy] == SNAKE_BODY) {  
	     // 判断蛇头移动至的位置是否墙或蛇身 
        if_end_game = 1; //  若是墙或蛇身则游戏结束 
        return; 
    }
    //除了撞墙，无论是否进食，头部均会移动
    x = headx + dx ;
	y = heady + dy ;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//更新离头部最近的一节蛇的下一节的坐标，即头部坐标 
	map_playgame[x][y] = SNAKE_HEAD;//移动头部 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//更新头部的坐标
	if(is_eat) {
		put_food(food);
	} 
} 
void restart() {//重置游戏 

	memcpy(map_playgame,map_restart,sizeof(map_restart));
	
    memcpy(snake_nextx_playgame,snake_nextx_restart,sizeof(snake_nextx_restart));
    
    memcpy(snake_nexty_playgame,snake_nexty_restart,sizeof(snake_nexty_restart));
    
    headx = 1,heady = 5,tailx = 1,taily = 1;if_end_game = 0;//重置蛇头蛇尾的位置,并且更新end变量 
    
	printf("Welcome to Greedy Snake!\n");
	printf("Do you want to start?\ny for yes;n for no\n");
	scanf("%c",&if_start_game);
	getchar(); //将上一次scanf输入后的回车清除 
	if(if_start_game != 'y') exit(1); //除了'y',其余键退出 
}
void playgame() {//进行游戏

	printf("\033[2J");//清屏 
	put_food(food);//投放食物 
	printf("x = %d y = %d\n",food[0],food[1]);

	print_map_playgame();//打印初始地图 
	system("pause");
	char direction = 'd';//初始方向为'd',即朝向右
	while(!if_end_game) {
		int possible_move[4] = {1,1,1,1};
		printf("\033[2J");//清屏 
		int xx[4] = {-1,0,1,0};
		int yy[4] = {0,-1,0,1};
		for(int i = 0;i < 4;i ++) {
			if(map_playgame[headx+xx[i]][heady+yy[i]] == WALL_CELL || map_playgame[headx+xx[i]][heady+yy[i]] == SNAKE_BODY)
			possible_move[i] = 0;
		}
		
		int cur_distance[4];
		
		cur_distance[0] = abs(headx - 1 - food[0]) + abs(heady - food[1]);//w
		cur_distance[1] = abs(headx - food[0]) + abs(heady - 1 - food[1]);//a
		cur_distance[2] = abs(headx + 1 - food[0]) + abs(heady - food[1]);//s
		cur_distance[3] = abs(headx - food[0]) + abs(heady + 1 - food[1]);//d
		
		char next_direction;
		int k;
		for(k = 0;k < 4;k ++) {
			if(possible_move[k] == 1) break;
		}
		int min = cur_distance[k];
		int min_pos = k;
		for(int i = 0;i < 4;i ++) {
			if( possible_move[i] == 1 && cur_distance[i] < min) 
			{
				min = cur_distance[i];
				min_pos = i;
			}
		}
		
		
		switch(min_pos) {
			
			case 0: next_direction = 'w';break;
			case 1: next_direction = 'a';break;
			case 2: next_direction = 's';break;
			case 3: next_direction = 'd';break;
			
		}
		
		
		direction = next_direction;
		
		switch(direction) {
			
			case 'w': snake_move(-1,0,food);break;
			case 'a': snake_move(0,-1,food);break;
			case 's': snake_move(1,0,food);break;
			case 'd': snake_move(0,1,food);break;
			
		}
		print_map_playgame();//打印进行一次snake_move后的地图 

	}
	printf("\033[2J");//跳出上述循环时，游戏结束，清屏 
	
}
