#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ll long long

struct node{
    struct node *left, *right;
    int val;
};

struct node *new_node(int value){
    struct node *ret = (struct node *) malloc(sizeof(struct node));
    ret->left = ret->right = NULL;
    ret->val = value;
}

void print_off(int offset){
    while(offset--) putchar(' ');
}

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
        return 1;
}

struct node *Insert(struct node *v_r, int value){
    if (v_r == NULL)
        return new_node(value);
    if (value <= v_r->val)
        v_r->left = Insert(v_r->left, value);
    else
        v_r->right = Insert(v_r->right, value);
    return v_r;
}

struct node *Delete(struct node *v_r, int value){
    if (v_r == NULL)
        return v_r;
    if (value < v_r->val)
        v_r->left = Delete(v_r->left, value);
    else if (value > v_r->val)
        v_r->right = Delete(v_r->right, value);
    else{
        // find the node to be deleted
        if (!v_r->right){
            // case 1: Root has no right child
            struct node *to_delete = v_r;
            v_r = v_r->left;
            free(to_delete);
        }
        else{
            struct node *nxt, *nxt_parent;
            nxt_parent = v_r, nxt = nxt_parent->right;
            while(nxt->left){
                nxt_parent = nxt;
                nxt = nxt->left;
            }
            // case 2: nxt is just the right son of v_r;
            if (nxt_parent == v_r)
                nxt_parent->right = nxt->right;
            // case 3: otherwise
            else
                nxt_parent->left = nxt->right;
            v_r->val = nxt->val;
            free(nxt);
        }
    }

    return v_r;
}

void handler(){
    struct node *root;
    int opt, val;
    while(scanf("%d %d", &opt, &val) == 2){
        if (opt == 1){
            // Insert
            root = Insert(root, val);
        }
        else if (opt == 2){
            // Delete
            root = Delete(root, val);
        }
        else if (opt == 3){
            // look_up
            printf("%d\n", look_up(root, val));
        }
    }
}

int main(){
    // struct node *root;
    // /*
    //     Q_1.1
    // */
    // root = Insert(root, 8);
    // root = Insert(root, 7);
    // root = Insert(root, 9);
    // root = Insert(root, 1);
    // root = Insert(root, 5);
    // root = Insert(root, 2);
    // root = Insert(root, 6);
    // root = Insert(root, 3);
    // root = Insert(root, 4);
    // root = Insert(root, 0);

    // // display(root, 0);


    // /*
    //     Q_1.2
    // */
    // root = Delete(root, 7);
    // root = Delete(root, 0);
    // root = Delete(root, 6);
    // root = Delete(root, 2);
    // root = Delete(root, 10);

    // // display(root, 0);

    // /*
    //     Q_1.3
    // */
    // printf("%d\n", look_up(root, 1));
    // printf("%d\n", look_up(root, 6));
    // printf("%d\n", look_up(root, 10));
    // printf("%d\n", look_up(root, 5));
    // printf("%d\n", look_up(root, 8));
    // printf("%d\n", look_up(root, 0));

    handler();

    return 0;
}