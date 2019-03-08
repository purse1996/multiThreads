#include "stdafx.h"
#include <iostream>
#include <thread>
#include <string>

// ������Ҫ�ǽ�����λ�ȡ�߳�id���̵߳��ƶ�������������߳��������ô���
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
	//std::thread t1((function1()), s); // ���������s ��ʹ�����в���Ϊ���ã���Ҳ�����޸�s����ֵ
	std::thread t1((function1()), std::ref(s)); // �����ſ���ʵ������������
	for (int i = 0; i < 10; i++)
	{
		std::cout << "from main thread" << i << std::endl;
	}
	t1.join();
	std::cout << s << std::endl;

	std::cout << t1.get_id() << std::endl; // ����߳�t1��id
	std::cout << "total threads" << std::thread::hardware_concurrency() << std::endl; //�������������߳���

	std::thread t2 = std::move(t1); // �̵߳Ŀ�����Ҫ��std::move()
	std::cout << t2.get_id() << std::endl; //��ʱt2��id��t1һ��


	system("pause");
	return 0;
}