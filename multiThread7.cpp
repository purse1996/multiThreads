#include "stdafx.h"
#include <iostream>
#include <thread>
#include <future>
#include <mutex>

// ���������ݿ������Ϊ�첽������ƣ��������߳��л�ȡ���߳�ִ�еĽ����һ��thread�ǲ��ܷ��ص�
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
	// ���ڸú����������߳������ã�Ϊ�˷�ֹ���ݳ�ͻ������������ס�˻������
	std::lock_guard<std::mutex> lock(mu);
	std::cout << "Result is: " << res << std::endl;
	return res;
}

void main()
{
	// experiment1 ���̶߳�ȡ���߳̽��
	int x;
	// std::launch::async���ڵ���async�Ϳ�ʼ�����߳�
	// std::launch::deferred���ӳټ��ط�ʽ�����̡߳�����asyncʱ�������̣߳�ֱ��������future��get����waitʱ�Ŵ����߳�
	std::future<int> fu = std::async(std::launch::deferred, factorial, 4);
	x = fu.get(); ////get()������ȴ����߳̽�����Ȼ�󽫷���ֵ����x.����future����ֻ�ܱ�����һ��
	std::cout << "experiment1: " << x << std::endl;


	// experiment2 ���߳������̴߳������,�������̻߳�ȡ���̵߳õ��Ľ��
	std::promise<int> p; //����һ��promise����
	std::future<int> fut = p.get_future(); ////��ȡpromise�ڲ���future��fut����promise����
	std::future<int> fu2 = std::async(std::launch::async, frac2, std::ref(fut));
	p.set_value(5);
	x = fu2.get();
	std::cout << "experiment2: " << x << std::endl;

	// experiment3 ���߳��������̴߳��������Ҳ������Ϊ��future������Կ���

	std::promise<int> p2;
	std::future<int> f = p2.get_future();
	std::shared_future<int> sf = f.share();
	std::future<int> fu3 = std::async(std::launch::async, frac3, std::ref(sf));
	std::future<int> fu4 = std::async(std::launch::async, frac3, std::ref(sf));
	p2.set_value(8);

	system("pause");
}



