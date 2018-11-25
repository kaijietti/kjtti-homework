#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<conio.h>
#include<windows.h>

//游戏中横向为y，竖向为x 
//整条蛇为snake，除蛇头外，每一节蛇的下一节为snake_nextx与snake_nexty 
//标记为restart为重置游戏用，记录初始状态
//标记为playgame为游戏进行时记录蛇位置用
 
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define WALL_CELL '*'

char if_start_game = 'y';//用于判断是否是用户意愿停止游戏 
int if_end_game = 0;//用于判断是否是撞墙或者自食而停止游戏 

char map_restart[12][13] = {//初始状态地图 
	"************",      //可用于重置游戏
	"*XXXXH     *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"************"
};
int headx = 1,heady = 5,tailx = 1,taily = 1;//创建表示蛇头、蛇尾的坐标变量 
char map_playgame[12][13];//游戏中用于记录蛇等坐标的“游戏时 ”地图

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
	{0,0,0,0,0,0,0,0,0,0,0,0}
};
char snake_nexty_playgame[12][12];//游戏时用于记录除蛇头外每一节蛇的下一节的y坐标

void print_map_playgame();//打印地图 
void snake_move(int dx,int dy);//蛇移动 
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
	for(int i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
} 

void snake_move(int dx,int dy) {//蛇移动 
    int x,y; 
	Sleep(100);
	map_playgame[tailx][taily] = BLANK_CELL;//尾部变成空格
	x = snake_nextx_playgame[tailx][taily];
	y = snake_nexty_playgame[tailx][taily];//获取新蛇尾的坐标 
	tailx = x; taily = y;
	
	if (map_playgame[headx+dx][heady+dy] == WALL_CELL || map_playgame[headx+dx][heady+dy] == SNAKE_BODY) {  
	     // 判断蛇头移动至的位置是否墙或蛇身 
        if_end_game = 1; //  若是墙或蛇身则游戏结束 
        return; 
    }
    //除了撞墙，头部均会移动
    x = headx + dx ; y = heady + dy;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//更新离头部最近的一节蛇的下一节的坐标，即头部坐标 
	map_playgame[x][y] = SNAKE_HEAD;//移动头部 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//更新头部的坐标

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

	system("cls");//清屏 
	int if_hit = 0;//判断键盘是否有输入 
	print_map_playgame();//打印初始地图 
	char direction = 'd';//初始方向为'd',即朝向右 
	int first_press = 0;//这是为了防止在为进行游戏前闪屏而设置的bool变量 
	while(!if_end_game) {
		char next_direction ;
		if(kbhit()) {//当键盘有敲击时才给下一次方向赋值 
        	next_direction = getch();
        	first_press = 1;//这里表示有了第一次键盘输入 
        }
		if(next_direction != direction) {//如果下一次方向与当前方向不同，判断下列语句 
			if(next_direction + direction == 'w'+'s' || next_direction + direction == 'a'+'d');
										//如果下一次方向与当前相反，不做改变 
			else direction = next_direction;
										//否则改变方向 
		}//如果下一次方向与当前相同，不做改变 
		switch(direction) {
			case 'w': snake_move(-1,0);break;
			case 'a': snake_move(0,-1);break;
			case 's': snake_move(1,0);break;
			case 'd': snake_move(0,1);break;
		}
		if(first_press) {//当有了第一次键盘输入之后，再清屏打出地图，避免未进行游戏前闪屏 
			system("cls");//清屏 
			print_map_playgame();//打印进行一次snake_move后的地图
		} 
	}
	system("cls");//跳出上述循环时，游戏结束，清屏 
}
/*	*/
