文件在github下gh-pages分支

请在运行时使用dev的.cpp格式，原因在于.c格式貌似还未支持C99(此处并未影响我的程序的运行)。

且在线阅读时缩进会略显区别。

考虑到实验要求为10*10地图，下两程序不对地图做修改。

[会动的蛇snake_move](https://github.com/kaijietti/kjtti-homework/blob/gh-pages/snake_move.c)

[贪吃蛇snake_eat](https://github.com/kaijietti/kjtti-homework/blob/gh-pages/snake_eat.c)

## 游戏设计及算法

### 分解问题：

首先贴上游戏玩法以及表示：

1、玩法

　　贪吃蛇游戏是一款经典的益智游戏，有PC和手机等多平台版本。既简单又耐玩。该游戏通过控制蛇头方向吃蛋，从而使得蛇变得越来越长。百度百科

2、游戏表示

　　给定一个10 * 10的字符矩阵表示蛇的生存空间，其中有一条长度5的蛇 (HXXXX)， “ H ”表示蛇头，“ X ”表示蛇身体。空间中可能有食物（“ $ ”表示）和障碍物（“ * ”表示）

　　你可以使用“ADWS”按键分别控制蛇的前进方向“左右上下”， 当蛇头碰到自己的身体或走出边界，游戏结束，否则蛇按你指定方向前进一步。

**分清游戏要素：**（初步）

　　游戏对象主体是蛇，其次是墙、食物。另外，我们还需模块化设计出，移动、判断是否自食或撞墙、··· 的函数。

### 细化问题：

体现**自顶向上的设计思维，逐步求精**：

首先是main()的窗口设计：

（注意：该游戏可选择是否重玩）

```c
int main() { 

	while(if_start_game) {
		restart();//重置游戏 ，在此处选择是否运行游戏，y运行，其余键退出 
		playgame();//玩游戏 
	}
	
	return 0;
}
```

#### 细化问题之关于游戏对象表示的实现

　　如何表示出游戏界面？我们可以采用一个二维数组记录墙、食物、蛇的位置。

　　（考虑到可以选择是否重玩游戏，将使用的数组区分为用于restart和用于playgame两种，restart的数组作为游戏初始记录用，而playgame则用于游戏进行时对游戏对象记录。其余的函数的名称标注也是此用意）

　　**墙**的表示是简单的，在二维数组的边缘且无需改变。

　　**食物**呢？我们通过一个产生随机数的函数，随机生成横纵坐标，并判断是否在地图内。若坐标可行，则在此位置打印出食物。

　　接下来是**蛇**，为了实现移动，我们发现，主导因素是蛇头、蛇尾，因为在每一次的移动中，中间的蛇节就像是不动的一样，这给我们的设计提供了便捷之处，但是单单关注蛇头、蛇尾又是不足够的，因为蛇拐弯是，靠近蛇头的蛇节并不知道自己何去何从，这就引发我们的深层思考，除蛇头外，必须记录每一节蛇的下一节的坐标。

　　于是，贴出代码：

```c
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
```

上述代码展示了初始的墙以及蛇，接下来是投放食物的函数

```c
void put_food(){    //  随机投放食物 

    srand(time(0)); //  初始化伪随机
    int x,y;
    while (1) { //  一直循环随机直到出现可行坐标 
        x=rand() % 12; y=rand() % 12;    //  随机一个坐标 
        if (map_playgame[x][y]==BLANK_CELL) {    //  若此坐标为空白格 
            map_playgame[x][y] = SNAKE_FOOD ;   //  改为食物 
            return;     //  跳出 
        }  
    }
    
}
```

#### 细化问题之游戏的restart、playgame

　　做好了上述表示工作，我们以自顶向上的游戏设计思维为蓝图，此时应该设计游戏的重置以及进行。

　　贴上代码：

```c
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
	put_food();//投放食物 
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
```

#### 细化问题之蛇的移动以及“贪吃”

　　最棘手、最核心的问题莫过于如何让蛇动起来。通过上述的细化问题，我们清楚地知道我们对蛇的移动需要兼顾蛇头和蛇尾、蛇节两大部分，判断是否撞墙或者自食或者是吃到食物是容易的，难点在于如何在打印出地图时，蛇朝着期望的方向前进。

　　除却撞墙及自食结束游戏的情况，蛇的移动剩下是否进食两种情况，若进食，那么由于会生长出新的一节，则相当于蛇尾不动，蛇头前进。若未进食，则蛇全身动，此时除蛇头每一节蛇都要到达原先未移动时下一节蛇的位置，至此，蛇的移动的设计思想即如上。

　　贴出代码：

```c
void snake_move(int dx,int dy) {//蛇移动 
    
	int is_eat = 0;//bool变量判断是否吃到食物
	int x,y;//用于暂存坐标 
	if(map_playgame[headx + dx][heady + dy] == SNAKE_FOOD) {//判断头部进行move时是否会碰到食物 
		is_eat = 1;//如果吃到食物则更新bool 
	}
	if(!is_eat) {//如果没吃到食物，尾巴将往下一节移动 
		Sleep(100);
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
    x = headx + dx ; y = heady + dy;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//更新离头部最近的一节蛇的下一节的坐标，即头部坐标 
	map_playgame[x][y] = SNAKE_HEAD;//移动头部 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//更新头部的坐标
	if(is_eat) {
		put_food();
	} 
} 
```

　　对于Sleep(a)函数的a参数的把握是必要的，为什么？

　　若去掉Sleep(a)函数，蛇的移动将非常快，死的也快。若Sleep(a)函数的参数a（以毫秒为单位）设计过大，将会使蛇的动作延迟非常明显，于是，经过反复测验，我采用100毫秒的延迟时间（其实这是睡眠时间，即程序睡眠，这里的效果及目的在于延迟故称之为延迟时间）。

### 完整代码（问题解决）

```c
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
#define SNAKE_FOOD '$'
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
void put_food();//投放食物 
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
	int i;
	for(i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
    
} 
void put_food(){    //  随机投放食物 

    srand(time(0)); //  初始化伪随机
    int x,y;
    while (1) { //  一直循环随机直到出现可行坐标 
        x=rand() % 12; y=rand() % 12;    //  随机一个坐标 
        if (map_playgame[x][y]==BLANK_CELL) {    //  若此坐标为空白格 
            map_playgame[x][y] = SNAKE_FOOD ;   //  改为食物 
            return;     //  跳出 
        }  
    }
    
}
void snake_move(int dx,int dy) {//蛇移动 
    
	int is_eat = 0;//bool变量判断是否吃到食物
	int x,y;//用于暂存坐标 
	if(map_playgame[headx + dx][heady + dy] == SNAKE_FOOD) {//判断头部进行move时是否会碰到食物 
		is_eat = 1;//如果吃到食物则更新bool 
	}
	if(!is_eat) {//如果没吃到食物，尾巴将往下一节移动 
		Sleep(100);
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
    x = headx + dx ; y = heady + dy;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//更新离头部最近的一节蛇的下一节的坐标，即头部坐标 
	map_playgame[x][y] = SNAKE_HEAD;//移动头部 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//更新头部的坐标
	if(is_eat) {
		put_food();
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

	system("cls");//清屏 
	int if_hit = 0;//判断键盘是否有输入 
	put_food();//投放食物 
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
```