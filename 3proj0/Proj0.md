# Proj0实验报告

---

### Q1. 请描述你改了哪些文件，以及每个修改所起到的作用。

我修改了如下文件

* Makefile的UPROGS中添加_print22\
  * 起到的作用是编译print22.c并变成_print22
* 增加了print22.c
  * 起的作用是实现print22，内容如下

```c++
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  printf(1, "%s", "OS Lab 081520122: ");
  for(i = 1; i < argc; i++)
    printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
  exit();
}
```

---

### Q2：此时CPU的CS寄存器的后两位是多少？当前处于内核态还是用户态？通过info reg来查看。

```bash
Program received signal SIGINT, Interrupt.
=> 0x80103624 <mycpu+20>:       mov    0x80112d00,%esi
mycpu () at proc.c:48
48        for (i = 0; i < ncpu; ++i) {
(gdb) info reg
eax            0x0      0
ecx            0x1      1
edx            0x0      0
ebx            0x80114c54       -2146349996
esp            0x8010b510       0x8010b510 <stack+3920>
ebp            0x8010b528       0x8010b528 <stack+3944>
esi            0x80112780       -2146359424
edi            0x80112784       -2146359420
eip            0x80103624       0x80103624 <mycpu+20>
eflags         0x46     [ PF ZF ]
cs             0x8      8
ss             0x10     16
ds             0x10     16
es             0x10     16
fs             0x0      0
gs             0x0      0
```

此时CPU的CS寄存器的后两位是**00**，当前处于**内核态**。

---

### Q3: 此时CPU的CS寄存器的最后两位是什么？当前处于内核态还是用户态？请通过info reg命令来查看。

```bash
Breakpoint 1, 0x00000000 in ?? ()
(gdb) info reg
eax            0x0      0
ecx            0x1940   6464
edx            0xbfac   49068
ebx            0xbfa8   49064
esp            0x2fd8   0x2fd8
ebp            0x3fc8   0x3fc8
esi            0x0      0
edi            0x0      0
eip            0x0      0x0
eflags         0x202    [ IF ]
cs             0x1b     27
ss             0x23     35
ds             0x23     35
es             0x23     35
fs             0x0      0
gs             0x0      0
```

此时CPU的CS寄存器的最后两位是**11**，当前处于**用户态**

---

### Q4: 哪条指令的执行导致cs寄存器的值改变了？CPU从什么状态变成什么状态？

```bash
(gdb) 
=> 0x2e2:       mov    $0x10,%eax
0x000002e2 in ?? ()
1: $cs = 27
(gdb) 
=> 0x2e7:       int    $0x40
0x000002e7 in ?? ()
1: $cs = 27
(gdb) 
=> 0x80105a78 <vector64+2>:     push   $0x40
vector64 () at vectors.S:320
320       pushl $64
1: $cs = 8
(gdb) 
=> 0x80105a7a <vector64+4>:     jmp    0x801053ad <alltraps>
vector64 () at vectors.S:321
321       jmp alltraps
1: $cs = 8
```

如下这条指令导致cs寄存器的值改变了,CPU从**用户态**转为**内核态**

```bash
0x2e7:       int    $0x40
```

---

### Q5: 哪条指令的执行导致cs寄存器的值发生改变？CPU从什么状态改变成什么状态？

```bash
(gdb) 
=> 0x801053cf <trapret+10>:     iret   
trapret () at trapasm.S:32
32        iret
1: $cs = 8
(gdb) 
=> 0x2e9:       ret    
0x000002e9 in ?? ()
1: $cs = 27
(gdb) 
=> 0x575:       movzbl (%edi),%eax
0x00000575 in ?? ()
1: $cs = 27
```

如下这条指令导致cs寄存器的值改变了,CPU从**内核态**转为**用户态**

```bash
0x801053cf <trapret+10>:     iret
```

---

### Q6: 为何在你的用户命令的执行过程中会发生CPU状态的改变？（无法回答时，可以先完成后面的实验。）

由于这个语句使用了特权指令，需要在内核态执行，所以不能直接在用户程序中使用。

---

### Q7. 用gdb跟踪shutdown的执行过程。xv6在关闭前执行的最后一条指令是什么?

用si跟踪，完全运行不完（放弃）；改用ni跟踪，得到如下指令：

```bash
0x2d7:       int    $0x40
```

---

### Q8. 请描述你添加系统调用的过程（添加用户命令的部分与1一样，故不需重复叙述）。重点解释修改的每个文件所起的作用。

修改了如下5个内容：

* Makefile的UPROGS中添加_shutdown\
  * 起到的作用是编译shutdown.c并变成_shutdown
* 增加了shutdown.c
  * 起的作用是实现shutdown，内容如下

```c++
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  shutdown();
  return 0;
}
```

* syscall.h增加了#define SYS_shutdown 22v
  * 作用是系统调用Int时增加%eax指向的对象
* syscall.c文件static int (*syscalls[])(void)中增加了[SYS_shutdown] sys_shutdown,
  * 作用是系统调用syscall中增加shutdown
* syspro.c文件中增加如下内容
  * 作用是在系统调用中使用outw(0xB004, 0x0|0x2000)令机器关机

```c++
int
sys_shutdown(void)
{
  outw(0xB004, 0x0|0x2000);
  return 0;
}
```

---

### 我在做这个的过程中花时间最多的是

1. 看makefile文件,理解语言
3. 实现shutdown，完全不知道该往哪些地方添加东西，直到看到<a href="http://www.voidcn.com/article/p-dychhxcp-vu.html">这个链接</a>

---

### <span style="color:red">遇到的困难和建议 </span>

* 教一下*系统调用*具体该添加哪些内容，以及为什么，现在依然不是很理解
* 杨昊老板教的grep

```bash
lvchaojie:~/workspace/proj0-revise (master) $ grep -r 'uptime' .
./user.h:int uptime(void);
./usys.S:SYSCALL(uptime)
./sysproc.c:sys_uptime(void)
./syscall.c:extern int sys_uptime(void);
./syscall.c:[SYS_uptime]  sys_uptime,
./syscall.h:#define SYS_uptime 14
```