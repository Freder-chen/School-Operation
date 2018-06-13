#ifndef __HUFFMAN__
#define __HUFFMAN__

/* Huffman Tree */

struct HuffmanNode {
	char ch;
	int weight;
	struct HuffmanNode *lchild;
	struct HuffmanNode *rchild;
};

struct HuffmanTree {
	struct HuffmanNode *root;
};

/* Huffman Code */

struct HuffmanCode {
	char ch;
	char *code;
};

struct HuffmanCodeArr {
	struct HuffmanCode *arr;
	int len;
};

/* Huffman Struct */

struct Huffman {
	struct HuffmanTree *tree;
	struct HuffmanCodeArr *code_arr;
};

struct Huffman *init_huffman();

void free_huffman(struct Huffman *huffman);

void coding(struct HuffmanCodeArr *code_arr);

void decoding(struct HuffmanTree *tree);

void print_code();

void print_tree(struct HuffmanTree *tree);

#endif