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