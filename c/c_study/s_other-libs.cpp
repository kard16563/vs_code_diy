//其他函数库
#include<stdio.h>
#include<stdarg.h> // 可变长度实参

#include <stdlib.h>
#include <string.h>


int max_int(int n ,...){//n为后面参数的数量 ... 表示后面可以跟其他可变数量的参数
    va_list ap;//这样可以强制函数访问到 n 后面的实参们
    printf(" n-> %d ",n);
    int i,current,largest;

    va_start(ap,n);//实参列表当中 可变长 部分的开始 
    largest = va_arg(ap,int);//把获取的第二次参数N后面的赋值给large 并且自动到下一个

    for (int ii=0;ii<n;ii=ii+1){
        current = va_arg(ap,int);//这个会逐步 逐个获取余下的参数
        printf("\n ii-> %d current-> %d ",ii,current);
        if (current>largest){
            largest=current;

        }

       
    }
        va_end(ap);//进行释放
        printf("\n  最大值--》 %d  \n ",largest);

        return largest;

}

struct city_info{
    char *city;
    int miles;
};

int compare_city(const void *key_ptr,const void *elment_ptr){
    printf("\n run Destination  %s find-> %s the answer %d ",(char*)key_ptr,(char*)((city_info*)elment_ptr)->city,strcmp((char*)key_ptr,((city_info*)elment_ptr)->city));
    return strcmp((char*)key_ptr,((city_info*)elment_ptr)->city);
}

int cmpfunc(const void * a, const void * b)
{
    printf("\n  测试 %d %d",*(int*)a,*(int*)b);
   return ( *(int*)a - *(int*)b );
}


int main(){
    int largest=max_int(4,10,30,20,40);

    printf("\n  最大值--》 %d  \n ",largest);

    int i=1;
    char *ch="i";

    float val;
    char str[20];
   
   strcpy(str, "3400");
   val = atof(str); //将字符串变为数字  这里还有 很多 类似的函数--》414 
   printf("字符串值 = %s, 浮点值 = %f  %f \n", str, val,(val+20));

    printf("\n\n航班选择字符串查找....>");
    char *city_name="wuhan";
    city_info *ptr11;
    const struct city_info mileage[]=
    {
        {"beijing",200},//这个是二分查找的得从小到大排序 按字母顺序从小到大排
        {"linyi",400},
        {"shanghai",300},
        {"wuhan",500},
        
        
        
    };

    int values[] = { 5, 20, 29, 32, 63 };
    int *item;
    int key = 32;
    item = (int*) bsearch (&key, values, 5, sizeof (int), cmpfunc);

   if( item != NULL ) 
   {
      printf("Found item = %d\n", *item);
   }
   else 
   {
      printf("Item = %d could not be found\n", *item);
   }





    printf("\n  样本数量 %d - %d %d %d %d",sizeof(mileage)/sizeof(mileage[0]),
    sizeof(mileage[0]),sizeof(mileage[1]),sizeof(mileage[2]),sizeof(mileage[3]));                                                //这里是元素的数量
    ptr11=(city_info *)bsearch(city_name
                                ,mileage
                                ,sizeof(mileage)/sizeof(mileage[0]),
                    sizeof(mileage[0]),compare_city );//问题
                    //每个元素的大小     //这里指向比较函数
    printf("\n answer %d",ptr11);

    if(ptr11 !=NULL){
        printf("%s,%d",ptr11->city,ptr11->miles);
    }else
    {
        printf("\n no-> %s",city_name);
    }
    



    getchar();
    getchar();
    return 0 ;
}

