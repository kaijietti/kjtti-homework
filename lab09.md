# 编程方法：自顶向下，逐步求精

## 简要介绍：

　　“自顶向下，逐步求精”的编程方法是这样的一个过程：将一个大的任务分解成很多小的、容易理解的子任务(自顶向下)。如果可能的话将每一个子任务进一步的细化(逐步求精)。一旦程序被分解成子任务，每个子任务都可以单独编译和测试。直到每个子任务都被测试并能正常工作，我们才会将子任务合并为一个完整任务。

## 应用：

　　其实，这种自顶向下的方法不只是用于编程，大到工厂、制造业也有这种思想存在，毕竟从Big Picture出发，往往难以得到最优解，而从小部分出发，却总能得到部分解，从而得到通往Big Picture的部分蓝图。比方说下面的小变形金刚例子：

![](images\200962111342148249.jpg)

　　当然，上面的例子不足以概括这个编程方法的精髓。但下面的案例，足以体验一下贴近生活的“自顶向下”的思想，就从洗衣机开始吧！！

## 我的案例：洗衣机洗衣设计：

#### 提供的洗衣程序有：

water_in_switch(open_close) // open 打开上水开关，close关闭

water_out_switch(open_close) // open 打开排水开关，close关闭

get_water_volume() //返回洗衣机内部水的高度

motor_run(direction) // 电机转动。left左转，right右转，stop停

time_counter() // 返回当前时间计数，以秒为单位

halt(returncode) //停机，success 成功 failure 失败

#### 使用伪代码分解“正常洗衣”程序的大步骤：

1.用户选择洗衣模式：对应水位

2.注水，计水位直到达到对应水位时停止注水

3.浸泡，计时器计时，浸泡三十分钟

4.电机转动，左三分钟，又三分钟

5.排水，计水位直到0水位时停止

6.电机转动，左四分钟

6.停机

#### 进一步用基本操作、控制语句(IF、FOR、WHILE等)、变量与表达式，写出每个步骤的伪代码：

```

READ Mode

WHILE 水位 < Mode对应水位

　　　　water_in_switch(open)

ENDWHILE

water_in_switch(close)

SET time_counter() to 0

WHILE time_counter() < 1800

　　　　motor_run(stop)

ENDWHILE

SET time_counter() to 0

WHILE time_counter() > 0

　　　　IF time_counter() <= 180 THEN

　　　　　　　　motor_run(left)

　　　　ELSE

　　　　　　　　motor_run(right)

　　　　ENDIF

ENDEHILE

motor_run(stop)

WHILE 水位 > 0

　　　　water_out_switch(open)

ENDWHILE

water_out_switch(close)

SET time_counter() to 0

WHILE time_counter() < 240

　　　　motor_run(left)

ENDWHILE

HALT

```

#### 对洗衣机洗衣程序的小总结：

　　在上述案例中：对洗衣过程如选择模式、注水、浸水、电机转动强度等程序的划分、提炼，就是顺序控制设计的体现，其中，对注水时间、电机转动方向及强度进行数量化，使得这些单独的程序有自己正常工作的条件基础，这就是逐步求精。宏观上看，洗衣这个Big Picture被我们像上面方式一样分解，也是自顶向下的一种体现。

## 总结：

　　经过上面案例的分析，我相信，“自顶向下，逐步求精”应该在你脑海中有个更贴切的印象或者感觉了，其实它就是一种顺序的分而治之，寻求基本元素，既然它如此有用，何不去尝试一下解决更大的问题呢？