#include<stdio.h>

#include<stdlib.h>
#include <string.h>


     void* p123(int i ){   // 不知道该返回什么类型的指针
        printf("5555 --> %d",i);
    } 

    char* concat(const char *s1,char *s2){    //返回的为指针类型
        char *result=(char*)malloc(strlen(s1)+strlen(s2)+2);
        printf("\n  concat ->  %c,%c,%d \n",*s1,*s2,strlen(s1));
        
        if (*result == NULL)
        {
            printf("\n 指针为空 \n");
            //exit(0);
        }

        strcpy(result, s1);
        printf("\n 1  %c --> \n",*result);

        strcat(result,s2);
        printf("\n 2  %c --> \n",*result);
        printf("address %p ",result);//返回地址
        return result;
    }

void print_str(char *p){
    while (*p != NULL )
    {
       printf(" \n print_str_=>oooo - > %c \n ",*p);
       p=p+1;

       
    }
}

int ahah(  ){
    printf("\n\n\n  本函数由指针调用 ----");
    return 88;
}
//  int (*f)() -》 int 函数返回类型 (*f) 函数指针--函数名--> 跟数组一样
//                  （） 指针所指函数的参数 类型 没有就空着
int haha( int (*f)(),int a ){
    printf(" int (*f)() --<  %d",(*f)() );//-->调用函数
    printf("\n\n   ----- > %d---- ",a);
    return 99;
}

int aa12( int i,int b){
    
    printf("%d",i);
    return i;

}


int haha2( int (*f)(int i,int ii),int a ){
    printf(" int (*f)()haha2 --<  %d",(*f)(a,a)  );//-->调用函数
    printf("\n\n   ----- > %d---- ",a);
    return 99;
}


/*void qsort(void *base, size_t nmemb,
                   size_t size,
                   int(*compar)(const void *, const void *)
                   );
because void* pointers can point to anything. Because we don't know the size of the individual elements in a void* array,
 we must give qsort the number of elements, nmemb, of the array to be sorted, base, 
 in addition to the standard requirement of giving the length, size, of the input. 

But what we're really interested in is the compar argument to qsort: 
it's a function pointer that takes two void *s and returns an int. 
This allows anyone to specify how to sort the elements of the array 
base without having to write a specialized sorting algorithm. 
Note, also, that compar returns an int; 
the function pointed to should return -1 
if the first argument is less than the second, 
0 if they are equal, or 1 if the second is less than the first.                   
                   
                   */
int int_sorter( const void *first_arg, const void *second_arg )
{
    printf("\n run ");
    int first = *(int*)first_arg;
    int second = *(int*)second_arg;
    printf(" fir %d sec %d",first,second);
    if ( first < second )
    {
        return -1;
    }
    else if ( first == second )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int main(){
    int *i;//没初始化就没有申请位置

    if ( i == NULL){
        printf(" null \n \n ");
        printf(" %p ", i );//空白  0x0 为16进制中的空白等于十进制的0
    }


    char te[]="678";
    char *p=(char*)malloc( sizeof(char)*10);

    if (  *p == NULL)
    {
        printf("NULL-----> 空指针 1 \n " );
    }

    p=te;
    printf("\n ppp-> %c \n",*p);
    print_str(p);
    

    int*in= (int*)p123(55);//返回int形式的指针
    char *pp;
    //const char pa[10]=concat("456","abc");

    pp=concat("456","abc");//更新pp地址
    printf("\n answer-> %c ", *pp);
    print_str(pp);
    
    //分配了空间，没指向东西 为野指针
    //没分配空间为 空指针 ---->这两个在vs好像都一样

    int *emp;//一般为也指针
    int *emp_true=NULL;//空指针
    //printf(" %d %p ",*emp_true,emp_true);
    // printf("空指针 的区分 emp : 地址 ：%p ,内容 %d   emp : 地址 ：%p ,内容 %d ", emp,*emp,emp_true,*emp_true);

    int *ga=(int*)malloc(sizeof(int)*10);//---->  1
    int *ga2=(int*)malloc(sizeof(int)*10);//----> 2
    *ga=1;
    *ga2=2;
    printf(" \n --- ga -> %d  %d",*ga,*ga2);

    //free(ga);//将ga的空间还回去防止泄漏
    //调用free（）会会释放ga所指的内存块但不会改变指针本身 生成悬空指针
    ga=ga2;//ga指向2区域  ga2也指向2区域  ga原来指1区域 现在没了
            // 1区域无法跟踪 程序无法访问 这样被称为 垃圾 留有垃圾的程序 称为内存泄漏
     printf(" \n --- ga -> %d  %d",*ga,*ga2);


    char *dang_pointer=(char*)malloc(sizeof(char)*10);
    printf("\n\n\n   悬空指针检查-1  %p %d \n\n\n",dang_pointer,*dang_pointer);
    //在c++中输出为13
    free(dang_pointer);
    dang_pointer=NULL;//注销掉空指针
    //printf("悬空指针检查-2  %p %s \n\n\n",dang_pointer,*(dang_pointer+1));
    //strcpy(dang_pointer,"asad");


    haha(   ahah  , 456  );

    int  (*fun) (int ii ,int i) =& aa12;//直接指向喊
    (*fun)(5,5);


    int array[10];
    int ii=0;
    /* fill array */
    for ( ii = 0; ii<10; ii=ii+1 )
    {
       array[ ii ] = 10 - ii;
       printf("ii %d  - %d \n",ii,array[ ii ] );
    }
    qsort( array, 10 , sizeof( int ), int_sorter );
    for ( ii = 0; ii < 10; ii=ii+1 )
     {
         printf ( "%d\n" ,array[ii] );
     }


    //haha2(  aa12(5,6) ,456);
    printf("555");


//     char* src=(char*)malloc(7);
// //   char dest[100];
  
//  //  memset(dest, '\0', sizeof(dest));
//    strcpy(src, "123456");
//    //strcpy(dest, src);
 
//    printf("最终的目标字符串： %s \n", src);



    getchar();
    getchar();

    return 1;
}