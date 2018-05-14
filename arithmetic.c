#include <stdio.h>
#include <string.h>
#include "queue\link_queue.h"
#include "stack\sequence_stack.h"

/* Low level handing */

int is_space(char ch)
{
	return (ch == ' ' || ch == '\t') ? 1 : 0;
}

int is_digit(char ch)
{
	return (ch >= '0' && ch <= '9') ? 1 : 0;
}

int is_operator(char ch)
{
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/') ? 1 : 0;
}

int is_high_operator(char ch)
{
	return (ch == '*' || ch == '/') ? 1 : 0;
}

enum Status find_next_number(char *str, int *index, int *num)
{
	struct LinkedQueue *digit_queue = new_queue();
	
	// Get digit array.
	int i = *index;
	while (str[i] != '\0')
	{
		if (is_space(str[i]))
		{
			i++;
			continue;
		}
		if (!is_digit(str[i])) break;
		push_queue_elem(digit_queue, str[i++]);
	}

	// Get number value.
	int number = 0;
	if (is_empty_queue(digit_queue))
	{
		return Error;
	}

	while (!is_empty_queue(digit_queue))
	{
		number = number * 10 + pop_queue_elem(digit_queue) - '0';
	}

	// Get ruselt.
	*index = i - 1;
	*num = number;
	free_queue(digit_queue);
	return Succeed;
}

/* arithmetic handing */

double arithmetic(char sign, int num1, int num2)
{
	switch (sign)
	{
		case '+': return (double) num1 + num2;
		case '-': return (double) num1 - num2;
		case '*': return (double) num1 * num2;
		case '/': return (double) num1 / num2;
		default:
			printf("The sign is %c(%d).\n", sign, sign);
			return 0;
	}
}

int parse(char *str)
{
	int i, num = 0;
	struct SequenceStack *operator_stack = new_stack();
	struct SequenceStack *number_stack = new_stack();

	for (i = 0; str[i] != '\0'; i++)
	{
		if (is_digit(str[i]))
		{
			find_next_number(str, &i, &num); // Error not deal.
			push_elem(number_stack, num);
		}
		else
		{
			if (is_operator(str[i]))
			{
				if (is_high_operator(str[i]))
				{
					int next_num;
					char operator = str[i];
					num = pop_elem(number_stack);
					i++;
					if (find_next_number(str, &i, &next_num) == Error)
					{
						printf("The expression has error.\n");
						return 0;
					}
					push_elem(number_stack, arithmetic(operator, num, next_num));
				}
				else // low operator
				{
					push_elem(operator_stack, str[i]);
				}
			}
		}
	}

	while (stacklen(number_stack) > 1)
	{
		push_elem(number_stack, arithmetic(pop_elem(operator_stack), pop_elem(number_stack), pop_elem(number_stack)));
	}
	int result = pop_elem(number_stack);

	free_stack(operator_stack);
	free_stack(number_stack);
	return result;
}

int main()
{
	char str[200] = "2 + 1 * 3";
	// scanf("%[^\n]", str);

	int result = parse(str);
	printf("%d\n", result);

	system("pause");
	return 0;
}