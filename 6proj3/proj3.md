# proj3实验报告

---

> Q1: 关于你实现的wait_sem，如果多个进程同时执行wait_sem，但wait的具体信号量不同，它们能并发执行吗？（你应该用一个spinlock去保护一个信号量，另外用一个spinlock保护资源分配）

可以并发执行，我用100个spinlock分别保护100个信号量，使用1个spinlock保护计数器count，计数器判断是否还有资源。
具体方法：在alloc一开始count--，后分配资源；在dealloc释放资源后，count++。

资源如果用信号量，直接分配101个资源，就会一直wait,所以只能用spinlock。
这时候，如果有两个进程同时执行，一个释放一个新分配，这个地方似乎没法保证先释放后分配，只能保证该释放的总能释放。

> Q2: 解释你如何做到以下的：如果receive先执行，但send未执行，你是如何让receive阻塞的？如果send先执行，但receive未执行，你是如何让send阻塞的？

使用1个spinlock保护send和receive的创建和释放。
如果receive先执行，但send未执行，那么我会创建一个资源为0的信号量，放入队列，执行wait；等待send的signal；唤醒后，由receive的进程释放。
如果send先执行，但receive未执行，那么我会创建一个资源为0的信号量，放入队列，执行wait；等待receive的signal；唤醒后，由send的进程释放。

没有使用资源的信号量，因为把（mailbox）队列的长度设置成NPROC（进程的最大个数）。
进程使用send或者receive，把进程放入队列，进程就会阻塞（即每个进程最多只能出现在队列中），所以队列是足够多的。

###  <span style="color:red">遇到的困难和建议 </span>

emm，做的时候把之前学的信号量相关的知识都忘了,只能重新复习一下。然后看了[这个](https://zh.wikipedia.org/wiki/%E7%94%9F%E4%BA%A7%E8%80%85%E6%B6%88%E8%B4%B9%E8%80%85%E9%97%AE%E9%A2%98)
这个实验是比较难，初次使用spinlock和信号量，可能还会有比较多遗漏的地方。以及send，receive先执行的要阻塞，这个地方交换执行有点难。
可以推荐一些学习的资源。

提问：killed=1，runnable后，如果进程运行，会直接进入trap吗？