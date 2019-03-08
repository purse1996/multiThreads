#include "stdafx.h"
#include <iostream>
#include <thread>

// 这部分主要就是介绍一下线程的join和detach函数

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
	threadObj.join(); // 主线程等待子线程完成后再继续
	// threadObj.detach(); // 主线程不需要考虑子线程的完成状态；
	std::cout << "Exit of Main function" << std::endl;
	printf("按任意键继续……");
	getchar();
	return 0;
}

