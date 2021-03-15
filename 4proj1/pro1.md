# proj1实验报告

---
> **Q1**. 在命令行输入`make`（不带任何参数），编译完成后打开shutdown.asm文件，main函数中的`shutdown()`语句对应的汇编代码是什么？粘贴到实验报告中。

##### 我的回答：

```asm
6:	e8 c7 02 00 00       	call   2d2 <shutdown>
```


> **Q2**：修改shutdown.c文件，将第5行`extern int shutdown()`修改为`extern int shutdown(int)`，然后，将第10行的`shutdown()`改为`shutdown(20)`。重新在命令行执行`make`，然后打开shutdown.asm文件。`shutdown(20)`语句对应的汇编代码是什么？

##### 我的回答：

```asm
9:	c7 04 24 14 00 00 00 	movl   $0x14,(%esp)
10:	e8 cd 02 00 00       	call   2e2 <shutdown>
```

> **Q3**: 此时，esp寄存器指向的（逻辑）内存区域开始的四个字节的值是什么？这是哪条指令的地址？这个值是哪条指令执行后被放到栈里的？esp指向的内存区域再往上四个字节的内容是什么？哪条指令将这个值放入栈中的？

##### 我的回答：

esp寄存器指向的（逻辑）内存区域开始的四个字节的值是`0x2fbc`.
这是`15:	e8 28 02 00 00       	call   242 <exit>`指令的地址.
这个值是`10:	e8 cd 02 00 00       	call   2e2 <shutdown>`指令执行后被放到栈里的.
esp指向的内存区域再往上四个字节的内容是`0x00000014`
`9:	c7 04 24 14 00 00 00 	movl   $0x14,(%esp)`指令将这个值放入栈中的.

> **Q4**: 查看CPU在执行内核的main函数时esp寄存器的值。请问内核的栈的起始地址是什么？（用16进制表示, 即，类似于 0x1234 这种）

##### 我的回答：

通过查看`kernel.asm`文件,我将断点设在`0x80102e00`处。
此时esp的值是`0x8010b5c0`.
内核栈的起始地址是`0x8010bfff`.

> **Q5**: 在命令行执行shutdown命令，此进程用户栈的起始地址是什么？（请确保执行的是shutdown命令而不是其他命令）当CPU执行完int指令陷入内核后，esp指向的栈的起始地址是什么？这个栈是进程的内核栈，不同于内核的栈。

##### 我的回答：

此时esp是`0x2fd8`,此进程用户栈的起始地址是`0x2fff`.
当CPU执行完int指令陷入内核后，esp指向的栈的起始地址是`0x8dfbefff`.

> **Q6**: scheduler函数运行的栈的起始地址是什么？是进程的内核栈还是内核的栈？

##### 我的回答：

在*proj1-revise*目录下编译后使用`grep -r scheduler`,查找到`scheduler`在*kernel.asm*中，得到函数地址`0x80103970`,设断点。

scheduler函数运行的栈的起始地址是`0x8010bfff`,是内核的栈.

>**Q7**: 被选中的进程p的id是什么，p的父进程的id是什么，p的内核栈的起始地址是什么？

##### 我的回答：

loop的asm文件特别奇怪，重复了好多内容。
被选中的进程p的id是`3`,
p的父进程的id是`2`,
p的内核栈的起始地址是`0x8dfbefff`.

>**Q8**: 哪条指令的执行导致了esp指向了不同的栈？esp指向的栈的起始地址是什么？

##### 我的回答：

`0x801044d9 <swtch+14>:       mov    %edx,%esp`指令的执行导致了esp指向了不同的栈
esp指向的栈的起始地址是`0x8dfbefff`.

> **Q9**: cpu执行完swtch函数的ret指令后，跳转到哪个函数中了？

##### 我的回答:

跳转到`sched`函数中.

> **Q10**: 为什么cpu执行完ret指令后没有返回到scheduler函数中，继续执行proc.c中347行swtichkvm（）？

##### 我的回答:

因为在swtch.S中修改了esp寄存器，将它指向了进程的内核栈。

> **Q11**: 继续单步跟踪执行，直到cpu从内核态降级到用户态。哪条指令造成cpu降级的? 

##### 我的回答:

`0x80105419 <trapret+10>:     iret`

>Q12: cpu降级到用户态后执行的第一条用户指令是什么？这条指令在哪个函数中？请根据指令地址从loop.asm找到对应函数。继续跟踪执行，当前是在子进程还是在父进程中运行？

##### 我的回答:

（有点奇怪）
cpu降级到用户态后执行的第一条用户指令是`0x2e1:       ret`.
这条指令在`SYSCALL(fork)`。
当前是在`父进程`中运行，通过跳转判断：

```
(gdb) 
=> 0x4d:        test   %eax,%eax
0x0000004d in ?? ()
2: $esp = (void *) 0x3fd0
1: $esp = (void *) 0x3fd0
(gdb) 
=> 0x4f:        je     0x89
0x0000004f in ?? ()
2: $esp = (void *) 0x3fd0
1: $esp = (void *) 0x3fd0
(gdb) 
=> 0x89:        movl   $0x1940,(%esp)
0x00000089 in ?? ()
2: $esp = (void *) 0x3fd0
1: $esp = (void *) 0x3fd0
```

##### 另一种答案：

cpu降级到用户态后执行的第一条用户指令是`0x17:        test   %eax,%eax`.
这条指令在`main`函数。
当前是在`父进程`中运行。

> **Q13**: 回顾以上过程。eip寄存器的值是一个重要的信息，但是在scheduler调用的swtch函数中似乎并没保存，那么，当选中的p进程结束执行，调用sched返回scheduler时，cpu怎么会知道应该执行switchkvm呢？

##### 我的回答:

使用call调用swtch的时候，栈已经保存了eip.所以cpu是知道的.

>Q14. 子进程在诞生后（以其状态标记为runnable为准），scheduler便可选择它运行。在第一次选中它执行时，在swtch的ret指令后，CPU执行的第一条指令是什么，在哪个函数中？这是在哪里设置的？

##### 我的回答:

CPU执行的第一条指令是什么`push   %ebp`,在`forkret`函数.这是在`allocproc`函数里设置。

>Q15: 在父进程优先的情况下，偶尔会有子进程先于父进程打印到屏幕的情况出现；在子进程优先的情况下，偶尔也会有父进程先于子进程打印到屏幕的情况。解释可能的原因。

##### 我的回答:

1. 可能是进程的时间片用完了。
2. 循环过多，导致队列中父进程子进程顺序出现问题。

###  <span style="color:red">遇到的困难和建议 </span>

1. Q3调试的时候直接用虚拟机的gdb调试，然后一直错误。知道问了老师，用`make qemu-gdb`.

2. gdb调试的时候上次设在proj0-revise，得到如下提示(修改解决）：

```bash
add-auto-load-safe-path /home/ubuntu/workspace/proj1-revise/.gdbinit
line to your configuration file "/home/ubuntu/.gdbinit".
```

3. 调试loop的时候，设完两个断点，si运行进入死循环。后来佛系多次调试才成功。再次尝试的时候要输出几个p（parent）后再si才行。

4. 做子进程优先的fork时，不知道该怎么添加。尝试了好久，最后问了饶春旭才完成。
