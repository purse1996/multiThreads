#include "stdafx.h"
#include <mutex>
#include <fstream>
#include <future>

// ���ڵ���Ҫ�����ǽ�����������Ƚ������̴߳����������ַ���
// �������߳��������ִ���������std::thread��std::async���������std::threadΪ��
class A {
public:
	void f(int x, char c) { }
	int operator()(int N) { return 0; }
};

void foo(int x) {

}

int main() {
	A a;
	std::thread t1(a, 6);//����a�Ŀ��������߳�
	std::thread t2(std::ref(a), 6);//����a�����ø����߳�
	std::thread t3(std::move(a), 6);//�ƶ�����a�����߳�,��ʱ�����߳��о��޷�����
	std::thread t4(A(), 6); //������ʱ������a��������߳�

	std::thread t5(foo, 6); //ͨ����֪�����������߳�
	std::thread t6([](int x) {return x * x; }, 6); //����lambda�����������߳�
	std::thread t7(&A::f, a, 8, 'w'); //����a�Ŀ����ĳ�Ա���������߳�
	std::thread t8(&A::f, &a, 8, 'w'); //����a�ĵ�ַ�ĳ�Ա���������̣߳������̴߳���

	std::async(std::launch::async, a, 6); //�����Ϸ���ͬ������

	return 0;
}