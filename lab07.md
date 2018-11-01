# 任务 1：简单程序

### （1）打开网页 The PIPPIN User’s Guide ，然后输入 Program 1：Add 2 number

由于该网页打不开，自己编写一个，如下所示：
![](images\QQ截图20181101194448.png)

### （2）点step after step。观察并回答下面问题：

1)PC，IR 寄存器的作用。

IR 指令寄存器，用来保存当前正在执行的一条指令。

PC 程序计数器，存放下一条要执行指令在内存中的地址。

2)ACC 寄存器的全称与作用。

全称是 The accumulator(A register)，用于存放操作的数据和结果，是一种特殊的存储寄存器。

3)用“LOD #3”指令的执行过程，解释Fetch-Execute周期。

从 RAM 获取指令及数据(LOD #3) -> IR寄存器 -> Decoder(Decode instruction(LOD #3)) -> 数字3存在MUX(数据选择器)中 -> Decoder将LOD指令传给ALU -> ALU从MUX得到数字3 -> 累加到ACC上 -> PC累加2，用于存放下一条指令的地址

4)用“ADD W” 指令的执行过程，解释Fetch-Execute周期。

从 RAM 获取指令及数据(ADD W) -> IR寄存器 -> Decoder(Decode instruction(ADD W)) -> MUX(数据选择器)从 RAM 中获取W对应地址的内容，即数值3 -> Decoder将ADD指令传给ALU -> ALU从MUX得到数字3 -> 累加到ACC上 -> PC累加2，用于存放下一条指令的地址

5)“LOD #3” 与 “ADD W” 指令的执行在Fetch-Execute周期级别，有什么不同。

“LOD #3” 与 “ADD W” 

### （3）点击“Binary”,观察回答下面问题

写出指令 “LOD #7” 的二进制形式，按指令结构，解释每部分的含义。

解释 RAM 的地址。

该机器CPU是几位的？（按累加器的位数）

写出该程序对应的 C语言表达。

# 任务 2：简单循环

### （1） 输入程序Program 2，运行并回答问题：

用一句话总结程序的功能

写出对应的 c 语言程序

### （2） 修改该程序，用机器语言实现 10+9+8+..1 ，输出结果存放于内存 Y

写出 c 语言的计算过程

写出机器语言的计算过程

用自己的语言，简单总结高级语言与机器语言的区别与联系。
