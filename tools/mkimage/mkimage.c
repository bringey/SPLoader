/*
** File: mkimage.c
**
** mkimage <image files> -o <output_file>
**
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <SPLoader/i386/pc/bios/bootstrap.h>


#define SECTOR_SIZE 512



void usage(void) {
	fputs("usage: mkimage <image files...> -o <output_file>\n", stderr);
	exit(EXIT_FAILURE);
}



int main(int argc, char* argv[]) {

	char *outfile = NULL;
	char *bootstrap = NULL;
	char **loaderBins = NULL;
	int loaderBinCount = 0;

	int c;
	extern int optind, optopt;
	extern char *optarg;

	while ((c = getopt(argc, argv, ":b:o:")) != EOF) {

		switch (c) {
			case ':':
				fprintf(stderr, "missing operand after -%c\n", optopt);
			case '?':
				usage();
				break;
			case 'b':
				bootstrap = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			default:
				usage();
				break;

		}

	}

	if (bootstrap == NULL) {
		fputs("no bootstrap binary specified\n", stderr);
		usage();
	}

	if (outfile == NULL) {
		fputs("no output file specified\n", stderr);
		usage();
	}

	loaderBins = argv + optind;
	loaderBinCount = argc - optind;



	return 0;
}