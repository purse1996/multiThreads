#include "stdafx.h"
#include <iostream>
#include <thread>
#include <future>
#include <mutex>

// 本部分内容可以理解为异步任务机制，即在主线程中获取子线程执行的结果。一般thread是不能返回的
std::mutex  mu;
int factorial(int N) {
	int res = 1;
	for (int i = N; i > 1; i--)
		res *= i;
	std::cout << "Result is: " << res << std::endl;

	return res;
}

int frac2(std::future<int>& f)
{
	int res = 1;
	int N = f.get();
	for (int i = N; i > 1; i--)
		res *= i;
	std::cout << "Result is: " << res << std::endl;
	return res;
}

int frac3(std::shared_future<int>& f)
{
	int res = 1;
	int N = f.get();
	for (int i = N; i > 1; i--)
		res *= i;
	// 由于该函数被两个线程所调用，为了防止数据冲突，所以在这锁住了互斥对象
	std::lock_guard<std::mutex> lock(mu);
	std::cout << "Result is: " << res << std::endl;
	return res;
}

void main()
{
	// experiment1 主线程读取子线程结果
	int x;
	// std::launch::async：在调用async就开始创建线程
	// std::launch::deferred：延迟加载方式创建线程。调用async时不创建线程，直到调用了future的get或者wait时才创建线程
	std::future<int> fu = std::async(std::launch::deferred, factorial, 4);
	x = fu.get(); ////get()函数会等待子线程结束，然后将返回值传给x.并且future对象只能被调用一次
	std::cout << "experiment1: " << x << std::endl;


	// experiment2 主线程向子线程传入参数,并且主线程获取子线程得到的结果
	std::promise<int> p; //创建一个promise对象
	std::future<int> fut = p.get_future(); ////获取promise内部的future，fut将和promise共享
	std::future<int> fu2 = std::async(std::launch::async, frac2, std::ref(fut));
	p.set_value(5);
	x = fu2.get();
	std::cout << "experiment2: " << x << std::endl;

	// experiment3 主线程向多个子线程传入参数，也可以认为是future对象可以拷贝

	std::promise<int> p2;
	std::future<int> f = p2.get_future();
	std::shared_future<int> sf = f.share();
	std::future<int> fu3 = std::async(std::launch::async, frac3, std::ref(sf));
	std::future<int> fu4 = std::async(std::launch::async, frac3, std::ref(sf));
	p2.set_value(8);

	system("pause");
}



