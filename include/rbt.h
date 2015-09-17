#ifndef RBT_H
#define RBT_H

enum rb_color
{
    RED = 0,
    BLK = 1
};

struct rb_node
{
    void* key;
    void* info;
    int color;

    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* p;
};

struct rb_tree
{
    /* return a positive value when a > b */
    /* 0 for a == b, negative value for a < b */
    int (*compare)(const void *a, const void *b);
    void (*destroy_key)(void *a);
    void (*destroy_info)(void *a);
    void (*print_key)(void *a);
    void (*print_info)(void *a);

    struct rb_node* root;
    struct rb_node* nil;
};

#define RB_TREE_INIT {0,0}

struct rb_tree* create_rb_tree(int (*compFunc)(const void*, const void*),
                               void (*destFunc)(void*),
                               void (*destInfoFunc)(void*),
                               void (*printFunc)(void*),
                               void (*printInfoFunc)(void*));
struct rb_node* rb_insert(struct rb_tree* T, void* key);
void rb_delete(struct rb_tree* T, struct rb_node* z);

struct rb_node* rb_get_maximum(struct rb_tree* T, struct rb_node* x);
struct rb_node* rb_get_predecessor(struct rb_tree* T, struct rb_node* x);
struct rb_node* rb_get_minimum(struct rb_tree* T, struct rb_node* x);
struct rb_node* rb_get_successor(struct rb_tree* T, struct rb_node* x);

struct rb_node* rb_search(const struct rb_tree* T, const void* key);
void rb_in_order_print_tree(struct rb_tree* T, struct rb_node* x);
void rb_free_tree(struct rb_tree* T);
void rb_free_subtree(struct rb_tree* T, struct rb_node* x);

extern struct rb_node nil;
#endif
