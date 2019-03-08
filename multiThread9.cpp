#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <deque>

// ʹ��packaged_task�������̺߳����߳�֮������໥���ݣ�ͬʱǰ��ѧ���ĺܶ���������Ҳ��ʹ�õ�

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
		p = std::move(q.front());// �ƶ�����
	}
	p();

}
void main()
{
	std::thread t1(thread_1);
	// packaged_task���캯��ֻ����һ������������ʹ��std::bind��factorial��Ҫ�Ĳ�������ȥ
	std::packaged_task<int()> t(std::bind(factorial, 5));
	std::future<int> f = t.get_future();

	// �����ڶ���̶߳����õ��˶���q������Ҫ������ֹ���ݳ�ͻ
	// ͬʱ��Ҫ��֤�ڳ�����֮ǰ������У�������������������
	
	std::unique_lock<std::mutex> ulock(mu);
	q.push_back(std::move(t));
	ulock.unlock(); //��ʹ��condʱ����Ҫ���Ƚ��������򲻻ᴥ��notify_one
	cond.notify_one();

	int value = f.get();
	t1.join();
	system("pause");

}