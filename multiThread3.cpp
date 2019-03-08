#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>

// 解决数据冲突问题

std::mutex mu;   // 为解决数据冲突引入互斥对象进行加锁
void function1()
{
	for (int i = 0; i < 10; i++)
	{
		// 方案1，方案1中如果在加锁后执行代码过程中出现了异常，那么锁就一直不会解开
		//mu.lock();
		//std::cout << "from child thread" << i << std::endl;
		//mu.unlock();
		// 方案2，使用std::lock_guard加锁mutex对象，在lock_guard析构时会自动释放
		std::lock_guard <std::mutex> guard(mu);
		std::cout << "from child thread" << i << std::endl;
	}
}

void main()
{
	std::thread t1(function1);
	for (int i = 10; i > 0; i--)
	{
		//mu.lock();
		//std::cout << "from main thread" << std::endl;
		//mu.unlock();

		// 这个guard和子线程中guard没什么关系，因为退出之后guard已经被析构了
		std::lock_guard <std::mutex> guard(mu); 
		std::cout << "from main thread" << std::endl;

	}
	t1.join();
	system("pause");
}