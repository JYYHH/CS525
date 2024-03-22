#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define ll long long

struct node{
    struct node *left, *right;
    pthread_mutex_t *subtree_pass;
        // retaining this lock, means that PRESENT THREAD have the possibility to modify ANY node's info inside this subtree
        // releasing this lock, means that we gain more info to make sure the possibility MUST happen in left of right child subtree
    int val, tag;
};

/*
    Thread Control Global Vars Definition
*/
#define MAX_THREAD_COUNT 1
pthread_t *thread_arr;
pthread_mutex_t *read_mutex;
struct node *root;

struct node *new_node(int value){
    struct node *ret = (struct node *) malloc(sizeof(struct node));
    ret->left = ret->right = NULL;
    ret->val = value;
    ret->tag = 0;
    ret->subtree_pass = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(ret->subtree_pass, NULL);
    return ret;
}

// avoid the memory leak
void free_node(struct node *v_r){
    pthread_mutex_destroy(v_r->subtree_pass);
    free(v_r);
}

void free_tree(struct node *v_r){
    if (v_r == NULL)
        return;
    free_tree(v_r->left);
    free_tree(v_r->right);
    free_node(v_r);
}

void print_off(int offset){
    while(offset--) putchar(' ');
}

struct node **get_fa2chd_pointer(struct node *v_r_fa, int child){
    if (child == 0)
        return &(v_r_fa->left);
    else 
        return &(v_r_fa->right);
}

void enter_function(struct node * v_r, struct node * v_r_fa){
    pthread_mutex_lock(v_r->subtree_pass);
    pthread_mutex_unlock(v_r_fa->subtree_pass);
}

// useless for this simple dfs pattern
// void exit_function(struct node * v_r, struct node * v_r_fa){
//     pthread_mutex_unlock(v_r->subtree_pass);
//     pthread_mutex_lock(v_r_fa->subtree_pass);
// }

void display(struct node *v_r, int offset){
    print_off(offset), printf("Present subtree begin---->\n");
    print_off(offset), printf("Root value = %d\n", v_r->val);
    print_off(offset), printf("Left sub tree:\n");
    if (v_r->left) display(v_r->left, offset + 2);
    else puts("");
    print_off(offset), printf("Right sub tree:\n");
    if (v_r->right) display(v_r->right, offset + 2);
    else puts("");
    print_off(offset), printf("Present subtree end---->\n");
}

int look_up(struct node *v_r, int value){
    if (v_r == NULL)
        return 0;
    if (value < v_r->val)
        return look_up(v_r->left, value);
    else if (value > v_r->val)
        return look_up(v_r->right, value);
    else
        return v_r->tag ^ 1;
}

void Insert(struct node *v_r, int value, struct node *v_r_fa, int child){
    if (v_r == NULL){
        // use the lock of father to protect the insert
        *get_fa2chd_pointer(v_r_fa, child) = new_node(value);
        pthread_mutex_unlock(v_r_fa->subtree_pass); // important, don't forget to release the father's lock
        return;
    }

    // lock transfer from father to here
    enter_function(v_r, v_r_fa);
    
    if (value < v_r->val)
        Insert(v_r->left, value, v_r, 0);
    else if (value > v_r->val)
        Insert(v_r->right, value, v_r, 1);
    else{
        v_r->tag = 0;
        pthread_mutex_unlock(v_r->subtree_pass);
    }

    // no need code below anymore, for the easy pattern (one link through root to the target node) of this function
    // // lock transfer from here to father
    // exit_function(v_r, v_r_fa);
}

void Delete(struct node *v_r, int value, struct node *v_r_fa){
    if (v_r == NULL){
        pthread_mutex_unlock(v_r_fa->subtree_pass); // important, don't forget to release the father's lock
        return;
    }

    // lock transfer from father to here
    enter_function(v_r, v_r_fa);

    if (value < v_r->val)
        Delete(v_r->left, value, v_r);
    else if (value > v_r->val)
        Delete(v_r->right, value, v_r);
    else{
        v_r->tag = 1;
        pthread_mutex_unlock(v_r->subtree_pass);
    }

    // no need code below anymore, for the easy pattern (one link through root to the target node) of this function
    // // lock transfer from here to father
    // exit_function(v_r, v_r_fa);
}

void *handler(void *null){
    int opt, val;
    while(1){
        // try to gain the read lock
        pthread_mutex_lock(read_mutex);
        if (scanf("%d %d", &opt, &val) != 2){
            pthread_mutex_unlock(read_mutex);
            break;
        }
        pthread_mutex_unlock(read_mutex);

        if (opt == 1){
            // Insert
            pthread_mutex_lock(root->subtree_pass);
            Insert(root->right, val, root, 1);
        }
        else if (opt == 2){
            // Delete
            pthread_mutex_lock(root->subtree_pass);
            Delete(root->right, val, root);
        }
        else if (opt == 3){
            // look_up
            printf("%d\n", look_up(root->right, val));
        }
    }
}

int main(){
    // allocate the space for the global thread control variables
    thread_arr = (pthread_t *) malloc(sizeof(pthread_t) * MAX_THREAD_COUNT);
    read_mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    // init mutex
    pthread_mutex_init(read_mutex, NULL);
    // init the global minimum virtual root
    root = new_node(~0);
    
    // feed all the thread in thread_pool with an initial routine
    for (int i = 0; i < MAX_THREAD_COUNT; i++)
        pthread_create(thread_arr + i, NULL, handler, NULL);

    // clean-up, in an out-stack order
    for (int i = 0; i < MAX_THREAD_COUNT; i++)
        while (pthread_join(thread_arr[i], NULL));
    free_tree(root);
    pthread_mutex_destroy(read_mutex);
    free(thread_arr);

    return 0;
}