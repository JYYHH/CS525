#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ll long long

struct node{
    struct node *left, *right;
    int val, size;
    int tag, tag_size;
};

/*
    Part 1: Useful functions
*/

int get_size(struct node *v_r){
    return v_r == NULL ? 0 : v_r->size;
}

int get_tag_size(struct node *v_r){
    return v_r == NULL ? 0 : v_r->tag_size;
}

int get_real_size(struct node *v_r){
    return get_size(v_r) - get_tag_size(v_r);
}

void _maintain(struct node *v_r){
    v_r->size = get_size(v_r->left) + get_size(v_r->right) + 1;
    v_r->tag_size = get_tag_size(v_r->left) + get_tag_size(v_r->right) + v_r->tag;
}

int need_reconstruct(struct node *v_r){
    return 
        (
            abs(
                get_real_size(v_r->left) 
                    - 
                get_real_size(v_r->right)
            ) 
                * 
            3 
                >= 
            get_real_size(v_r)
        )
        ||
        (
            get_tag_size(v_r) 
                * 
            2 
                >= 
            get_size(v_r)
        );
}

void print_off(int offset){
    while(offset--) putchar(' ');
}

void display(struct node *v_r, int offset){
    print_off(offset), printf("Present subtree begin---->\n");
    print_off(offset), printf("Root value = %d\n", v_r->val);
    print_off(offset), printf("Subtree size = %d\n", v_r->size);
    print_off(offset), printf("Tag value = %d\n", v_r->tag);
    print_off(offset), printf("Subtree TAG size = %d\n", v_r->tag_size);
    print_off(offset), printf("Left sub tree:\n");
    if (v_r->left) display(v_r->left, offset + 2);
    else puts("");
    print_off(offset), printf("Right sub tree:\n");
    if (v_r->right) display(v_r->right, offset + 2);
    else puts("");
    print_off(offset), printf("Present subtree end---->\n");
}

/*
    Part 2: Q_1.1
*/

void dfs_up(struct node *v_r, int *val_list, int *cnt_pt){
    if (v_r->left)
        dfs_up(v_r->left, val_list, cnt_pt);
    if (v_r->tag == 0){
        val_list[*cnt_pt] = v_r->val;
        (*cnt_pt) ++;
    }
    if (v_r->right)
        dfs_up(v_r->right, val_list, cnt_pt);
    // recycle useless memory
    free(v_r);
}

struct node *re_build(int *val_list, int cnt){
    struct node *ret = (struct node *) malloc(sizeof(struct node));
    ret->size = 1;
    ret->val = val_list[cnt >> 1];
    ret->tag = 0;
    ret->tag_size = 0;

    if (cnt >= 2)
        ret->left = re_build(val_list, cnt >> 1);
    else
        ret->left = NULL;
    if (cnt >= 3)
        ret->right = re_build(val_list + (cnt >> 1) + 1, (cnt - 1) >> 1);
    else 
        ret->right = NULL;
    
    _maintain(ret);
    return ret;
}

struct node *reconstruct(struct node *v_r){
    /*
        In my implementation, a tree is seen balanced only when 0 <= T_l - T_r <= 1
            for every node in this tree.
        It can be proved for any value list, there's only one tree satisfied this conditions.
        So I rebuild all the input trees, EVEN for that only balanced one (in my definition).
            But that one will remain identical after the rebuilding..

        update: ADD SUPPORT FOR THE TAGGED TREE
    */

    // empty sub tree
    if (get_real_size(v_r) == 0)
        return NULL;

    int *val_list = (int *) malloc(sizeof(int) * get_real_size(v_r));
    int cnt = 0;
    struct node *ret;

    // step 1: go through the sub-tree to record all the values
    dfs_up(v_r, val_list, &cnt);
    // step 2: rebuild the sub_tree
    ret = re_build(val_list, cnt);
    // step 3: clean up
    free(val_list);

    return ret;
}

struct node *Lazy_Insert(struct node *v_r, int value){ // in this hw, values will be different from each other
    // empty place on a tree, just build it
    if (v_r == NULL){
        struct node *tmp = (struct node *) malloc(sizeof(struct node));
        tmp->val = value;
        tmp->size = 1;
        tmp->left = tmp->right = NULL;
        tmp->tag = 0;
        tmp->tag_size = 0;
        return tmp;
    }
    // not empty
    if (value <= v_r->val)
        v_r->left = Lazy_Insert(v_r->left, value);
    else
        v_r->right = Lazy_Insert(v_r->right, value);
    _maintain(v_r);

    // possible reconstruct
    if (need_reconstruct(v_r))
        return reconstruct(v_r);
    else
        return v_r;
}

/*  
    Part 3: Q_1.2
*/

struct node *Lazy_Delete(struct node *v_r, int value){ // there's possiblity that value is not in this whole tree..
    if (v_r == NULL)
        return v_r;
    if (value < v_r->val)
        v_r->left = Lazy_Delete(v_r->left, value);
    else if (value > v_r->val)
        v_r->right = Lazy_Delete(v_r->right, value);
    else{
        // should delete this node, if it's still here?
        // can be tag not only once, besides the first time it should be identical to nothing.
        v_r->tag = 1;
    }
    _maintain(v_r);

    // possible reconstruct
    if (need_reconstruct(v_r))
        return reconstruct(v_r);
    else
        return v_r;
}

/*
    Part 4: Q_1.3
*/

int look_up(struct node *v_r, int value){
    if (v_r == NULL)
        return 0;
    if (value < v_r->val)
        return look_up(v_r->left, value);
    else if (value > v_r->val)
        return look_up(v_r->right, value);
    else
        return 1 - v_r->tag;
}

int main(){
    struct node *root;
    /*
        Q_1.1
    */
    root = Lazy_Insert(root, 8);
    root = Lazy_Insert(root, 7);
    root = Lazy_Insert(root, 9);
    root = Lazy_Insert(root, 1);
    root = Lazy_Insert(root, 5);
    root = Lazy_Insert(root, 2);
    root = Lazy_Insert(root, 6);
    root = Lazy_Insert(root, 3);
    root = Lazy_Insert(root, 4);
    root = Lazy_Insert(root, 0);

    // display(root, 0);


    /*
        Q_1.2
    */
    root = Lazy_Delete(root, 7);
    root = Lazy_Delete(root, 0);
    root = Lazy_Delete(root, 6);
    root = Lazy_Delete(root, 2);
    root = Lazy_Delete(root, 10);

    // display(root, 0);

    /*
        Q_1.3
    */
    printf("%d\n", look_up(root, 1));
    printf("%d\n", look_up(root, 6));
    printf("%d\n", look_up(root, 10));
    printf("%d\n", look_up(root, 5));
    printf("%d\n", look_up(root, 8));
    printf("%d\n", look_up(root, 9));

    // /*
    //     My Speed Test
    // */
    // int N = 4 * 1e5;
    // for (int i = 1; i <= N; i++) 
    //     root = Lazy_Insert(root, i);
    // for (int i = N/2; i <= N - 100; i++)
    //     root = Lazy_Delete(root, i);
    // int cnt = 0;
    // for (int i = 1; i <= N; i++){
    //     cnt += (look_up(root, i)!=(i < N/2 || i > N - 100));
    // }
    // printf("%d\n", cnt);

    return 0;
}