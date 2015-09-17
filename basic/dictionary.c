#include "dictionary.h"
#include "mm.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

char ALPHABETA[256];

char* dic_get_name(struct dictionary *dict, int i, vector_char *str)
{
    struct rb_node *e = dict_ele(dict, i); 
	dic_traceback_string(e, str);
	return VECTOR(*str);
}

char* dic_traceback_string(struct rb_node *e, vector_char *str)
{
	assert(0 != str);
	assert(0 != str->stor_begin);

    int len = DIC_BITINFO(e)->depth;
	vector_char_adjust(str, len+1);

    VECTOR(*str)[len + 1] = '\0';
    struct bitinfo *tmp = (struct bitinfo*)(e->info);
    for (; 0 <= len; len--) {
        VECTOR(*str)[len] = *(char*)(e->key);
        e = tmp->p;
        tmp = (struct bitinfo*)(e->info);
    }
	return VECTOR(*str);
}

/*
 * dic_setup - initialize the alphabeta list and the nil object
 */
void dic_setup()
{
    for(int i = 0; i < 256; i++)
        ALPHABETA[i] = i;
}

struct dictionary* new_dictionary(int n)
{
    struct dictionary *result = malloc(sizeof(struct dictionary));
    result->root = create_rb_tree(dic_compare, dic_destroy_key, dic_destroy_info, dic_print_key, dic_print_info);
	vector_ptr_alloc(&(result->data), n);
	return result;
}

/*
 * create_bitinfo - the info of each node except the terminal node
 * @depth - the depth of current node
 * @p - point to the owner node of current node
 */
struct bitinfo* create_bitinfo(int depth, struct rb_node* p)
{
    struct bitinfo* dic = malloc(sizeof(struct bitinfo));
    dic->depth = depth;
    dic->ID = -1;
    dic->p = p;
    dic->rbt = create_rb_tree(dic_compare, dic_destroy_key, dic_destroy_info, dic_print_key, dic_print_info);
    return dic;
}

/*
 * dic_search_term - search the dictionary @T for the string @a
 */
void* dic_search_term(char *a, struct rb_tree *T)
{
    struct rb_node* tnd;

    while ('\0' != a[0]) {
       tnd = rb_search(T, a);
       if (T->nil == tnd)
           return 0;
       a++;
       T = ((struct bitinfo*)(tnd->info))->rbt;
    }
    tnd = rb_search(T,a);
    return ((T->nil == tnd)? 0 : (tnd->info));
}

int dic_compare(const void* a, const void* b)
{
    if (0 == a) {
        if (0 == b)
            return 0;
        else return -1;
    } else if (0 == b) {
        return 1;
    }

    if(*((char*)a) > *((char*)b))
        return 1;
    else if(*((char*)a) < *((char*)b))
        return -1;
    else return 0;
}

void dic_destroy_key(void* a)
{
//    free(a);
}

void dic_destroy_info(void* a)
{
    if(0 != a) {
        rb_free_tree(((struct bitinfo*)a)->rbt);
        free(a);
    }
}

void dic_print_key(void* a)
{

}

void dic_print_info(void* a)
{

}

struct rb_node* dict_ele(struct dictionary *dict, int i)
{
    int s = dict_size(dict);
    assert(i < s);
    return VECTOR(dict->data)[i];
}

int dict_size(const struct dictionary *dict)
{
    return vector_ptr_size(&(dict->data));
}

int dict_capacity(const struct dictionary *dict)
{
	return vector_ptr_capacity(&(dict->data));
}

struct rb_node* dict_add_str(struct dictionary *dict, const char *str)
{
    int depth = 0;
    struct rb_node* tmp = dic_insert(dict->root, dict->root->nil, str, &depth);

    if (-2 == DIC_BITINFO(tmp)->ID) {
        DIC_BITINFO(tmp)->ID = dict_size(dict);
		vector_ptr_push_back(&(dict->data),tmp);
        /* printf("<><><>  %d  <><><>\n", DIC_BITINFO(tmp)->ID); */
    }
    return tmp;
}

struct rb_node* dict_find_str(const struct dictionary *dict, const char *str)
{
    int depth = 0;
    return dic_search(dict->root, str, &depth);
}


/*
 * dic_insert - insert a string into the dictionary T
 * @T: the target dictionary
 * @p: point to the owner node 
 * @pdepth: return the length of the string a
 */
struct rb_node* dic_insert(struct rb_tree *T, struct rb_node *p, const char *a, int *pdepth)
{
    struct rb_node* tnd = rb_search(T,(void*)a);

    if (T->nil == tnd) {
        tnd = rb_insert(T,&ALPHABETA[a[0]]);
        tnd->info = create_bitinfo(*pdepth, p);
    }

    a++; (*pdepth)++;
    if ('\0' != a[0]) {
        return dic_insert(((struct bitinfo*)(tnd->info))->rbt,tnd,a, pdepth);
    } else {
        T = ((struct bitinfo*)(tnd->info))->rbt;
        p = tnd;
        tnd = rb_search(T,(void*)a);
        if (T->nil == tnd) {
            tnd = rb_insert(T, &ALPHABETA['\0']);
            tnd->info = create_bitinfo(*pdepth, p);
            DIC_BITINFO(tnd)->ID = -2;
        }
        return tnd;
    }
}

struct rb_node* dic_search(const struct rb_tree *T, const char *a, int *pdepth)
{
    struct rb_node* tnd = rb_search(T,a);
    if (T->nil == tnd) {
        return T->nil;
    }
    
    a++; (*pdepth)++;
    if ('\0' == a[0]) {
        tnd = rb_search(((struct bitinfo*)(tnd->info))->rbt, a);
        return tnd;
    } else {
        return dic_search(((struct bitinfo*)(tnd->info))->rbt, a, pdepth);
    }
}

void dic_delete(struct rb_tree *T, char *a, int len)
{

}

int dic_contains(struct rb_tree *T, char *a, int len)
{
    return 0;
}

