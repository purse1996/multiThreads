#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>

// 使用更为灵活的unique_lock来为互斥对象加锁和解锁
// 使用std::once_flag保证文件只被打开，同时避免二次检查锁定

class LogFile {
public:
	LogFile()
	{

	}
	void sharePrint(std::string s, int i)
	{
		// 使用多个线程向文件中写入，为了保证一次只被一个线程写，需要进行上锁
		// 方法1：
		//std::unique_lock<std::mutex> file_lock(mu_file); //默认参数是为互斥对象上锁
		//if (!file.is_open())
		//{
		//	file.open("1.txt");
		//}

		// 方法1的缺点是在检查文件是否打开时候，每一次都要对其进行上锁，是一件开销很大的事情，为了避免二次检查锁定
		// 方法二 使用std::once_flag 后面是一个匿名函数， &代表用到的任何外部变量都隐式按引用捕获
		std::call_once(file_flag, [&]() {file.open("1.txt"); });

		// defer_lock表示只创建，不上锁
		// uniqure_lock相对于lock_guard最方便的地方就在于可以在你需要上锁的地方上锁，同时支持移动拷贝
		// 但与此同时开销也更高
		std::unique_lock<std::mutex> ulock(mu, std::defer_lock); 
		ulock.lock();
		file << "from " << s << " " << i << std::endl;
		ulock.unlock();
		
		// 移动拷贝
		std::unique_lock<std::mutex> ulock2 = std::move(ulock);
	}
private:
	std::ofstream file;
	std::mutex mu;
	std::mutex mu_file;
	std::once_flag file_flag;
	
};

void child_function(LogFile& log)
{
	for (int i = 10; i > 0; i--)
		log.sharePrint("child thread", i);
}
void main()
{
	LogFile log;
	std::thread t(child_function, std::ref(log));
	for (int i = 0; i < 10; i++)
		log.sharePrint("main thread", i);
	t.join();
	system("pause");
}