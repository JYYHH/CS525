#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ll long long
#define M 1000000

int at_tree[M + 5];

int main(int argc, char **argv){
    // Insert : Delete : look_up ~= 2 : 1 : 1
    // Delete Empty (not in the tree) Pr{} ~= 1 / 16
    srand(time(0));
    int N = atoi(argv[1]), m = atoi(argv[2]);
    int opt, val;
    while(N--){
        opt = rand() & 0xf;
        val = rand() % m + 1;
        if (opt >= 8){
            if (at_tree[val]){
                N++;
                continue;
            }
            at_tree[val] = 1;
            printf("%d %d\n", 1, val);
        }
        else if (opt >= 4){
            if (!at_tree[val] && ((rand() & 0xf) != 0xf)){
                // control the Delete empty rate into 1/16
                N++;
                continue;
            }
            at_tree[val] = 0;
            printf("%d %d\n", 2, val);
        }
        else
            printf("%d %d\n", 3, val);
    }
    return 0;
}