# 智能蛇实验报告

本报告包括：

1.Linux环境

　　·搭建Linux环境

2.体验Linux IO

　　·初次体验清屏函数以及VT100 终端标准

　　·贪吃蛇融入kbhit()函数

3.智能蛇


## 搭建Linux环境（Ubuntu 18.04）

（上述Ubuntu为微软商店下载的终端版本）

**编辑环境** Vim（Ubuntu自带）

**编译环境** gcc 7.3.0

```
root@LAPTOP-BQ574H8H:~# sudo apt-get update //此用于更新，提高安装gcc的概率
root@LAPTOP-BQ574H8H:~# sudo apt-get install gcc
```
安装成功后，通过下列指令查看版本号
```
root@LAPTOP-BQ574H8H:~# gcc -v
```
安装成功，则在显示信息的最下方出现如下：
```
gcc version 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04)
```

#### 注意，也可通过VMware构建虚拟机Ubuntu桌面版，实现文件共享会更简单，同样，需要上述指令构建gcc

---

## 初次体验清屏函数以及VT100 终端标准

### VT100 颜色输出

VT100 的颜色输出分为,前景色和背景色可以分别输出,如果不需要之前所有的设置可以用\033[0m取消。
```
1. 字背景颜色范围:40----49
    40:黑
    41:深红
    42:绿
    43:黄色
    44:蓝色
    45:紫色
    46:深绿
    47:白色
    2. 字前景颜色范围:30----39
    30:黑
    31:红
    32:绿
    33:黄
    34:蓝色
    35:紫色
    36:深绿
    37:白色
    3. 输出一个字符串( something here )有前景色和背景色代码如下:
    printf("\033[41;36m something here \033[0m");
```
代码及效果如下：

![](images\QQ截图20181208002140.png)

![](images\QQ截图20181208002339.png)

### sin函数动态图sinDemo示例（清屏函数）

使用vi将代码敲完后，运行效果如下所示：

![](images\sin动态.gif)

#### 注意，此处在编译时可能出现如下问题：

![](images\QQ截图20181208003202.png)

解决方法如下：

![](images\QQ截图20181208003240.png)

#### [为啥？点我进入解释](https://my.oschina.net/u/239287/blog/69028)

---

### 字符蛇融入kbhit()函数

（**温馨提示：加载完成后播放效果流畅**）

![](images\linux下贪吃蛇.gif)

[源码在这里](snake.c)

**注意啦**：

　　其实，第一次跑融入kbhit()的贪吃蛇时，编译正常，但是，在按下按钮之后蛇不会动，
~~（我曾怀疑是kbhit()的问题）~~，后来问过学长之后发现，windows的dev下Sleep()函数是为毫秒为单位的，而linux下sleep()函数是以秒为单位的，后将我的sleep()函数改为usleep()函数后，将参数扩大1000倍，运行效果则正常（usleep()函数参数以微妙为单位）。

---

## 智能蛇设计

效果如下：**（智能的时候）**

![](images\智能蛇1.gif)

**（智障的“瞬间”）**

![](images\智障蛇.gif)

代码：[智能蛇](snake_smart.c)

**思路？**

**首先**，先不考虑距离，判断w、a、s、d四方向的**可行性**，设一数组possible_move[4] = {1,1,1,1} （注意，数组元素依次对应 w、a、s、d，比如，第一个元素下标是0，对应w方向的可行性），若不可行，将不可行方向的对应的下标导引的元素值从0变为1；

**第二步**，经过上一轮的筛选，再**考虑距离**，在put_food()函数里改变food[2]数组的值，下标0导引的元素值对应食物x坐标，下标1导引的元素值对应食物y坐标。创建cur_distance[4]数组，如下所示赋值：

```c

int cur_distance[4];
		
cur_distance[0] = abs(headx - 1 - food[0]) + abs(heady - food[1]);//w

cur_distance[1] = abs(headx - food[0]) + abs(heady - 1 - food[1]);//a

cur_distance[2] = abs(headx + 1 - food[0]) + abs(heady - food[1]);//s

cur_distance[3] = abs(headx - food[0]) + abs(heady + 1 - food[1]);//d

```

**第三步**，在可行的方向中选择最小距离的那个方向作为下一次移动的方向。

以下就是具体的如何选择移动方向的函数设计

```c
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
	if(possible_move[k] == 1) 
        break;//随机选取一个可行方向
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
```

---

## 实验总结

　　经过这次Linux体验以及智能蛇的设计，~~感觉自己很智障~~，我感受到了命令行的魅力，感受到了开发游戏AI的困难而有趣，在这次实验中，我还遇到了不少问题，比方说windows下Sleep(a)函数参数的单位与Linux下sleep(a)函数参数的单位的不同，前者是毫秒，后者是秒；也遇到了平时编程的基础问题，获益匪浅。

　　感谢这次实验，让我体验到亲身投入游戏制作的有趣以及魅力。~~pml无敌~~

