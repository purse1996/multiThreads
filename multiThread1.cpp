#include "stdafx.h"
#include <iostream>
#include <thread>

// �ⲿ����Ҫ���ǽ���һ���̵߳�join��detach����

void thread_function()
{
	for (int i = 0; i < 10; i++)
		std::cout << "from child thread" << i << std::endl;
}

int main()
{
	std::thread threadObj(thread_function);
	for (int i = 0; i < 10; i++)
		std::cout << "from main thread" << i << std::endl;
	threadObj.join(); // ���̵߳ȴ����߳���ɺ��ټ���
	// threadObj.detach(); // ���̲߳���Ҫ�������̵߳����״̬��
	std::cout << "Exit of Main function" << std::endl;
	printf("���������������");
	getchar();
	return 0;
}

