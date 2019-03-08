#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

// ������ܴ��ڵ���������
std::mutex mu1;
std::mutex mu2;
void print1(int i)
{
	// ������� �ڶ�������б�֤��ס��������˳����һ�µ�
	// ����1�� �ֶ�ָ������˳��һ��
	/*
	std::lock_guard<std::mutex> guard1(mu1);
	std::lock_guard<std::mutex> guard2(mu2);
	*/
	// ����2��ʹ��std::lock
	// ���п���ָ������������ͨ������൱��ָ����������˳��,
	// �����lock_guard��סһ��mutex���������ڶ���mutex���ڶ���mutex�ͻ����
	std::lock(mu1, mu2);
	std::lock_guard<std::mutex> guard1(mu2, std::adopt_lock);
	std::lock_guard<std::mutex> guard2(mu1, std::adopt_lock);
	std::cout << "from child thread" << "  " << i << std::endl;

}
	
void print2(int i)
{	
	// ������� �ڶ�������б�֤��ס��������˳����һ�µ�
	// ����1�� �ֶ�ָ������˳��һ��
	/*
	std::lock_guard<std::mutex> guard1(mu1);
	std::lock_guard<std::mutex> guard2(mu2);
	*/

	// ����2��ʹ��std::lock
	std::lock(mu1, mu2);// ���п���ָ������������ͨ������൱��ָ����������˳��
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