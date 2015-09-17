#include "parser.h"
#include <lex.h>
#include <dictionary.h>

#include <dirent.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>

unsigned int flag = 0x0000;
/* --ct format output for network. */
#define CT_FLAG 0x0001
/* --dot format output for graphviz. */
#define DOT_FLAG 0x0002
/* --in the input file or directory. */
char *inPath = NULL;
/* --out the output file. */
char *outFileName = NULL;
/* --extention	the file's extention. */
char *extention = NULL;

void usage(int i)
{
    fprintf(stderr, "Usage:  gyc.exe (directory|file) [options]\n");
    fprintf(stderr, "Options:\n");
	fprintf(stderr, "	--out		the output file.\n");
	fprintf(stderr, "	--extention	the file's extention.\n");
	fprintf(stderr, "	--ct		format output for network.\n");
	fprintf(stderr, "	--dot		format output for graphviz.\n");
    fprintf(stderr, "	--help		display this help and exit.\n");
    fprintf(stderr, "===================================\n");
    fprintf(stderr, "end.\n");
    exit(i);
}

void setup_option(int argc, char* argv[])
{
    if (0 != argc) {
        char *tmp = argv[0];
        int len = strlen(tmp);
        int p = 1;
        
		if (0 == strcmp("--help", tmp))
			usage(0);
		else if (0 == strcmp("--ct", tmp))
			flag = CT_FLAG;
		else if (0 == strcmp("--dot", tmp))
			flag = DOT_FLAG;
		else if (0 == strcmp("--extention", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			else if (0 != strcmp(".ct", argv[0]) && 0 != strcmp(".c", argv[0]))
				usage(1);
			extention = argv[0];
		} else if (0 == strcmp("--out", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			outFileName = argv[0];
		} else
			usage(1);

		argc--; argv++;
        setup_option(argc, argv);
    }
}



int main(int argc, char *argv[])
{
    if(2 > argc) {
        printf("Please specify the file's name or directory's name\n");
		usage(1);
    }

	flag = CT_FLAG;
	extention = ".c";
	setup_option(argc-2, argv+2);

    SetupLexer();
    dic_setup();
    struct dictionary *func_dict = new_dictionary(4);
	struct dictionary *file_dict = new_dictionary(4);
	vector_ffpair fflist;
	vector_ffpair_init(&fflist, 8);

	struct stat info;
	stat(argv[1], &info);
	if (S_ISDIR(info.st_mode)) {
		char *path_name = argv[1];
		parse_dir(path_name, func_dict, file_dict, &fflist, extention);
	} else if (S_ISREG(info.st_mode)) {
		char *file_name = argv[1];
		dict_add_str(file_dict, file_name);
		parse_file(file_name, func_dict, file_dict, &fflist);
	} else {
        fprintf(stderr, "Please specify the file's name or directory's name\n");
		usage(1);
	}

	FILE *out_file = (NULL != outFileName) ? fopen(outFileName,"w") : stdout;

	if (flag & CT_FLAG)
		print_funcs_ct(func_dict, out_file);

	if (flag & DOT_FLAG)
		print_funcs_dot(func_dict, out_file);

	if (NULL != outFileName)
		fclose(out_file);

	vector_ffpair_destroy(&fflist);
	return 0;
}


