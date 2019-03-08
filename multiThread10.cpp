#include "stdafx.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>  
#include <fstream>
#include <future>
#include <deque>

// �ò�����Ҫ�����Ƕ��߳� mutex �������� condition����ʱ�����

int factorial(int N) {
	int res = 1;
	for (int i = N; i > 1; i--)
		res *= i;
	std::cout << "Result is: " << res << std::endl;
	return res;
}

int main() {
	//ʵ�ֶ��̵߳�ʱ������
	std::thread t1(factorial, 6);
	//std::this_thread::sleep_for(std::chrono::milliseconds(3));
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(3);
	std::this_thread::sleep_until(tp);

	//ʵ�ֶ�mutex��ʱ������,ע��������Ҫʹ��timed_mutex
	std::timed_mutex mu;
	mu.try_lock_for(std::chrono::milliseconds(3));
	mu.try_lock_until(tp);

	//ʵ�ֶ�����������ʱ������
	std::mutex mu2;
	std::unique_lock<std::mutex> locker2(mu2);
	std::condition_variable cond;
	cond.wait_for(locker2, std::chrono::milliseconds(3));
	cond.wait_until(locker2, tp);

	//ʵ�ֶ�future��ʱ������
	std::promise<int> p;
	std::future<int> f = p.get_future();
	f.wait_for(std::chrono::milliseconds(3));
	f.wait_until(tp);

	t1.join();
	system("pause");
	return 0;
}