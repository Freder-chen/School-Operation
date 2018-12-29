#include <stdio.h>
#include <stdlib.h>

/* base struct */
struct process {
	int btr;
	int ltr;
	int size;
	int pid; // if not process, pid = 0
	struct process *next;
};

struct form {
	int size;
	struct process *process;
};

/* base func */
struct process *_create_process(int btr, int ltr, int size, int pid, struct process *next) {
	struct process *pro = (struct process *)malloc(sizeof(struct process));
	pro->btr = btr;
	pro->ltr = ltr;
	pro->size = size;
	pro->pid = pid;
	pro->next = next;
	return pro;
}

struct process *create_process_in_form(struct form *form, int btr, int ltr, int size, int pid) {
	if (size > form->size) return NULL;

	struct process *pro;

	// load the begin status
	for (pro = form->process; pro; pro = pro->next) {
		if (btr >= pro->btr + pro->ltr) {
			pro->pid = 0;
			pro->btr = 0;
			pro->ltr = 0;
		}
	}
	for (pro = form->process; pro && pro->next; pro = pro->next) {
		// printf("pid: %d, npid: %d\n", pro->pid, pro->next->pid);
		if (pro->pid == 0 && pro->next->pid == 0) {
			struct process *pn = pro->next;
			pro->size = pro->size + pn->size;
			pro->next = pn->next;
			free(pn);
		}
	}

	// create new process
	for (pro = form->process; pro; pro = pro->next) {
		// printf("pid: %d\n", pro->pid);
		if (!pro->pid && pro->size > size) {
			pro->next = _create_process(0, 0, pro->size - size, 0, pro->next);
			pro->btr = btr;
			pro->ltr = ltr;
			pro->size = size;
			pro->pid = pid;
			break;
		}
	}
	if (!pro) {
		pro = create_process_in_form(form, btr + 1, ltr, size, pid);
	}	

	return pro;
}

struct form *create_form(int size) {
	struct form *form = (struct form *)malloc(sizeof(struct form));
	form->size = size;
	form->process = _create_process(0, 0, size, 0, NULL);
	return form;
}

void free_form(struct form *form) {
	if (form == NULL) return;
	
	struct process *pro = form->process;
	free(form);

	while (pro) {
		struct process *p = pro;
		pro = pro->next;
		free(p);
	}
}

void print_form(struct form *form) {
	struct process *pro;

	printf("%6s\t%6s\t%6s\t%6s\n", "begin", "end", "size", "pid");
	for (pro = form->process; pro; pro = pro->next) {
		printf("%6d\t%6d\t%6d\t%6d\n", pro->btr, pro->btr + pro->ltr, pro->size, pro->pid);
	}
}

int main()
{
	int i, size = 256;
	struct form *form = create_form(size);

	int btr[] = {0, 2, 6};
	int ltr[] = {5, 4, 9};
	int psize[] = {50, 30, 100};

	for (i = 0; i < sizeof(btr) / sizeof(int); i++) {
		printf("--- current time: %d\n", btr[i]);
		create_process_in_form(form, btr[i], ltr[i], psize[i], i + 1);
		print_form(form);
		printf("------------------------\n\n");
	}

	free_form(form);
	return 0;
}
