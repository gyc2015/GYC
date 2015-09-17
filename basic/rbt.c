/*
 * red black tree:
 * 1. Every node is either red or black
 * 2. The root is black
 * 3. Every leaf (NIL) is black
 * 4. If a node is red, then both its children are black
 * 5. For each node, all simple paths from the node to descendant leaves
 *    contain the same number of black nodes.
 */

#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>

struct rb_node nil = { 0, 0, BLK, &(nil), &(nil), &(nil) };

struct rb_tree* create_rb_tree(int (*compFunc)(const void*, const void*),
                               void (*destFunc)(void*),
                               void (*destInfoFunc)(void*),
                               void (*printFunc)(void*),
                               void (*printInfoFunc)(void*))
{
    struct rb_tree* t = malloc(sizeof(struct rb_tree));
    t->compare = compFunc;
    t->destroy_key = destFunc;
    t->destroy_info = destInfoFunc;
    t->print_key = printFunc;
    t->print_info = printInfoFunc;

    t->root = t->nil = &nil; 

    return t;
}

/*
 * left_rotate - It makes right child y of x the new root of the subtree,
 * with x as y's left child, and y's left child as x's right child.
 * @T: the original rb_tree pointer
 * @x: may be any node of the tree whose right child is not NULL
 */
void left_rotate(struct rb_tree *T, struct rb_node *x)
{
    /* Set y */
    struct rb_node *y;
    y = x->right;
    /* turn y's left subtree into x's right subtree */
    x->right = y->left;
    if (y->left != T->nil)
        y->left->p = x;
    /* link x's parent to y */
    y->p = x->p;
    if (x->p == T->nil)
        T->root = y;
    else if (x->p->left == x)
        x->p->left = y;
    else x->p->right = y;
    /* put x on y's left*/
    y->left = x;
    x->p = y;
}

/*
 * right_rotate - It makes right child y of x the new root of the subtree,
 * with x as y's right child, and y's right child as x's left child.
 * @T: the original rb_tree pointer
 * @x: may be any node of the tree whose left child is not NULL
 */
void right_rotate(struct rb_tree *T, struct rb_node *x)
{
    /* Set y */
    struct rb_node *y;
    y = x->left;
    /* turn y's right subtree into x's left subtree */
    x->left = y->right;
    if (y->right != T->nil)
        y->right->p = x;
    /* link x's parent to y */
    y->p = x->p;
    if (x->p == T->nil)
        T->root = y;
    else if (x->p->left == x)
        x->p->left = y;
    else x->p->right = y;
    /* put x on y's right*/
    y->right = x;
    x->p = y;
}

/*
 * rb_insert_fixup - fixup the 5 properties of a RBT
 *
 * After a simple insert action, the property 2 and property 4
 * may be violated.
 */
void rb_insert_fixup(struct rb_tree* T, struct rb_node* z)
{
    struct rb_node* y;

    while (RED == z->p->color) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == RED) {
                z->p->color = BLK;
                y->color = BLK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(T, z);
                }
                z->p->color = BLK;
                z->p->p->color = RED;
                right_rotate(T,z->p->p);
            }
        } else {
            y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLK;
                y->color = BLK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(T,z);
                }
                z->p->color = BLK;
                z->p->p->color = RED;
                left_rotate(T,z->p->p);
            }
        }
    }
    
    T->root->color = BLK;
}

struct rb_node* rb_insert(struct rb_tree* T, void* key)
{
    struct rb_node *y = T->nil;
    struct rb_node *x = T->root;
    struct rb_node *z = malloc(sizeof(struct rb_node));
    z->key = key;

    while (T->nil != x) {
        y = x;
        if (1 == T->compare(x->key,z->key))
            x = x->left;
        else x = x->right;
    }

    z->p = y;
    if (T->nil == y)
        T->root = z;
    else if (1 == T->compare(y->key,z->key))
        y->left = z;
    else y->right = z;

    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;

    rb_insert_fixup(T, z);
    return z;
}

struct rb_node* rb_get_minimum(struct rb_tree* T, struct rb_node* x)
{
    while (T->nil != x->left)
        x = x->left;
    return x;
}

/*
 * get_successor - return the successor of given RBT and node.
 *
 * the successor of x is the minimum of the right subtree of x,
 * or the root of the minimal subtree that contains x in its left subtree.
 */
struct rb_node* rb_get_successor(struct rb_tree* T, struct rb_node* x)
{
    struct rb_node *y;

    if (T->nil != x->right) {
        return rb_get_minimum(T,x->right);
    } else {
        y = x->p;
        while (x == y->right) {
            if (y == T->root)
                return T->nil;
            x = y;
            y = y->p;
        }
        return y;
    }

}

struct rb_node* rb_get_maximum(struct rb_tree* T, struct rb_node* x)
{
    while (T->nil != x->right)
        x = x->right;
    return x;
}

/*
 * get_predecessor - return the predecessor of given RBT and node.
 *
 * the predecessor of x is the maximum of the right subtree of x,
 * or the root of the minimal subtree that contains x in its right subtree.
 */
struct rb_node* rb_get_predecessor(struct rb_tree* T, struct rb_node* x)
{
    struct rb_node *y;

    if (T->nil != x->left) {
        return rb_get_maximum(T,x->left);
    } else {
        y = x->p;
        while (x == y->left) {
            if (y == T->root)
                return T->nil;
            x = y;
            y = y->p;
        }
        return y;
    }
}

/*
 * rb_transplant - replace u by v in the same tree T
 */
void rb_transplant(struct rb_tree* T, struct rb_node* u, struct rb_node* v)
{
    if (T->nil == u->p)
        T->root = v;
    else if (u->p->left == u)
        u->p->left = v;
    else u->p->right = v;

    v->p = u->p;
}

void rb_delete_fixup(struct rb_tree* T, struct rb_node* x)
{
    struct rb_node* w;
    while (T->root != x && x->color == BLK) {
        if (x == x->p->left) {
            w = x->p->right;
            if (RED == w->color) {
                w->color = BLK;
                x->p->color = RED;
                left_rotate(T,x->p);
                w = x->p->right;
            }
            if (BLK == w->left->color && BLK == w->left->color) {
                w->color = RED;
                x = x->p;
            } else {
                if (BLK == w->right->color) {
                    w->left->color = BLK;
                    w->color = RED;
                    right_rotate(T,w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLK;
                w->right->color = BLK;
                left_rotate(T,x->p);
                x = T->root;
            }
        } else {
            w = x->p->left;
            if (RED == w->color) {
                w->color = BLK;
                x->p->color = RED;
                right_rotate(T,x->p);
                w = x->p->left;
            }
            if (BLK == w->right->color && BLK == w->left->color) {
                w->color = RED;
                x = x->p;
            } else {
                if (BLK == w->right->color) {
                    w->right->color = BLK;
                    w->color = RED;
                    left_rotate(T,w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLK;
                w->left->color = BLK;
                right_rotate(T,x->p);
                x = T->root;
            }
        }
    }
    x->color = BLK;
}

void rb_delete(struct rb_tree* T, struct rb_node* z)
{
    struct rb_node* y;
    struct rb_node* x;

    y = z;
    int yc = y->color;

    if (T->nil == z->left) {
        x = z->right;
        rb_transplant(T,z,z->right);
    } else if ( T->nil == z->right) {
        x = z->left;
        rb_transplant(T,z,z->left);
    } else {
        y = rb_get_minimum(T,z->right);
        yc = y->color;
        x = y->right;
        if (z == y->p) {
            x->p = y;
        } else {
            rb_transplant(T,y,y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rb_transplant(T,z,y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if (BLK == yc)
        rb_delete_fixup(T,x);

    T->destroy_key(z->key);
    T->destroy_info(z->info);
}

struct rb_node* rb_search(const struct rb_tree* T,const void* key)
{
    struct rb_node* x = T->root;
    int tmp = T->compare(x->key, key);
    while (0 != tmp) {
        if (1 == tmp)
            x = x->left;
        else x = x->right;

        if (T->nil == x)
            return T->nil;
        tmp = T->compare(x->key, key);
    }
    return x;
}

void rb_in_order_print_tree(struct rb_tree* T, struct rb_node* x)
{
    if (x != T->nil) {
        rb_in_order_print_tree(T, x->left);
      
        printf("key=");T->print_key(x->key);printf(",");
        (RED == x->color) ? printf("red") : printf("blk");
        printf("||");
        
        printf("l->key=");
        (T->nil == x->left) ? printf("NIL") : T->print_key(x->left->key);
        printf(";");
        
        printf("r->key=");
        (T->nil == x->right) ? printf("NIL") : T->print_key(x->right->key);
        printf(";");

        printf("p->key=");
        (T->nil == x->p) ? printf("NIL") : T->print_key(x->p->key);
        printf("\n");

        rb_in_order_print_tree(T, x->right);
    }
}

void rb_free_subtree(struct rb_tree* T, struct rb_node* x)
{
    if (T->nil != x) {
        rb_free_subtree(T,x->left);
        rb_free_subtree(T,x->right);
        T->destroy_key(x->key);
        T->destroy_info(x->info);
        free(x);
    }
}

void rb_free_tree(struct rb_tree* T)
{
    rb_free_subtree(T, T->root);
    free(T);
}
