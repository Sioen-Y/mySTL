// new is a build-in operator, operator new() is a function
// new() �ȵ���operator new()���뵽һ���ڴ浥Ԫ��֮��ʹ��construct�������

void *operator new(size_t size){
	cout<<"new operator overloaded"<<endl;
	void* p = malloc(size);
	return p;	
}

void operator delete(void* ptr){
	cout<<"delete operator overloaded"
	free(ptr);
}

// placement new������һ���Ѿ�������ڴ���Դ�Ϲ������
    Complex* pc = new Complex(4.2, 5.3); 
	Complex* pd = new Complex[2]; 
	
// using placement new 
    Complex *pe = new (buf) Complex(2.6, 3.9); 
