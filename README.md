# Container
Just for practice

最近在学习一些数据结构,也一边在开始摸索github,这一切对我来说都是新的尝试.

basic_stack是一个虚基类,只定义了stack的必要方法(empty,push,pop,top)

basic_stack_a也是虚基类,是数组实现的stack的祖先,添加了(size,full)

//本来预计会有一个basic_stack_l的虚基类,是链表实现的祖先,考虑之下还是剔除了,虽然size()对于链表来说也可以存在

stack_a是一个基于数组的实现类

stack_l是一个基于双向节点的链表实现类

//关于代码测试的知识有点匮乏,所以代码没有经过严格的测试,也许会存在许多bug.欢迎大家的指正.


///=======================2018/11/12=========分割线============================///

添加了一些queue的实现.与stack类似

basic_queue只定义了(empty,enqueue,dequeue)

basic_queue_a仍是虚基类,是数组实现的祖先,添加了(full,size)

queue_a是一个基于数组的实现类

queue_l是一个基于双向节点的链表实现类.

添加了连个辅助文件,tools和node.node定义了一个简单的双向节点类(bi_node),以后其他的节点,也会定义在这个文件中.

tools则是一些小函数.目前只有两个(array_resize,array_move)

array_resize:重新为数组a分配空间,新空间容量为(ca_a*factor),pos指定从a的pos位置处的数据开始移动.
            先移动[pos,end),再移动[0,pos),移动的数量为min(ca_a,ca_a*factor);

array_move:移动数组a中的序列[ab,ae)到数组b的[bb,bb+ae-ab)中.

///=======================2018/11/13=========分割线============================///
