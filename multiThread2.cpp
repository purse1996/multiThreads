#include "stdafx.h"
#include <iostream>
#include <thread>
#include <string>

// 这里主要是介绍如何获取线程id，线程的移动拷贝，允许的线程数，引用传参
class function1{
public:
	void operator()(std::string& msg)
	{
		for (int i = 10; i > 0; i--)
		{
			std::cout << "from child thread" << msg << i << std::endl;
			msg = "test";
		}
	}
};

int main()
{
	std::string s = ".......";
	//std::thread t1((function1()), s); // 如果这里是s 即使函数中参数为引用，但也不会修改s的数值
	std::thread t1((function1()), std::ref(s)); // 这样才可以实现真正的引用
	for (int i = 0; i < 10; i++)
	{
		std::cout << "from main thread" << i << std::endl;
	}
	t1.join();
	std::cout << s << std::endl;

	std::cout << t1.get_id() << std::endl; // 输出线程t1的id
	std::cout << "total threads" << std::thread::hardware_concurrency() << std::endl; //输出电脑允许的线程数

	std::thread t2 = std::move(t1); // 线程的拷贝需要用std::move()
	std::cout << t2.get_id() << std::endl; //此时t2的id和t1一致


	system("pause");
	return 0;
}