#include "huffman.h"
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

int main()
{
	// Change Work Directory
    char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _splitpath(_pgmptr, drive, dir, fname, ext);
    chdir(dir);

    // Huffman Work
	struct Huffman *huffman = init_huffman();
	coding(huffman->code_arr);
	decoding(huffman->tree);
	print_code();

	printf("\nPrint Tree:\n");
	print_tree(huffman->tree);

	free_huffman(huffman);
	return 0;
}