#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>

// ʹ�ø�Ϊ����unique_lock��Ϊ�����������ͽ���
// ʹ��std::once_flag��֤�ļ�ֻ���򿪣�ͬʱ������μ������

class LogFile {
public:
	LogFile()
	{

	}
	void sharePrint(std::string s, int i)
	{
		// ʹ�ö���߳����ļ���д�룬Ϊ�˱�֤һ��ֻ��һ���߳�д����Ҫ��������
		// ����1��
		//std::unique_lock<std::mutex> file_lock(mu_file); //Ĭ�ϲ�����Ϊ�����������
		//if (!file.is_open())
		//{
		//	file.open("1.txt");
		//}

		// ����1��ȱ�����ڼ���ļ��Ƿ��ʱ��ÿһ�ζ�Ҫ���������������һ�������ܴ�����飬Ϊ�˱�����μ������
		// ������ ʹ��std::once_flag ������һ������������ &�����õ����κ��ⲿ��������ʽ�����ò���
		std::call_once(file_flag, [&]() {file.open("1.txt"); });

		// defer_lock��ʾֻ������������
		// uniqure_lock�����lock_guard���ĵط������ڿ���������Ҫ�����ĵط�������ͬʱ֧���ƶ�����
		// �����ͬʱ����Ҳ����
		std::unique_lock<std::mutex> ulock(mu, std::defer_lock); 
		ulock.lock();
		file << "from " << s << " " << i << std::endl;
		ulock.unlock();
		
		// �ƶ�����
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