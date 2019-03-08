### 这是一个c++11多线程的例子
1. multiThread1.cpp介绍使用join和detach函数添加线程
2. multiThread2.cpp介绍如何获取线程id，线程的移动拷贝，允许的线程数，线程引用传参
3. multiThread3.cpp介绍如何解决数据冲突问题
4. multiThread4.cpp介绍如何解决可能存在的死锁问题
5. multiThread5.cpp使用更为灵活的unique_lock来为互斥对象加锁和解锁;使用std::once_flag保证文件只被打开，同时避免二次检查锁定;
6. multiThread6.cpp生产者和消费者模型。使用condition变量来减少线程等待时间
7. multiThread7.cpp异步任务机制,主线程如何获取子线程返回的结果
8. multiThread8.cpp子线程创建的若干种方法
9. multiThread9.cpp使用packaged_task累从主线程获取子线程的返回值，同时利用前面的条件变量，为互斥对象加锁等
10. multiThread10.cpp是对线程 mutex 条件变量 condition进行时间控制
