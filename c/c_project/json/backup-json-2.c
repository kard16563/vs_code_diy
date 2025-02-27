#ifdef _WINDOWS  // 内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif//在两个 .c 文件首行插入这一段代码  
//并在 main() 开始位置插入
//int main() {
//#ifdef _WINDOWS
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif


#include "json.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */

#include <string.h>  /* memcpy() */

#ifndef T_PARSE_STACK_INIT_SIZE // stack
    #define T_PARSE_STACK_INIT_SIZE 256// stack
#endif

#define expect(c,ch) do { assert(*c->json == (ch));c->json++; } while(0)

typedef struct{
    const char *json;
    char* stack ; //引入堆栈 用作缓冲区
    size_t size, top;// 堆栈的东西

}t_context;

static void t_parse_ws(t_context *c){// parse_whitespace 解析空格
    const char *p = c->json;//测试第一个字符n
    while (*p==' ' || *p=='\t' ||*p=='\n'||*p=='\r' )
    {
        p++;//看看如果是字符串 就继续向后进行拨动字符串 直到不是为止
    }
    //printf("\n fun->t_parse_ws_>  p - %d ",p);
    c->json=p;//将位置进行保存
}


///////////////

// static int t_parse_null(t_context *c,t_value *v){//解析null

//     expect(c,'n');
//     if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l'  )//测试剩下的ull
//     {//字符串判断前3个字母 null
//         return T_PARSE_INVALID_VALUE;//不是那三种字面值 null、false 或 true
//     }
//     // 反之有
//     c->json+=3;
//     //printf("\n  null 类型正确 \n ");
//     v->type = T_NULL;
//     return T_PARSE_OK;
    
// }


// static int t_parse_true(t_context *c,t_value *v){

//     expect(c,'t');// t? true
//     //printf(" \n  true is  %c,%c ,%c   \n ",c->json[0],c->json[1],c->json[2]);
//     if (c->json[0] != 'r' ||c->json[1] != 'u' || c->json[2] != 'e'  )
//     {
//         //printf(" \n  true is bad %c,%c ,%c   \n ",c->json[0],c->json[1],c->json[2]);
//         return T_PARSE_INVALID_VALUE;//反向判断 快速
//     }
//     c->json+=3;//指针指到最后
//     //printf(" \n  true is ok \n ");
//     v->type=T_TURE;                 //输出结果
//     return T_PARSE_OK;              //表示过程完毕   
// }

// static int t_parse_false(t_context *c,t_value *v){

//     expect(c,'f');
//     //printf(" \n  true is ok 2 \n ");
//     if( c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e' ){
//         return T_PARSE_INVALID_VALUE;
//     }
//     //printf(" \n  true is ok \n ");
//     c->json+=4;

//     v->type=T_FALSE;
//     return T_PARSE_OK;//成功解析

// }

// //这些null true Flase 函数非常的相似 违反编程中常说的 DRY（don't repeat yourself）原则
// //需要合并  但是 上述把 3 个函数合并后，优点是减少重复的代码，维护较容易，
// //但缺点可能是带来性能的少量影响。  软件的架构难以用单一标准评分，重构时要考虑平衡各种软件品质。

// 将 if( c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e' )
// 之类的收为上面操作
static int t_parse_literal(t_context *c ,t_value *v,const char *literal,t_type type){

//与int固定四个字节不同有所不同,size_t的取值range是目标平台下最大可能的数组尺寸,
//一些平台下size_t的范围小于int的正数范围,
//又或者大于unsigned int. 使用Int既有可能浪费，又有可能范围不够大。
    size_t i;
    expect(c,literal[0]);//第一个字母进行比较 成功并将c的指针后移
    for ( i = 0; literal[i+1] ; i++) // literal[i+1] 为空表示 没了----》将不定的字符变量向上收了 以 literal 为代替--》非常巧妙
        if ( c->json[i] != literal[i+1] )
            return T_PARSE_INVALID_VALUE;
    c->json += i;//指针移到最后
    v->type = type;
    
    return T_PARSE_OK;

}

////////////////////////////

void check_string(const char *ch2){

    printf("\n\n  check_string  -----------------------> begin\n");
 while  ( *ch2 )
    {
        printf("%c",*ch2);
        ch2=ch2+1;
    }

    printf("\n end check ------------------>\n\n");
    printf("\n\n json.c check_string 128  -> 字符串检查结果-->   \n");
}


static int  t_parse_end_not_null(t_context *t ){
    if (*t->json != '\0') 
        return  T_PARSE_ROOT_NOT_SINGULAR;
    return T_PARSE_OK;//直接返回结果 一点也不臃肿 函数尽可能要求单功能尽可能 组合使用

}

// static double t_parse_number(t_context *c, t_value *v){
//     char *end;
//     v->n=strtod(c->json,&end);//进行结果转化
//     //printf("\n __> %s",*end);
//     if (c->json == end ) return T_PARSE_INVALID_VALUE;
//     printf("\n v->n %lf \n",v->n);
//     v->type=T_NUMBER;
//     c->json=end;//指到尾部
//     printf("\n t_parse_number--->ok  %d",T_PARSE_OK);
//     return T_PARSE_OK;//解析完毕 用于过程确认
// }
# define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')   // 判断是不是数字
# define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')   //判断是不是从1到9的数组

static double t_parse_number(t_context *c, t_value *v){
    //流水线式的处理
    //============》    》=========    ========
    //   正常      ||  ||         || ||  《-检查
    //             ===》          ====》
    //           异常抛出          异常抛出--》 return T_PARSE_INVALID_VALUE;
    
    
    const char *p = c->json;// 申请一个指针 让他指向json 并方便遍历 不影响json的指针
    //printf ( "   t_parse_number 1 %c  p->%x  c->json%x \n",*p,p ,c->json );
    if( *p == '-' ) p++;
    if( *p == '0' ) p++;
    else
    {    //printf("\n  t_parse_number -> a1 = %d %c \n",ISDIGIT1TO9(*p),*p);
        if (!ISDIGIT1TO9(*p)) return T_PARSE_INVALID_VALUE;
        //printf("\n  t_parse_number -> a1.1 = %d %c\n",ISDIGIT(*p),*p);
        for ( p++ ; ISDIGIT(*p) ; p++ );
        //if ( !ISDIGIT(*p) ) return T_PARSE_INVALID_VALUE;
    }

    if ( *p == '.' )

    {   //printf("\n  t_parse_number -> a2.0 %d %c \n",ISDIGIT1TO9(*p),*p);
        p=p+1; //指针下移动 跳开 .
        //printf("\n  t_parse_number -> a2 %d %c \n",ISDIGIT1TO9(*p),*p);
            if (!ISDIGIT(*p)) return T_PARSE_INVALID_VALUE;
        //printf("\n  t_parse_number -> a2.2 %d %c\n",ISDIGIT(*p),*p);
            for ( p++ ; ISDIGIT(*p) ; p++ );
    }

    if ( *p == 'e' || *p == 'E' )
    {
        p++;
        if (*p == '+' || *p == '-') p++;
        //printf("\n  t_parse_number -> a3 %d %c \n",ISDIGIT1TO9(*p),*p);
        if ( !ISDIGIT(*p) ) return T_PARSE_INVALID_VALUE;
         //printf("\n  t_parse_number -> a3.3 %d %c\n",ISDIGIT(*p),*p);
        for ( p++ ; ISDIGIT(*p) ; p++ );
    }
    //printf ( "    t_parse_number 2 %c  p->%x  c->json%x \n\n ",*p,p ,c->json );
    errno = 0; //errno 是记录系统的最后一次错误代码。代码是一个int型的值
    char *end;
    v->n = strtod( c->json , &end );
    //printf(" --> %lf ",  v->n );
    if( errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL) )
        return T_PARSE_NUMBER_TOO_BIG;
    v->type = T_NUMBER;
    c->json = end;//json指针 指向队尾
    return T_PARSE_OK;
    
    


}

//处理栈函数 缓冲区
//------》一个字符空间 的申请 一个字符空间的填装
#define PUTC(c,ch) do { *(char*) t_contex_push(c,sizeof(char)) = (ch); } while(0)


void t_free(t_value *v){
    //printf("\n\n ----------> t_free - b   \n");
    assert (v != NULL);
    if (v->type == T_STRING)
        free(v->s);
    v->type = T_NULL;
    //printf("\n\n---------------> t_free - n   \n");
}

void t_init2 (t_context *c){//栈的初始化
    c->stack = (char*) malloc(0);//分部内存 指向一个内存 象征一下
    c->top = 0;// 顶部指向0
    c->size=0;
}

static void *t_contex_push(t_context *c,size_t size){
    //用于申请空间处理 返回的是 申请出 空间的 首地址 
    printf("\n\njson.c 227 t_contex_push-> 用于申请空间处理 --------\n");
    void *ret;
    assert(size>0);//判断传入的数据是否出错大于0就通过 
    printf("\n\n  t_contex_push  size-> %d c->size %d c->top %d 本次初始值\n",size, c->size, c->top);
    if ( c->top + size >= c->size ){// top 位置将超过 限定的高度  栈的容量比较小
        if ( c->size == 0 ){
            printf("\n\n  c->size   T_PARSE_STACK_INIT_SIZE 内存初始化设定为256\n ");
            c->size = T_PARSE_STACK_INIT_SIZE;
            //c->stack = (char*) realloc (c->stack,c->size);--->添加的这个为边判断边分配了
        }// 完全为空
        while ( c->top + size >= c->size ){
            //c->size +=  c->size * 1;
            c->size = c->size + 122;
           // printf("\n----> push end 00 c->size %d top->  %d 打算扩容已经达到临界 \n",c->size ,c->top);
        }//临界
        
        //通过上面确定好 栈的大小后 进行扩容操作
        printf("\n----> push end 01 c->size %d 准备扩容 \n",c->size);
            c->stack = (char*) realloc (c->stack,c->size);//void *realloc(void *ptr, size_t size) ptr -- 指针指向一个要重新分配内存的内存块  size -- 内存块的新的大小，以字节为单位 
        printf("\n----> push end 02 c->size %d  扩容完毕 \n",c->size);
    }
    ret = c->stack + c->top;//在限定内 top直接上移动   起始地址+top  一开始top为0
    c->top += size;//更新大小
    printf("\n----> push end 03 栈顶指针更新完毕 更新为 c->top: %d c->size : %d  \n",c->top,c->size);
    return ret;
}

static void * t_context_pop( t_context *c,size_t size ){
    //size 为信息单元 的高度
    assert( c->top >= size );//为假就会报错 为真就会放行
    return c->stack +( c->top -= size );//下降
}//返回 要弹出栈的 起始位置  并不是真正的出栈 而是 从这个位置开始转移字符 转移完了
//将堆栈变为零  字符串 12345 以从左到右的顺序入栈  栈的起始指针指向 1 栈的top
// （借由1相对指向了5）指向5 



int t_get_boolean(const t_value* v) {
    /* \TODO */
    assert(v != NULL && (v->type == T_TURE || v->type == T_FALSE));
    return v->type == T_TURE;
}

void t_set_boolean(t_value* v, int b) {
    /* \TODO */
    t_free(v);// 清零 全部释放
    v->type = b ? T_TURE : T_FALSE;
}


void t_set_number(t_value* v, double n) {
    /* \TODO */
    t_free(v);
    v->n = n;
    v->type = T_NUMBER;
}

const char* t_get_string(const t_value* v){
    assert(v != NULL && v->type == T_STRING);
    //printf("\n\n t_get_string---->  len  %d \n",v->len);
    return v->s;
}
int  t_get_string_length (const t_value *v ){
    //printf("\n\n t_get_string_length----->  len  %d \n",v->len);
    assert(v != NULL && v->type == T_STRING);
    return v->len;
}

void t_set_string(t_value* v, const char* s, int len){
//printf("\n\n t_set_string->  len 0  %d \n",len);
    assert(v != NULL && (s != NULL || len == 0));
//printf("\n\n\n ------------->t_set_string -1  \n\n");
//printf("\n\n\n -----> %c \n\n",v->s);
//printf("\n\n  json.c t_set_string 258----> test the first %c %c %c \n",v->s[0],v->s[1],v->s[2]);
    t_free(v);
//printf("\n\n\n ------------->t_set_string -2  \n\n");
//printf("\n\n  json.c t_set_string 260----> test the sec %c %c %c \n",v->s[0],v->s[1],v->s[2]);
    v->s = (char*) malloc (len+1);
    
    //第二次参数为 要被复制的指针发的始位置
    //第一个参数为  复制的新位置的指针起始位置
    memcpy(v->s, s, len);//将解析出来的东西生成一个字符串  写入v的s中
    // char ch2 = *(v->s);
    // check_string(&ch2);

    // printf("json.c  t_set_string 311 -> v->s %c ",v->s);
    // getchar();

    
    v->s[len] = '\0';
    //printf("\n\n\n ------------->t_set_string -5  \n\n");
    v->len = len;
    //printf("\n\n\n ------------->t_set_string -6  \n\n");
    v->type = T_STRING;
    //printf("\n\n\n ------------->t_set_string -7  \n\n");

}
//实现 \uXXXX 解析  Unicode

//不合法的十六进位数
static const char *t_parse_hex4(const char *p, unsigned *u){

    int i;
    *u = 0;
    for(i=0; i<4; i++){
        char ch = *p++;
        *u<<=4;

        if      (ch >= '0' && ch <= '9')  *u |= ch - '0';
        else if (ch >= 'A' && ch <= 'F')  *u |= ch - ('A' - 10);
        else if (ch >= 'a' && ch <= 'f')  *u |= ch - ('a' - 10);
        else return NULL;
    }
    return p;
}

static void t_encode_utf8(t_context*c, unsigned u){

    if (u <= 0x7F)
        PUTC(c, u & 0xFF);
    else if(u <= 0x7FF)
    {
        PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
        PUTC(c, 0x80 | ( u       & 0x3F));
    }else if (u <= 0xFFFF) {
        PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
        PUTC(c, 0x80 | ((u >>  6) & 0x3F));
        PUTC(c, 0x80 | ( u        & 0x3F));
    }
    else {
        assert(u <= 0x10FFFF);
        PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
        PUTC(c, 0x80 | ((u >> 12) & 0x3F));
        PUTC(c, 0x80 | ((u >>  6) & 0x3F));
        PUTC(c, 0x80 | ( u        & 0x3F));
    }
    

}
//////////////////////////////////////////////////////////////
//数组处理

int t_get_array_size(const t_value *v){
    assert(v != NULL && v->type == T_ARRAY);
    return v->array_size;
}

t_value *t_get_array_element(const t_value*v ,int index,int flag){
    assert(v!=NULL && v->type == T_ARRAY);
    assert(index < v->array_size);//不要越界
    printf("\n json.c 381 t_get_array_element-> %d,%d  flag %d",&v->array_e[index].type,*(&v->array_e[index].type),flag,"\n");
    //getchar();
    if(flag == 1){
        printf("\njson.c 385  t_get_array_element flag =1  %d \n", &v->array_e[index].n);
        return &v->array_e[index];//返回地址
    }else
    {
        printf("\njson.c 387  t_get_array_element flag =0  %d \n", *(&v->array_e[index].type));
        return &v->array_e[index].type;//返回地址
        
    }
    }

int t_get_array_element_type(const t_value*v ,int index){
    assert(v!=NULL && v->type == T_ARRAY);
    assert(index < v->array_size);//不要越界
    printf("\njson.c 397 t_get_array_element_type-> %d,%d  ",&v->array_e[index].type,*(&v->array_e[index].type),"\n");
    printf("\njson.c 398  t_get_array_element_type flag =0  %d \n", *(&v->array_e[index].type));
    int aa =*(&v->array_e[index].type);
    return aa; //返回地址
}



///////////////////////////////////////////////////////
#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)


//预处理
static int t_parse_string_raw(t_context *c, char** str, int* len ){

    size_t head = c->top ;
    unsigned u, u2;
    const char *p;
    long flag2=0;


    //json---> "\"Hello\\nWorld\""
    //printf("\n\n p[0] %c p[1] %c p[2] %c p[3] %c  \n",c->json[0], c->json[1], c->json[2],c->json[3]);
    //printf("\n\n t_parse_string - > 257:p = c->json;  %c  asic %d 0--> %c - %d \n", c->json, c->json, '\0', '\0');
    printf("\n******字符串检查*******\n");
    t_context ch2=*c;
    check_string(ch2.json);
    expect (c, '\"');// 判断是不是字符串
    p = c->json;
    
    ch2=*c; 
    //char **ch2= &(c->json);
    check_string(ch2.json);
    printf("\njson.c 407  t_parse_string_raw  \n");
    //getchar();

    int flag = 0;
    int count=0;
    //printf("\n\n t_parse_string - > 258:p = c->json;  %c  asic %d 0--> %c - %d \n", p, p, '\0', '\0');
    for ( ; ; )
    {   
        char ch = *p++; // 向后拨动字符
        count= count+1;

        printf("\n\n t_parse_string - > for ->421 :char ch = *p ++; 图形符号：%c  ,asic %d  p-->%p  \n",ch,ch,p);
        switch (ch)
        {
        case '\"' ://检索结束 字符串到头  “ .... ”
            printf("\n\n t_parse_string---> case '\"' 检索结束 字符串到头 \n");
            *len = c->top - head;
            *str = t_context_pop(c , *len);


           // t_set_string (v , (const char *)t_context_pop(c , len), len);
            c->json = p;
            return T_PARSE_OK;//字符串解析完整的结束了
        
        case '\\':/*555 --> \ 单个斜杠 555*/
            printf("\n\n 384 ------->  work %d \n",p);
            //flag2 = p ;
            flag = 1;//---->1
            
            //char ch3= ; /* 看 \x x是啥 同时下一一个 */
            switch(*p++){//转义序列的解析
                
                    case '\"': printf("\n\n 384 ------->  work 403-1 p-> %p \n",p); PUTC(c, '\"'); break;
                    case '\\':printf("\n\n 384 ------->  work 404-2  p-> %p \n",p); PUTC(c, '\\'); break;
                    case '/':printf("\n\n 384 ------->  work /  p-> %p \n",p);  PUTC(c, '/' ); break;
                    case 'b':printf("\n\n 384 ------->  work b p-> %p \n",p);  PUTC(c, '\b'); break;
                    case 'f':printf("\n\n 384 ------->  work f p-> %p \n",p);  PUTC(c, '\f'); break;
                    case 'n':printf("\n\n 384 ------->  work n p-> %p \n",p);  PUTC(c, '\n');printf("384 ------->  work n p-> %p  count %d \n",p,count); break;
                    case 'r':printf("\n\n 384 ------->  work r p-> %p \n",p);  PUTC(c, '\r'); break;
                    case 't':printf("\n\n 384 ------->  work t p-> %p \n",p);  PUTC(c, '\t'); break;
                    case 'u':printf("\n\n 384 ------->  work u p-> %p \n",p);
                        if(!( p= t_parse_hex4(p, &u)))
                                STRING_ERROR(T_PARSE_INVALID_UNICODE_HEX);

                                if (u >= 0xD800 && u <= 0xDBFF) { /* surrogate pair */
                            printf("\n\n ------> add\n\n ");
                            if (*p++ != '\\')
                                STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
                            if (*p++ != 'u')
                                STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
                            if (!(p = t_parse_hex4(p, &u2)))
                                STRING_ERROR(T_PARSE_INVALID_UNICODE_HEX);
                            if (u2 < 0xDC00 || u2 > 0xDFFF)
                                STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }

                        t_encode_utf8(c,u);
                        break;

                    case '\0':
                        STRING_ERROR(T_PARSE_MISS_QUOTATION_MARK);


                    default:
                        c->top = head;
                        STRING_ERROR(T_PARSE_INVALID_STRING_ESCAPE);

                        //c->top = head;
                        // return T_PARSE_INVALID_STRING_ESCAPE;
            }


        case '\0' :
                printf("\n 425 ->t_parse_string--->  0-->  T_PARSE_MISS_QUOTATION_MARK \n");        
                printf("\n\nch-> %c  stand-> %c \n",ch,'\0');
                printf("\n\nd->  %d  stand-> %d \n",ch,'\0');
                //printf("\n\n  flag2: %p p: %p   count : %d  \n\n  ",flag2,p,count);
                
                if(flag != 0 ){
                    flag = 0;
                    //printf()
                }else
                {
                    printf("\n 425 ->t_parse_string--->  0-->  T_PARSE_MISS_QUOTATION_MARK \n");
                    STRING_ERROR(T_PARSE_MISS_QUOTATION_MARK);
                }
                
                
                break;

        default:
            printf("\n\n t_parse_string---> default : return T_PARSE_INVALID_STRING_CHAR  \n");
            if ((unsigned char)ch < 0x20 )//进行字符检查 不符合条件的 不能通过
            {
                printf("\n 432  t_parse_string ->if char-> %s ch-> %p , loss %d \n",ch,(unsigned char)ch,((unsigned char)ch-0x20));
                c->top = head;
                return T_PARSE_INVALID_STRING_CHAR;
            }
            PUTC(c,ch);
            printf("\n json.c 510 t_parse_string--->->default 进行入栈操作！\n");
            ch2=*c;
            check_string(ch2.json);
        
        }
        printf("\n json.c 513 t_parse_string---->switch 结束 注意查看分类结果!!\n\n\n");
        ch2=*c;
        check_string(ch2.json);
        //getchar();
    }

}


static int t_parse_string (t_context* c, t_value* v ){
    //t_set_string (v , (const char *)t_context_pop(c , len), len);

    int ret;
    char *s;
    int len;
    if ((ret=t_parse_string_raw(c, &s, &len)) == T_PARSE_OK )
    {
        t_set_string(v, s, len);
    }
    return ret; 
    
    
}


// static int t_parse_string (t_context* c, t_value* v ){
//     size_t head = c->top ,len;
//     unsigned u, u2;
//     const char *p;
//     long flag2=0;


//     //json---> "\"Hello\\nWorld\""
//     //printf("\n\n p[0] %c p[1] %c p[2] %c p[3] %c  \n",c->json[0], c->json[1], c->json[2],c->json[3]);
//     //printf("\n\n t_parse_string - > 257:p = c->json;  %c  asic %d 0--> %c - %d \n", c->json, c->json, '\0', '\0');
//     expect (c, '\"');// 判断是不是字符串
//     p = c->json;
    
//     t_context ch2=*c; 
//     //char **ch2= &(c->json);
//     check_string(ch2.json);
//     int flag = 0;
//     int count=0;
//     //printf("\n\n t_parse_string - > 258:p = c->json;  %c  asic %d 0--> %c - %d \n", p, p, '\0', '\0');
//     for ( ; ; )
//     {   
//         char ch = *p++; // 向后拨动字符
//         count= count+1;

//         printf("\n\n t_parse_string - > for ->361 :char ch = *p ++;  %c ,asic %d  p-->%p  \n",ch,ch,p);
//         switch (ch)
//         {
//         case '\"' ://检索接受 字符串到头  “ .... ”
//             printf("\n\n t_parse_string---> case '\"'  \n");
//             len = c->top - head;

//             t_set_string (v , (const char *)t_context_pop(c , len), len);
//             c->json = p;
//             return T_PARSE_OK;//字符串解析完整的结束了
        
//         case '\\':/*555 --> \ 单个斜杠 555*/
//             printf("\n\n 384 ------->  work %d \n",p);
//             //flag2 = p ;
//             flag = 1;//---->1
            
//             //char ch3= ; /* 看 \x x是啥 同时下一一个 */
//             switch(*p++){//转义序列的解析
                
//                     case '\"': printf("\n\n 384 ------->  work 403-1 p-> %p \n",p); PUTC(c, '\"'); break;
//                     case '\\':printf("\n\n 384 ------->  work 404-2  p-> %p \n",p); PUTC(c, '\\'); break;
//                     case '/':printf("\n\n 384 ------->  work /  p-> %p \n",p);  PUTC(c, '/' ); break;
//                     case 'b':printf("\n\n 384 ------->  work b p-> %p \n",p);  PUTC(c, '\b'); break;
//                     case 'f':printf("\n\n 384 ------->  work f p-> %p \n",p);  PUTC(c, '\f'); break;
//                     case 'n':printf("\n\n 384 ------->  work n p-> %p \n",p);  PUTC(c, '\n');printf("384 ------->  work n p-> %p  count %d \n",p,count); break;
//                     case 'r':printf("\n\n 384 ------->  work r p-> %p \n",p);  PUTC(c, '\r'); break;
//                     case 't':printf("\n\n 384 ------->  work t p-> %p \n",p);  PUTC(c, '\t'); break;
//                     case 'u':printf("\n\n 384 ------->  work u p-> %p \n",p);
//                         if(!( p= t_parse_hex4(p, &u)))
//                                 STRING_ERROR(T_PARSE_INVALID_UNICODE_HEX);

//                                 if (u >= 0xD800 && u <= 0xDBFF) { /* surrogate pair */
//                             printf("\n\n ------> add\n\n ");
//                             if (*p++ != '\\')
//                                 STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
//                             if (*p++ != 'u')
//                                 STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
//                             if (!(p = t_parse_hex4(p, &u2)))
//                                 STRING_ERROR(T_PARSE_INVALID_UNICODE_HEX);
//                             if (u2 < 0xDC00 || u2 > 0xDFFF)
//                                 STRING_ERROR(T_PARSE_INVALID_UNICODE_SURROGATE);
//                             u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
//                         }

//                         t_encode_utf8(c,u);
//                         break;

//                     case '\0':
//                         STRING_ERROR(T_PARSE_MISS_QUOTATION_MARK);


//                     default:
//                         c->top = head;
//                         STRING_ERROR(T_PARSE_INVALID_STRING_ESCAPE);

//                         //c->top = head;
//                         // return T_PARSE_INVALID_STRING_ESCAPE;
//             }


//         case '\0' :
//                 printf("\n 425 ->t_parse_string--->  0-->  T_PARSE_MISS_QUOTATION_MARK \n");        
//                 printf("\n\nch-> %c  stand-> %c \n",ch,'\0');
//                 printf("\n\nd->  %d  stand-> %d \n",ch,'\0');
//                 //printf("\n\n  flag2: %p p: %p   count : %d  \n\n  ",flag2,p,count);
                
//                 if(flag != 0 ){
//                     flag = 0;
//                     //printf()
//                 }else
//                 {
//                     printf("\n 425 ->t_parse_string--->  0-->  T_PARSE_MISS_QUOTATION_MARK \n");
//                     STRING_ERROR(T_PARSE_MISS_QUOTATION_MARK);
//                 }
                
                
//                 break;

//         default:
//             printf("\n\n t_parse_string---> default : return T_PARSE_INVALID_STRING_CHAR  \n");
//             if ((unsigned char)ch < 0x20 )//进行字符检查 不符合条件的 不能通过
//             {
//                 printf("\n 432  t_parse_string ->if char-> %s ch-> %p , loss %d \n",ch,(unsigned char)ch,((unsigned char)ch-0x20));
//                 c->top = head;
//                 return T_PARSE_INVALID_STRING_CHAR;
//             }
//             PUTC(c,ch);
//         }
//     }
// }




///////////////////////////////////////////////
static int t_parse_value(t_context* c, t_value* v,int flag);//先给下面的声明一下 下面的呢个函数 【t_parse_array】要用
//这两个函数有点 互相调用的意思 但是 要调用的话必须要出现在前面所以 前项 声明一下
static int t_parse_array(t_context *c, t_value*v ){
    int size=0;
    int ret;
    expect(c,'[');//["abc",[1,2],3]--->判定为数组

    t_parse_ws(c);// 解析空白字符  解析出来是空的将json指针 相向后移动直到不是为空为止

    if (*c->json == ']'){// [] 完了为空
        c->json++ ;
        v->type = T_ARRAY;
        v->array_size = 0;
        v->array_e=NULL; 
        return T_PARSE_OK;
    }
    // 如果为 【1，【2,3】，4】----》解析第二个 【时 会 中断再次调用该函数 所以知道处理完这个后再执行第一个
    //同过下面的e把它们都串起来  进入第二层就变为了 v 从而  memcpy(v->array_e = (t_value*)malloc(size), t_context_pop(c, size), size);//连接
    //
   // t_context* tmp= t_contex_push(c, sizeof(t_value));

    printf("\n\n 0 json.c 660  t_parse_array ->  c->size %d c->top %d \n", c->size ,c->top);

    for ( ; ; )
    {
        t_value e;
        
        t_init(&e);

        printf("\n\n 1 json.c 667  t_parse_array ->  c->size %d c->top %d \n", c->size ,c->top);
        //t_context tmp = *c;

        if((ret = t_parse_value(c,&e,1)) != T_PARSE_OK) break;//这里会对 c的进行刷新--->进入主调用在进行分类
        printf("----------->e %d, %d ,%d ,%d ",e.type,e.s,e.n,e.len);
        //getchar();
        printf("\n\n 1.5 json.c 671  t_parse_array ->  c->size %d c->top %d \n", c->size ,c->top);

        
        
        memcpy(t_contex_push(c, sizeof(t_value)), &e, sizeof(t_value));
        //memcpy(t_contex_push(c, sizeof(t_value)), &e, sizeof(t_value));
        size++;

        t_parse_ws(c);// 解析空白字符

        if(*c->json == ',') {
            c->json++;//继续处理 继续解析
            t_parse_ws(c);//  解析空白字符 看上面向后移动的是不是 空字符串 是的话 继续向后拨动 

            printf("\n\n 3 json.c 680  t_parse_array ->  c->size %d c->top %d \n", c->size ,c->top);

            }
        else if (*c->json == ']'){//解析结束
            c->json++;
            v->type = T_ARRAY;
            v->array_size = size;
            size *= sizeof(t_value);
            memcpy(v->array_e = (t_value*)malloc(size), t_context_pop(c, size), size);//连接
            
            printf("\n\n json.c 696 t_parse_array-> 解析结束 %d * %d * %d * %d \n",v->array_e[0].type,v->array_e[1].type,v->array_e[2].type,v->array_e[3].type,v->array_e[4].type);
            //getchar();
            
            return T_PARSE_OK;
        } 

        else {
            return T_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
            }

        printf("\n\n 4 json.c 697  t_parse_array ->  c->size %d c->top %d \n", c->size ,c->top);

    }

    //出栈  数组已经被放入 v->array_e  指针中
    //将c释放掉
    for(int i=0; i<size ; i++){
        t_free((t_value*)t_context_pop(c, sizeof(t_value) ));
        return ret;
    }

    getchar();
    
}
//t_parse_value




///////////////////////////////////////////// 对象 object
static int t_parse_object(t_context* c, t_value* v){

    int size;
    
    t_object_member m;
    int ret;

    expect(c,'{');
    t_parse_ws(c);
    if(*c->json == "}"){
        c->json++;
        v->type = T_OBJ;
        v->obj_member=0;
        v->obj_size=0;
        return 0;
    }

    //normal
    m.key_value_string=NULL;
    size = 0;
    for(;;){

        t_init(&m.v);
        if((ret= t_parse_value(c,&m.v,0)) != T_PARSE_OK)break;
        t_parse_ws(c);
        memcpy(t_contex_push(c,sizeof(t_object_member)), &m, sizeof(t_object_member));
        size++;
        m.key_value_string = NULL;
        

    }
    return ret;



}


static int t_parse_obj(t_context*c, t_value* v){
    //语法格式 分析
    //member = string ws %x3A ws value
    //object = %x7B ws [ member *( ws %x2C ws member ) ] ws %x7D
    // %x3A-->U+003A-> :  Unicode编码解释

    int i,size;
    t_object_member m;
    int ret;

    expect(c,'{');
    t_parse_ws(c);

    if (*c->json == '}')//为空的情况
    {
        c->json++;
        v->type = T_OBJ;
        v->obj_member = 0;
        v->obj_size = 0;
    }

    m.key_length = NULL;//处理正常的进行相关的初始化
    size=0;
    for (;;)
    {
        char* str;
        t_init(&m.v);

        if(*c->json == '"'){// err->{""}
            ret = T_PARSE_MISS_KEY;
            break;
        }

        if((ret = t_parse_string_raw(c,&m,m.key_length) )!= T_PARSE_OK )break;
        
        memcpy( (m.key_value_string=(char*)malloc(m.key_length+1) ),str,m.key_length );
        m.key_value_string[m.key_length]='\0';//添加最后一个字符

        t_parse_ws(c);

        if(*c->json != ':'){
            ret = T_PARSE_MISS_COLON;
            break;
        }

        c->json++;
        t_parse_ws(c);

        if( (ret = t_parse_value(c,&m.v,0)) != T_PARSE_OK ){
            break;
        }
        memcpy((t_contex_push(c,sizeof(t_object_member))) ,&m,sizeof(t_object_member));
        size++;
        m.key_value_string = NULL;
        t_parse_ws(c);

        if(c->json = ','){
            c->json++;
            t_parse_ws(c);
        }
        else if(c->json == '}'){
            int s = sizeof(t_object_member)*size;
            c->json ++ ;
            v->type=T_OBJ;
            memcpy(v->obj_member = (t_object_member*)malloc(s),t_contex_push(c,s),s);
            return T_PARSE_OK;
        }

        else
        {
            ret = T_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
            break;
        }

        free(m.key_value_string);
        for (int i = 0; i < size; i++)
        {
            t_object_member *m = (t_object_member*)t_context_pop(c,sizeof(t_object_member));
            free(&m->v);
            t_free(&m->v);
        }

        v->type = T_NULL;
        return ret;
        
    }
    
    


}



/////////////////////////////////////////////////////////////

///// 一掉部分工具函数写完 下面从37-68 都是对上面的调用与封装




static int t_parse_value(t_context *t , t_value *v, int flag ){//解析值
    if (flag == 1)
    {
        printf("\n\njson.c 863 -> t_parse_value 不会对其进行初始化 \n");
    }
    else
    {
        t_init2(t);
    }
    
    
    
    //char str[40];
    //sprintf(str," *t->json ,%s  *v , %s \n ",*t->json,*v);
    //printf("\n *t->json ,%s   \n",*t->json);
    switch (*t->json)

    {
    case 'n': // case "n" 就不行 因为不是常量
        printf("\n  ------> 1");
        //return t_parse_null(t,v);
        return t_parse_literal (t, v, "null", T_NULL);
        // break;

    case '\0': // case "n" 就不行 因为不是常量
    printf("\n  ------>  json.c  2 \n");
        return T_PARSE_EXPECT_VALUE; //任何字符串的背后都是 \0
    
    case 32: // 空格的阿斯克码为32
    printf("\n  ------> json.c 3 \n");
        return T_PARSE_EXPECT_VALUE; //任何字符串的背后都是 \0
    
        // break; 一个 JSON 只含有空白
    case 't':
    printf("\n  ------>  json.c 4 \n");
        //return t_parse_true(t,v);
        return t_parse_literal (t, v, "true", T_TURE);

    case 'f':
    printf("\n  ------> json.c 5 \n");
        //return t_parse_false(t,v);
        return t_parse_literal (t, v, "false", T_FALSE);
    case '"' :
    printf("\n  ------> json.c 6 \n");
        return t_parse_string(t, v);
    
    case '[':
    printf("\n ------> json.c 7\n");
        return t_parse_array(t, v);
    
    case '{':  
    printf("\n ------> json.c 8\n");
    return t_parse_object(t, v);
    
    default:
    printf("\n  ------> json.c 7");
        return t_parse_number(t, v);
        //break;不是那三种字面值
    }
}


/////////////////////////////////////////////


////////////////////////////////////////////////



int t_parse(t_value *v, const char *json){//t_parse(&v,"null")

    t_context c;
    //  error--> json---> "\"Hello\\nWorld\""
    printf("\njson.c get -->num : %d, string : %c  \n ",*json,*json);
    assert( v !=NULL );
    c.json =json;
    v->type=T_NULL;

    int ret= t_parse_value(&c,v,0);

    //printf("\n___1 ret--> %d ___",ret);

    if ( ret == T_PARSE_OK)//虽然有很多的函数 要判断很多的类型但是都是有统一的调用入口 和 统一的出口 出口的东西都一样（指的是正确的情况下）
    //但是在错误的情况下返回的都是各自领域的错误
    {
        t_parse_ws(&c);
        ret=t_parse_end_not_null(&c);
    }
    //printf("2 ret--> %d",ret);
    return ret;
    
}

t_type t_get_type(const t_value *v){
    assert(v!=NULL);
    //printf("\n t_get_type--->%d \n",v->type);
    return v->type;
}

double t_get_number(const t_value *v){
    //printf(" t_get_number  %lf \n",v->n);
    assert(v != NULL && v->type == T_NUMBER);
    return v->n;
}

// t_value* t_get_array_element(const t_value* v, int index) {
//     assert(v != NULL && v->type == T_ARRAY);
//     assert(index < v->array_size);
//     return &v->array_e[index];
// }



int t_get_object_size(const t_value* v) {
    assert(v != NULL && v->type == T_OBJ);
    return v->obj_size;
}

const char* t_get_object_key(const t_value* v, int index) {
    assert(v != NULL && v->type == T_OBJ);
    assert(index < v->obj_size);
    //return v->u.o.m[index].k;
    return v->obj_member[index].key_value_string;
}

int t_get_object_key_length(const t_value* v, int index) {
    assert(v != NULL && v->type == T_OBJ);
   //assert(index < v->u.o.size);
    assert(index < v->obj_size);
    //return v->u.o.m[index].klen;
    return v->obj_member[index].key_length;
}

t_value* t_get_object_value(const t_value* v, int index) {
    assert(v != NULL && v->type == T_OBJ);
    assert(index < v->obj_size);
    //return &v->u.o.m[index].v;
    return &v->obj_member[index].v;
}

//E:\the_c_of_vs_code\c\c_project\json>gcc -c json.c
