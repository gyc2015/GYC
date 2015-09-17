#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "rbt.h"
#include "vector_int.h"
#include "vector_ptr.h"

struct bitinfo
{
    int depth;
    int ID;
    struct rb_node* p;
    struct rb_tree* rbt;
};

#define DIC_BITINFO(a) ((struct bitinfo*)((a)->info))

void dic_setup();

struct bitinfo* create_bitinfo(int depth, struct rb_node* p);
void* dic_search_term(char *a, struct rb_tree *T);
int dic_compare(const void* a, const void* b);
void dic_destroy_key(void* a);
void dic_destroy_info(void* a);
void dic_print_key(void* a);
void dic_print_info(void* a);
/* dictionary operation */
struct dictionary
{
    struct rb_tree *root;
	vector_ptr data;
};

struct dictionary* new_dictionary(int n);
struct rb_node* dict_ele(struct dictionary *dict, int i);
int dict_size(const struct dictionary *dict);
int dict_capacity(const struct dictionary *dict);
struct rb_node* dict_add_str(struct dictionary *dict, const char *str);
struct rb_node* dict_find_str(const struct dictionary *dict, const char *str);

struct rb_node* dic_insert(struct rb_tree *T, struct rb_node *p, const char *a, int *pdepth);
struct rb_node* dic_search(const struct rb_tree *T, const char *a, int *pdepth);
int dic_contains(struct rb_tree *T, char *a, int len);
void dic_delete(struct rb_tree *T, char *a, int len);
char* dic_traceback_string(struct rb_node *e, vector_char *str);
char* dic_get_name(struct dictionary *dict, int i, vector_char *str);

/************************/

#endif
