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
