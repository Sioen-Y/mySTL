// new is a build-in operator, operator new() is a function
// new() 先调用operator new()申请到一块内存单元，之后使用construct构造对象

void *operator new(size_t size){
	cout<<"new operator overloaded"<<endl;
	void* p = malloc(size);
	return p;	
}

void operator delete(void* ptr){
	cout<<"delete operator overloaded"
	free(ptr);
}

// placement new允许在一个已经分配的内存资源上构造对象
    Complex* pc = new Complex(4.2, 5.3); 
	Complex* pd = new Complex[2]; 
	
// using placement new 
    Complex *pe = new (buf) Complex(2.6, 3.9); 
