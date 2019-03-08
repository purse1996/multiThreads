#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>

// ������ݳ�ͻ����

std::mutex mu;   // Ϊ������ݳ�ͻ���뻥�������м���
void function1()
{
	for (int i = 0; i < 10; i++)
	{
		// ����1������1������ڼ�����ִ�д�������г������쳣����ô����һֱ����⿪
		//mu.lock();
		//std::cout << "from child thread" << i << std::endl;
		//mu.unlock();
		// ����2��ʹ��std::lock_guard����mutex������lock_guard����ʱ���Զ��ͷ�
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

		// ���guard�����߳���guardûʲô��ϵ����Ϊ�˳�֮��guard�Ѿ���������
		std::lock_guard <std::mutex> guard(mu); 
		std::cout << "from main thread" << std::endl;

	}
	t1.join();
	system("pause");
}