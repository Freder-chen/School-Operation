#include "common.h"
#include <stdio.h>

void _putchars(char c, int n)
{
	while (n--) putchar(c);
}

int _print_node(char c, int w)
{
	return printf("%*c%c%c", w - 1, '(', c, ')');
}

void _print_left_to_parent_branch_top(int w)
{
	_putchars(' ', w + 1);
	_putchars('_', w - 3);
	printf("/ ");
}

void _print_right_to_parent_branch_top(int w)
{
	putchar('\\');
	_putchars('_', w - 3);
	_putchars(' ', w + 2);
}

void _print_left_to_parent_branch_bottom(int w)
{
	printf("%*c%c%*c", w, '_', '/', w - 1, ' ');
}

void _print_right_to_parent_branch_bottom(int w)
{
	printf("%*c%c%*c", w - 1, '\\', '_', w, ' ');
}

void draw_tree(char *nodes, int depth)
{
	int i, j, k, index = 0;
	for (j = 0; j < depth; j++)
	{
		int w = 1 << (depth - j + 1);
		// Branch
		if (j == 0)
		{
			printf("%*c\n", w, '_');
		}
		else
		{
			for (i = 0; i < 1 << j; i++)
				if (nodes[index + i])
					if (i % 2 == 0) _print_left_to_parent_branch_top(w);
					else 			_print_right_to_parent_branch_top(w);
				else
					_putchars(' ', w * 2);
			putchar('\n');

			for (i = 0; i < 1 << j; i++)
				if (nodes[index + i])
					if (i % 2 == 0) _print_left_to_parent_branch_bottom(w);
					else            _print_right_to_parent_branch_bottom(w);
				else
					_putchars(' ', w * 2);
			putchar('\n');
		}
		// Node Content
		for (i = 0; i < 1 << j; i++, index++)
			if (nodes[index])
				_putchars(' ', w * 2 - _print_node(nodes[index], w));
			else
				_putchars(' ', w * 2);
		putchar('\n');
	}
}