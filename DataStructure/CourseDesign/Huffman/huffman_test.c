#include "huffman.h"

int main()
{
	struct Huffman *huffman = init_huffman();

	coding(huffman->code_arr);
	decoding(huffman->tree);

	print_code();
	print_tree(huffman->tree);

	free_huffman(huffman);

	return 0;
}