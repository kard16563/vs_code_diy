//s_gdb_debug_watchpoint.cpp 
//E:\the_c_of_vs_code\c\c_study>gcc -g s_gdb_debug_watchpoint.cpp -o deb2
#include<stdio.h>

int main( void ){
    int sum=0,i=0;
    char input[5];

    while (1)
    {
        sum=0;
        scanf("%s",input);
        for ( i = 0; input[i] != '\0'; i++)
        {
            sum=sum*10 + input[i]-'0';
        }
        printf("input=%d\n",sum);
        
    }
    return 0;
    


}