#include "huffman.h"
#include "../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

/* Low level handling */

int _compare_node(const void *n1, const void *n2)
{
	return (*(struct HuffmanNode **)n2)->weight - (*(struct HuffmanNode **)n1)->weight;
}

struct HuffmanNode *_create_a_huffman_node(char ch, int weight, struct HuffmanNode *lchild, struct HuffmanNode *rchild)
{
	struct HuffmanNode *node = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
	node->ch = ch;
	node->weight = weight;
	node->lchild = lchild;
	node->rchild = rchild;
	return node;
}

void _free_node(struct HuffmanNode *node)
{
	free(node);
}

void _free_tree_node(struct HuffmanNode *root)
{
	if (root->lchild) _free_tree_node(root->lchild);
	if (root->rchild) _free_tree_node(root->rchild);
	free(root);
}

void _get_huffman_code(struct HuffmanNode *node, char *code, struct HuffmanCode *ncode, int *cl)
{
	if (!node) return;

	if (node->lchild == NULL && node->rchild == NULL)
	{
		ncode[*cl].ch = node->ch;
		ncode[*cl].code = code;
		(*cl)++;
		// printf("%c %d %s\n", node->ch, node->weight, code);
		return;
	}
	
	if (node->lchild != NULL)
	{
		char *c = (char *)malloc(sizeof(char) * strlen(code) + 1);
		strcpy(c, code);
		_get_huffman_code(node->lchild, strcat(c, "0"), ncode, cl);
	}
	if (node->rchild != NULL)
	{
		char *c = (char *)malloc(sizeof(char) * strlen(code) + 1);
		strcpy(c, code);
		_get_huffman_code(node->rchild, strcat(c, "1"), ncode, cl);	
	}
	
	if (code != "") free(code);
}

void _add_code(char **dest, char *src)
{
	int src_len = strlen(src);
	int dest_len = strlen(*dest);
	int dest_size = _msize(*dest);

	if (dest_size < dest_len + src_len)
	{
		char *s = (char *)realloc(*dest, dest_len + src_len + 1);
		if (s == NULL)
		{
			printf("Add Code Error: Realloc string failed.\n");
			exit(0);
		}
		*dest = s;
	}

	strcat(*dest, src);
}

void _insert_char(char **dest, char ch)
{
	int size = _msize(*dest);
	int len = strlen(*dest);

	if (len + 1 >= size)
	{
		char *s = (char *)realloc(*dest, len + 1);
		if (s == NULL)
		{
			printf("Add Char Error: Realloc string failed.\n");
			exit(0);
		}
		*dest = s;
	}

	(*dest)[len] = ch;
	(*dest)[len + 1] = '\0';
}

char *_read_file(char *name)
{
	char *str = (char *)malloc(sizeof(char));
	str[0] = '\0';

	char buff[255];
	FILE *fp = fopen(name, "r");
	while (fgets(buff, 255, (FILE*)fp) != NULL)
	{
		_add_code(&str, buff);
	}
	fclose(fp);

	return str;
}

/* Huffman handling */

struct Huffman *init_huffman()
{
	int i, j;

	// Input Data
	FILE *fp = fopen("text\\input", "r");
	
	int len;
	fscanf(fp, "%d%*[\n]", &len);

	struct HuffmanNode **nlist = (struct HuffmanNode **)malloc(sizeof(struct HuffmanNode *) * len);
	for (i = 0; i < len; i++)
	{
		char ch;
		int weight;
		fscanf(fp, "%c%*[ ]%d%*[\n]", &ch, &weight);
		nlist[i] = _create_a_huffman_node(ch, weight, NULL, NULL);
	}

	fclose(fp);

	// Create Huffman Tree
	for (i = len; i > 1; i--)
	{
		qsort(nlist, i, sizeof(struct HuffmanNode *), _compare_node);
		nlist[i - 2] = _create_a_huffman_node('\0', nlist[i - 1]->weight + nlist[i - 2]->weight, nlist[i - 1], nlist[i - 2]);
	}
	struct HuffmanNode *root = nlist[0];
	free(nlist);

	// Get Huffman Code.
	struct HuffmanCode *ncode = (struct HuffmanCode *)malloc(sizeof(struct HuffmanCode) * len);
	int cl = 0;
	_get_huffman_code(root, "", ncode, &cl);

	// Save Huffman Code
	fp = fopen("text\\huffman", "w");
	fprintf(fp, "%d\n", len);
	for (i = 0; i < len; i++)
	{
		fprintf(fp, "%c %s\n", ncode[i].ch, ncode[i].code);
	}
	fclose(fp);

	// Return Result
	struct Huffman *huffman = (struct Huffman *)malloc(sizeof(struct Huffman));

	huffman->tree = (struct HuffmanTree *)malloc(sizeof(struct HuffmanTree));
	huffman->tree->root = root;

	huffman->code_arr = (struct HuffmanCodeArr *)malloc(sizeof(struct HuffmanCodeArr));
	huffman->code_arr->arr = ncode;
	huffman->code_arr->len = len;

	return huffman;
}

void free_huffman(struct Huffman *huffman)
{
	// Free Tree
	_free_tree_node(huffman->tree->root);
	free(huffman->tree);
	// Free Code Array
	free(huffman->code_arr->arr);
	free(huffman->code_arr);
}

void coding(struct HuffmanCodeArr *code_arr)
{
	int i;

	// Read Trans File
	char *str = _read_file("text\\tobetrans.dat");

	// Read Huffman Code
	int len = code_arr->len;
	struct HuffmanCode *ncode = code_arr->arr;

	// Coding String
	char *code = (char *)malloc(sizeof(char));
	code[0] = '\0';
	int sp; // str position
	for (sp = 0; str[sp] != '\0'; sp++)
	{
		for (i = 0; i < len; i++)
		{
			if (ncode[i].ch == str[sp])
			{
				// printf("code: %s\n", code);
				_add_code(&code, ncode[i].code);
				break;
			}
		}
	}

	// Save Code File
	FILE *fp = fopen("text\\codefile", "w");
	fprintf(fp, "%s", code);
	fclose(fp);

	free(str);
	free(code);
}

void decoding(struct HuffmanTree *tree)
{
	int i;

	// Read Code File
	char *code = _read_file("text\\codefile");

	// Parse Code
	char *text = (char *)malloc(sizeof(char) * 8);
	memset(text, 0, sizeof(text));

	struct HuffmanNode *node = tree->root;
	for (i = 0; i < strlen(code); i++)
	{
		node = code[i] == '0' ? node->lchild : node->rchild;

		if (node->lchild == NULL && node->rchild == NULL)
		{
			_insert_char(&text, node->ch);
			node = tree->root;
		}
	}

	// Save Text File
	FILE *fp = fopen("text\\textfile", "w");
	fprintf(fp, "%s", text);
	fclose(fp);

	// printf("code: %s\n", code);
	// printf("text: %s\n", text);

	free(code);
	free(text);
}

void print_code()
{
	int i;

	char *code = _read_file("text\\codefile");

	for (i = 0; i < strlen(code); i++)
	{
		printf(((i + 1) % 50 == 0 ? "%c\n" : "%c"), code[i]);
	}

	free(code);
}

// Print Tree
int _height(struct HuffmanNode *node)
{
	if (node == NULL) return 0;
	int l = _height(node->lchild);
	int r = _height(node->rchild);
	return (l >= r ? l : r) + 1;
}

void _get_nodes(struct HuffmanNode *node, int index, char *text)
{
	if (node != NULL)
	{
		text[index - 1] = node->ch == '\0' ? '_' : node->ch;
		_get_nodes(node->lchild, 2 * index, text);
		_get_nodes(node->rchild, 2 * index + 1, text);
	}
}

void print_tree(struct HuffmanTree *tree)
{
	int depth = _height(tree->root);

	char *nodes = (char *)malloc(sizeof(char) * (1 << depth));
	memset(nodes, '\0', sizeof(char) * (1 << depth - 1));

	_get_nodes(tree->root, 1, nodes);
	draw_tree(nodes, depth);

	free(nodes);
}
