#include <stdlib.h>
#include <stdio.h>

int main(){
    int cnt = 0;
    while(1){
        system("./bin/datagen.run 300000 10000 > ./data/in.in");
        system("./bin/a.out < ./data/in.in > ./data/lazy.ans");
        system("./bin/b.out < ./data/in.in > ./data/normal.ans");
        if (system("diff ./data/lazy.ans ./data/normal.ans")){
            printf("output diff!\n");
            return 0;
        }
        printf("OK round %d\n", ++cnt);
    }

    return 0;
}