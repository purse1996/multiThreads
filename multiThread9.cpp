#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <deque>

// 使用packaged_task进行主线程和子线程之间参数相互传递，同时前面学到的很多内容在这也会使用到

std::deque<std::packaged_task<int()>> q;
std::mutex mu;
std::condition_variable cond;

int factorial(int N) {
	int res = 1;
	for (int i = N; i > 1; i--)
		res *= i;
	std::cout << "Result is: " << res << std::endl;
	return res;
}

void thread_1()
{
	std::packaged_task<int()> p;
	{
		std::unique_lock<std::mutex> ulock(mu);
		cond.wait(ulock, [](){return !q.empty(); });
		p = std::move(q.front());// 移动拷贝
	}
	p();

}
void main()
{
	std::thread t1(thread_1);
	// packaged_task构造函数只能有一个参数，所以使用std::bind把factorial需要的参数传进去
	std::packaged_task<int()> t(std::bind(factorial, 5));
	std::future<int> f = t.get_future();

	// 由于在多个线程都是用到了队列q，所以要加锁防止数据冲突
	// 同时需要保证在出队列之前先入队列，所以引入了条件变量
	
	std::unique_lock<std::mutex> ulock(mu);
	q.push_back(std::move(t));
	ulock.unlock(); //在使用cond时，需要首先解锁，否则不会触发notify_one
	cond.notify_one();

	int value = f.get();
	t1.join();
	system("pause");

}