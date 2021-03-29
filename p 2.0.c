#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>
#define TRUE 1
#define FALSE 0
void insertionSort(int arr[], int n);
void merge(int a[], int l1, int h1, int h2);

void mergeSort(int a[], int l, int h)
{
    int i, len = (h - l + 1);
    if (len <= 5)
    {
        insertionSort(a + l, len);
        return;
    }

    pid_t lpid, rpid;
    lpid = fork();
    if (lpid < 0)
    {
        perror("Left Child Proc. not created\n");
        _exit(-1);
    }
    else if (lpid == 0)
    {
        mergeSort(a, l, l + len / 2 - 1);
        _exit(0);
    }
    else
    {
        rpid = fork();
        if (rpid < 0)
        {
            perror("Right Child Proc. not created\n");
            _exit(-1);
        }
        else if (rpid == 0)
        {
            mergeSort(a, l + len / 2, h);
            _exit(0);
        }
    }

    int status;
    waitpid(lpid, &status, 0);
    waitpid(rpid, &status, 0);
    merge(a, l, l + len / 2 - 1, h);
}

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
void merge(int a[], int l1, int h1, int h2)
{

    int count = h2 - l1 + 1;
    int sorted[count];
    int i = l1, k = h1 + 1, m = 0;
    while (i <= h1 && k <= h2)
    {
        if (a[i] < a[k])
            sorted[m++] = a[i++];
        else if (a[k] < a[i])
            sorted[m++] = a[k++];
        else if (a[i] == a[k])
        {
            sorted[m++] = a[i++];
            sorted[m++] = a[k++];
        }
    }

    while (i <= h1)
        sorted[m++] = a[i++];

    while (k <= h2)
        sorted[m++] = a[k++];

    int arr_count = l1;
    for (i = 0; i < count; i++, l1++)
        a[l1] = sorted[i];
}

void isSorted(int arr[], int len)
{
    if (len == 1)
    {
        printf("Sorting Done Successfully\n");
        return;
    }

    int i;
    for (i = 1; i < len; i++)
    {
        if (arr[i] < arr[i - 1])
        {
            printf("Sorting Not Done\n");
            return;
        }
    }
    printf("Sorting Done Successfully\n");
    return;
}

void fillData(int a[], int len)
{
    int i;
    int lower = 10;
    int upper = 1000;
    for (i = 0; i < len; i++)
        a[i] = rand() % (upper - lower + 1) + lower;
    return;
}
bool isOdd(int l)
{
    if (l % 2 == 0)
    {
        return false;
    }
    if (l % 2 != 0)
    {
        return true;
    }
}

float median(int arr[], int len)
{
    if (len % 2 == 0)
    {
        // EVEN
        float post_medi;
        float medi_pos;
        float medi_val;
        medi_pos = len / 2;
        post_medi = medi_pos - 1;
        medi_val = medi_pos + post_medi / 2;
        printf("the medi pos is:%f\n the post medi is:%f\n the final medi value is:%f\n", medi_pos, post_medi, medi_val);
    }
    if (len % 2 != 0)
    {
        // ODD
        int medi_pos;
        medi_pos = len / 2;
        printf("%d", medi_pos);
    }
}
//scoring point
void ScoringMerge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	/* create temp arrays */
	int L[n1], R[n2];
	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];
	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}
//scoring section
void ScoringMergeSort(int arr[], int l, int r)
{
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l + (r - l) / 2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}
//scoring section
void printArray(int A[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}

int main()
{
    int shmid;
    key_t key = IPC_PRIVATE;
    int *shm_array;
    int length;
    clock_t start, end;
    double cpu_time_used;
    float medi = 0;

    start = clock();
    printf("Enter No of elements of Array:");
    scanf("%d", &length);
    size_t SHM_SIZE = sizeof(int) * length;
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        _exit(1);
    }
    if ((shm_array = shmat(shmid, NULL, 0)) == (int *)-1)
    {
        perror("shmat");
        _exit(1);
    }
    srand(time(NULL));
    fillData(shm_array, length);
    mergeSort(shm_array, 0, length - 1);
    for (int i = 0; i <= length; i++)
    {
        printf("%d", shm_array[i]);
        printf("\n");
    }
    // printf("the length is %d \n", length);
    isSorted(shm_array, length);
    //ابتدا میانه داده ها را بدست آورده سپس برای نیمه اول داده ها
    //(از کوچکترین عدد تا میانه) مجددا یکبار دیگر میانه را محاسبه می نماییم
    //این عدد که میانه نیمه اول داده ها است همان چارک اول می باشد.
    float newArr[length];
    float newArrQ3[length];
    int j;
    int newArrLen;
    int newQ3ArrLen;
    int Q3Counter = 0;
    newArrLen = j;
    int p = 0;
    if (isOdd(length) == TRUE)
    {
        // ODD CASE
        medi = shm_array[(length / 2)];
        int j = 0;
        printf("median=%0.3f\n", medi);
        j = 0;
        int Q3Counter = 0;
        for (int i = 0; i < length; i++)
        {
            if (shm_array[i] <= medi)
            {
                newArr[j] = shm_array[i];
                j++;
            }
            if (shm_array[i] >= medi)
            {
                newArrQ3[Q3Counter] = shm_array[i];
                Q3Counter++;
            }
        }
        printf("Q3Counter:%d\n", Q3Counter);
        printf("j:%d\n", j);
        for (int i = 0; i < j; i++)
        {
            printf("[%d]:%0.3f\n", i, newArr[i]);
        }
        for (int i = 0; i < Q3Counter; i++)
        {
            printf("[*%d]:%f\n", i, newArrQ3[i]);
        }
        // calculating Q3 on new array
        if (isOdd(Q3Counter) == TRUE)
        {
            printf("Q3:%f\n", newArrQ3[Q3Counter / 2]);
        }
        if (isOdd(Q3Counter) == FALSE)
        {
            printf("Q3:%f\n", newArrQ3[(Q3Counter / 2) - 1] + newArrQ3[(Q3Counter / 2)]);
        }
        // just in case the new array length is odd
        if (isOdd(j) == TRUE)
        {
            printf("Q1:%0.3f\n", newArr[j / 2]);
        }
        // just in case the new array length is even
        if (isOdd(j) == FALSE)
        {
            float cal;
            cal = newArr[(j / 2) - 1] + newArr[(j / 2)];
            printf("Q1:%0.3f\n", cal);
        }
    }

    if (isOdd(length) == FALSE)
    {
        // even CASE
        Q3Counter = 0;
        medi = shm_array[(length / 2) - 1] + shm_array[(length / 2)];
        medi = medi / 2;
        printf("median=%f\n", medi);
        int k = 0;
        for (int i = 0; i < length; i++)
        {
            if (shm_array[i] <= medi)
            {
                newArr[k] = shm_array[i];
                k++;
            }
            if (shm_array[i] >= medi)
            {
                newArrQ3[Q3Counter] = shm_array[i];
                Q3Counter++;
            }
        }

        for (int i = 0; i < k; i++)
        {
            printf("[%d]:%f\n", i, newArr[i]);
            //   printf("[*%d]:%f\n", i, newQ3Arr[i]);
        }
        for (int i = 0; i <= Q3Counter; i++)
        {
            printf("[*%d]:%f\n", i, newArrQ3[i]);
        }
        if (isOdd(k) == TRUE)
        {
            printf("Q1:%0.3f\n", newArr[k / 2]);
        }
        if (isOdd(Q3Counter) == FALSE)
        {
            printf("Q3:%0.3f\n", (newArrQ3[(Q3Counter / 2) - 1] + newArrQ3[Q3Counter / 2] / 2));
        }
        if (isOdd(Q3Counter) == TRUE)
        {
            printf("Q3:%0.3f\n", newArrQ3[Q3Counter / 2]);
        }
    }

    if (shmdt(shm_array) == -1)
    {
        perror("shmdt");
        _exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        _exit(1);
    }
    // printf("%d\n", isOdd(length));
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Executaion Took About: %d ms \n", end - start);
    printf("CPU Time Used Is About:  %d \n", cpu_time_used);

    //printf("Median: %f \n", medi);

    clock_t scoring_start , scoring_end;
    double scoring_cpu_time_used;
    scoring_start = clock();
    int arr_len;
	printf("enter the length of elements in array:");
	scanf("%d",&arr_len);
	int arr[arr_len];
	float prmpt_num;
	for(int i = 0 ; i < arr_len ; i++){
         scanf("%f\n",&prmpt_num);
         arr[i] = prmpt_num;
	}
    int arr_size = sizeof(arr) / sizeof(arr[0]);
	printf("Given array is \n");
	printArray(arr, arr_size);
	ScoringMergeSort(arr, 0, arr_size - 1);
	printf("\nSorted array is \n");
	printArray(arr, arr_size);
    scoring_start = clock()-start;
    double time_taken = ((double)start)/CLOCKS_PER_SEC; // in seconds
    printf("normal mergesort() took %f seconds to execute \n", time_taken);
    return 0;
}
