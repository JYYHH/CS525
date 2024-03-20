#include <stdlib.h>
#include <stdio.h>

int main(){
    int cnt = 0;
    while(1){
        system("./c.out 300000 10000 > in.in");
        system("./a.out < in.in > a.ans");
        system("./b.out < in.in > b.ans");
        if (system("diff a.ans b.ans")){
            printf("output diff!\n");
            return 0;
        }
        printf("OK round %d\n", ++cnt);
    }

    return 0;
}