#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

// 生产者和消费者模型。使用condition变量来减少等待时间

std::deque<int> q;
std::mutex mu;
std::condition_variable cond; //使用条件变量
void producer()
{
	int count = 10;
	{
		while (count > 0)
		{
			std::unique_lock<std::mutex> qlock(mu);
			q.push_front(count);
			qlock.unlock();
			cond.notify_one(); //通知其他等待的一个线程
			//cond.notify_all(); //通知正在等待的其他所有线程；
			std::this_thread::sleep_for(std::chrono::seconds(1)); // 线程等待
			count--;
		}
	}
}
void consumer()
{
	int data = 0;
	while (data != 1)
	{
		std::unique_lock<std::mutex> ulock(mu);
		// 防止由于其他原因比如超时之类的造成的伪激活，所以引入了一个lambda函数.
		// cond.wait(ulock);
		cond.wait(ulock, []() {return !q.empty(); });
		data = q.front(); 
		q.pop_back();
		std::cout << data << std::endl;
	}
}

void main()
{
	std::thread t1(producer);
	std::thread t2(consumer);

	t1.join();
	t2.join();

	system("pause");
}
