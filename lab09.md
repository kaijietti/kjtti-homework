# 编程方法：自顶向下，逐步求精

## 简要介绍：

## 应用：

## 我的案例：洗衣机洗衣设计：

提供的洗衣程序有：

water_in_switch(open_close) // open 打开上水开关，close关闭

water_out_switch(open_close) // open 打开排水开关，close关闭

get_water_volume() //返回洗衣机内部水的高度

motor_run(direction) // 电机转动。left左转，right右转，stop停

time_counter() // 返回当前时间计数，以秒为单位

halt(returncode) //停机，success 成功 failure 失败

### 使用伪代码分解“正常洗衣”程序的大步骤：

1.用户选择洗衣模式：对应水位

2.注水，计水位直到达到对应水位时停止注水

3.浸泡，计时器计时，浸泡三十分钟

4.电机转动，左三分钟，又三分钟

5.排水，计水位直到0水位时停止

6.电机转动，左四分钟

6.停机

### 进一步用基本操作、控制语句(IF、FOR、WHILE等)、变量与表达式，写出每个步骤的伪代码：

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

## 总结：

