# proj2实验报告

---

> Q1： 在你的设计中，struct proc增加了几个字节？（优先级调度部分增加的不算）

4个字节。
**方案**：
增加了用户栈的指针（事实上可以保存在进程的内核栈中）。
以及将isthread存在了进程的内核栈中，用了1个字节。有两种可能会改内核栈（创建和切换），所以我们要在allocproc函数还有vm.c的switchuvm函数里面修改。

> Q2. 父进程的优先级为1，为何有时优先级低的子进程会先于它执行？父进程似乎周期性出现在打印列表中，为什么？（你需要阅读schedtest.c） 

解答：
1. 可能是父进程正处于阻塞状态（printf输出）。
2. 因为父进程wait了子进程，当其中一个子进程结束时，就可以唤醒父进程。子进程之间都是由父进程fork的，属性类似。

> Q3. set_priority系统调用会否和scheduler函数发生竞争条件（race condition）？你是如何解决的？

会的。优先级影响。
当调用了set_priority时，如果设置的优先级比当前进程的优先级高，则重新scheduler。同时给set_priority增加&ptable.lock.

**我的优先级调度方案**：
1. 若执行了当前优先级的进程，则重新从优先级最高的开始；
2. 若没有执行当前优先级的进程，则选择下一个优先级，如果没有下一个优先级了（优先级变成4了），则重新进入最高优先级。
3. 若当前执行的进程唤醒了更高优先级的，或者设置了更高优先级的，则重新从优先级最够的开始。

###  <span style="color:red">遇到的困难和建议 </span>

实验难度太大，提示非常少。
尤其是第一个实验，要理解的内容太多。
来来回回，终于能了解个大概了，把程序写完整了。
以及，很感谢老师对我的每个问题都进行了很大的解答。

1. thread，当父进程退出，子进程怎么释放用户栈的资源？ 没解决
2. scheduler执行完一个时间片后:（非常复杂）
    - 可能唤醒了优先级更高的进程，重新执行scheduler
    - 可能继续执行当前优先级的循环
    - 可能执行下一优先级
    - 重新scheduler

以及，在`workplace`目录下创建了`Makefile`文件，提供了一条龙服务。主要是下列三点：
1. 创建。使用`make wget`可完成下载，解压并复制的功能。
2. 编译并运行qemu。使用`make`或者`make all`可以编译运行qemu。
3. 生成patch文件。使用`make diff`.自动clean`proj2-revise`并生成patch。

只需要修改`PROJ`和`ID`即可完成。

```Makefile
PROJ=proj2
ID=081520122

DIR=$(PROJ)-revise

all:
	cd $(DIR) && make qemu

wget:
	wget http://xzhu.info/os/$(PROJ)-base.zip && \
	unzip $(PROJ)-base && \
	cp -R $(PROJ)-base $(PROJ)-revise
	
clean:
	cd $(DIR) && make clean
	
diff:clean
	diff -uNr  $(PROJ)-base  $(DIR) > $(ID).$(PROJ).patch; [ $$? -eq 1 ]
```
