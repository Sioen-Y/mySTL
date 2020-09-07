#include <cstdio>
#include <cstring>

class SmartPtr{
	public:
	int count;
	int *p;
	SmartPtr():count(1),p(new int()){} //heap��������һ��int�ڴ浥Ԫ
	~SmartPtr(){
		printf("%d smart_ptr deleted\n", *p);
		delete p;
	}
}

class Ptr{
	public:
	SmartPtr *sp;
	Ptr():sp(new SmartPtr()) {} // Ĭ�Ϲ��캯��
	Ptr(const Ptr& other):sp(other.sp){
		sp->count++;
	}
	
	Ptr operator=(const Ptr& other){ // ��ֵ��������other�ͱ�Ptr��sp����Ϊͬһ������Ptr p1; Ptr p2; p1=p2; ��ʱp1��p2��SmartPtr��ͬһ���ڴ浥Ԫ
		sp = other.sp;				 // ������SmartPtr����heap�ϣ���ʹĳ����ʱ����p1���ջ�����SmarkPtr���ڣ�
		sp->count++;
		return *this;
	}
	
	~Ptr(){
		if(--sp->count == 0)
			delete sp;
	}
	
}