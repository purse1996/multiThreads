#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

// 解决可能存在的死锁问题
std::mutex mu1;
std::mutex mu2;
void print1(int i)
{
	// 解决方案 在多个函数中保证锁住互斥对象的顺序是一致的
	// 方法1： 手动指定上锁顺序一致
	/*
	std::lock_guard<std::mutex> guard1(mu1);
	std::lock_guard<std::mutex> guard2(mu2);
	*/
	// 方法2：使用std::lock
	// 其中可以指定多个互斥对象，通过这个相当于指定了上锁的顺序,
	// 如此在lock_guard锁住一个mutex后尝试锁定第二个mutex，第二个mutex就会解锁
	std::lock(mu1, mu2);
	std::lock_guard<std::mutex> guard1(mu2, std::adopt_lock);
	std::lock_guard<std::mutex> guard2(mu1, std::adopt_lock);
	std::cout << "from child thread" << "  " << i << std::endl;

}
	
void print2(int i)
{	
	// 解决方案 在多个函数中保证锁住互斥对象的顺序是一致的
	// 方法1： 手动指定上锁顺序一致
	/*
	std::lock_guard<std::mutex> guard1(mu1);
	std::lock_guard<std::mutex> guard2(mu2);
	*/

	// 方法2：使用std::lock
	std::lock(mu1, mu2);// 其中可以指定多个互斥对象，通过这个相当于指定了上锁的顺序
	std::lock_guard<std::mutex> guard1(mu1, std::adopt_lock);
	std::lock_guard<std::mutex> guard2(mu2, std::adopt_lock);
	std::cout << "from main thread" << "  " << i << std::endl;

}

void thread1()
{
	for (int i = 100; i > 0; i--)
	{
		print1(i);
	}
}
void main()
{
	std::thread t(thread1);
	for(int i=0; i<100; i++)
		print2(i);
	t.join();
	system("pause");
}