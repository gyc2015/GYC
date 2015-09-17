#include "sym.h"
#include <stdio.h>
#include <string.h>

char* normal_pre_words[] =
{
    "__attribute__", "__apicdebuginit",

    0
};

char* specialwords[] =
{
    "__releases","__acquires","__attribute__","__dm_written_to_disk",
    "__must_hold","__attribute","__printf",
    
    0
};

char* syswords[] = 
{
    "SYSCALL_DEFINE0","SYSCALL_DEFINE1","SYSCALL_DEFINE2",
    "SYSCALL_DEFINE3","SYSCALL_DEFINE4","SYSCALL_DEFINE5",
    "SYSCALL_DEFINE6",

    0
};

int lookup(char* name, char** list)
{
	int cmp;
	while (*list != 0) {
        cmp = strcmp(name, *list);
		if (cmp == 0)
			return 1;
		list++;
	}
	return 0;
}

int is_special_words(char* name)
{
	return lookup(name, specialwords);
}

int is_normal_pre_words(char* name)
{
	return lookup(name, normal_pre_words);
}

int is_sys_words(char* name)
{
	return lookup(name, syswords);
}
