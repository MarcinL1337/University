#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

void print_output(char *greeting, char *name, char *color, int capitalize) {
	if (capitalize)
		name[0] = toupper(name[0]);

	if (!strcmp(color, "always") || (!strcmp(color, "auto") && isatty(1)))
		printf("\033[1;32m");

	printf("%s %s\033[0m\n", greeting, name);
}

int main(int argc, char *argv[]) {
	static struct option long_options[] = {
		{ "capitalize", no_argument, 		NULL, 'c' },
		{ "greeting", 	required_argument,	NULL, 'g' },
		{ "color", 	required_argument, 	NULL, 'x' },
		{ "world", 	no_argument, 		NULL, 'w' },
		{ "version", 	no_argument, 		NULL, 'v' },
		{ "help", 	no_argument, 		NULL, 'h' },
	};

	static int capitalize = false;
	static char *greeting = "Hello,";
	static char *color = "never";
	static bool helloworld = false;

	while (true) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "cg:wvh", long_options, &option_index);

		if (c == -1) // end of options
			break;

		switch (c) {
			case 'c': capitalize = true; break;
			case 'g': greeting = optarg; break;
			case 'x': color = optarg; break;
			case 'w': helloworld = true; break;
			case 'v': printf("hwb version 0.01\n"); break;
			case 'h': 
				printf("Usage: %s <options> <arguments>\n", argv[0]); 
				printf("Options: \n");
				printf("	-c, --capitalize            = capitalize first letter of string\n");
				printf("	-g, --greeting              = substitute 'Hello' for given text\n");
				printf("	--color[=auto|always|never] = colorize the output\n");
				printf("	-w, --world                 = print extra 'Hello, world!'\n");
				printf("	-v, --verison               = print version\n");
				printf("	-h, --help                  = print help\n");
				break;
			default:  printf("%s", optarg);
		}
	}

	while (optind < argc) 
		print_output(greeting, argv[optind++], color, capitalize);

	if (helloworld)
		print_output(greeting, "world", color, capitalize);

	return 0;
}
