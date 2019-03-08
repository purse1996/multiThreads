#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

// �����ߺ�������ģ�͡�ʹ��condition���������ٵȴ�ʱ��

std::deque<int> q;
std::mutex mu;
std::condition_variable cond; //ʹ����������
void producer()
{
	int count = 10;
	{
		while (count > 0)
		{
			std::unique_lock<std::mutex> qlock(mu);
			q.push_front(count);
			qlock.unlock();
			cond.notify_one(); //֪ͨ�����ȴ���һ���߳�
			//cond.notify_all(); //֪ͨ���ڵȴ������������̣߳�
			std::this_thread::sleep_for(std::chrono::seconds(1)); // �̵߳ȴ�
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
		// ��ֹ��������ԭ����糬ʱ֮�����ɵ�α�������������һ��lambda����.
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
