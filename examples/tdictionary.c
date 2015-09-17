#include "dictionary.h"
#include "vector_int.h"
#include <stdio.h>
#include <string.h>

int main()
{
    dic_setup();
	vector_char str;
	vector_char_init(&str,1);
	VECTOR(str)[0] = '\0';
    struct dictionary *dict = new_dictionary(10);
    struct rb_node* a = dict_add_str(dict, "hehehe");
    struct rb_node* b = dict_add_str(dict, "lalalala");
    struct rb_node* c = dict_add_str(dict, "fdafdsa");
    struct rb_node* d = dict_add_str(dict, "fdafdsa");
    printf("<< %s %d >>\n", dic_traceback_string(a,&str), strlen(VECTOR(str)));
    printf("<< %d >>\n", dict_size(dict));
    int n = dict_size(dict);
    for (int i = 0; i < n; i++) {
        printf("<< %d:%s >>\n", i,dic_traceback_string(dict_ele(dict,i),&str));
    }
    printf("hehe\n");
    struct rb_node* e = dict_find_str(dict, "hehehe");
    if (dict->root->nil != e)
        printf("<< %s %d >>\n", dic_traceback_string(e,&str), strlen(VECTOR(str)));
        
    return 0;
}
