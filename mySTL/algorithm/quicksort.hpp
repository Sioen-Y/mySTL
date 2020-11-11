//a simple function to swap two numbers
void swap(int *i, int *j)
{
	int temp = *i;
	*i = *j;
	*j = temp;
}

// a function to partition the array arr
// having starting index as - start
// and ending index as - end
int partition(int arr[], int start, int end)
{
	// we take the pivot to be the last element
	// that means all elements smaller
	// to it will be on left and larger on right
	int pivot = arr[end];

	// taking i and j to define the range, we leave the pivot
	int i = start;
	int j = end - 1;

	// loop till in range
    // 当跳出while循环后，arra[i]是从左到右第一个大于pivot的element
	while (i <= j)
	{
		// keep moving till the left element is smaller than pivot
		while (arr[i]<pivot)
			++i;
		// keep moving left till right element is larger
		while (arr[j]>pivot){
            if(j==0)
            break;
            --j;
        }
		  
		// we need to swap the left and right
		if (i <  j)
		{
			swap(&arr[i], &arr[j]);
			++i;
			--j;
		}

        if(i==j)// i==j说明vct[i] == vct[j] ==pivot
        break;
	}

	// once partitioned, we need to put the pivot at correct place
    // 经过swap,arr[i]的value等于pivot，i之前的元素都小于arr[i]，i之后的元素都大于arr[i]
	swap(&arr[i], &arr[end]);

	// return the position of pivot
	return i;
}

// performQuickSort将arr数组从start到end进行从小到大排序
void performQuickSort(int arr[], int start, int end)
{
    // the terminating condition for recursion
    if( start < end ) 
    {
        // get the partition index
        // 经过partition，arr[p]的左边都比arr[p]小，arr[p]的右边有比arr[p]大
        int p = partition(arr,start,end);

        // perform quick sort on left sub part
        // arr的快排被分解成两个部分，一个是arr[p]左边的数列进行快排，一个是arr[p]右边的数列进行快排
        // 而arr[p]大于其左边数列的任意元素，小于右边数列的任意元素，因此arr[p]的位置是固定不受影响的
        performQuickSort(arr,start,p-1);

        // perform quick sort on right sub part
        performQuickSort(arr,p+1,end);
    }
}

// defining a function to perform merge sort on array arr[] of a given size
void quickSort(int arr[], int size)
{
    performQuickSort(arr,0,size-1);
}