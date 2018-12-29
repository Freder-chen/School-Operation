#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Block {
	int pid;
};

struct RAM {
	int cursor;
	int mcount;
	int bnum;
	struct Block *blist;
};

struct RAM *new_RAM(int bnum) {
	struct RAM *ram = (struct RAM *)malloc(sizeof(struct RAM));
	ram->cursor = 0;
	ram->mcount = 0;
	ram->bnum = bnum;
	ram->blist = (struct Block *)malloc(sizeof(struct Block) * bnum);
	memset(ram->blist, 0, sizeof(struct Block) * bnum);
	return ram;
}

void free_RAM(struct RAM *ram) {
	if (ram) {
		if (ram->blist) {
			free(ram->blist);
		}
		free(ram);
	}
}

int pid_in_ram(struct RAM *ram, int pid) {
	int i;
	for (i = 0; i < ram->bnum; i++) {
		if ((ram->blist)[i].pid == pid) {
			return 1;
		}
	}
	return 0;
}

void add_block(struct RAM *ram, int pid) {
	if (!pid_in_ram(ram, pid)) {
		(ram->blist)[ram->cursor].pid = pid;
		ram->cursor = (ram->cursor + 1) % ram->bnum;
		ram->mcount++;
	}
}

void _print_RAM(struct RAM *ram) {
	int i;

	printf("%s ", "block list:");
	for (i = 0; i < ram->bnum - 1; i++) {
		printf("%d\t", (ram->blist)[i].pid);
	} printf("%d\n", (ram->blist)[i].pid);
}

void FIFO(int bnum, int *pid_list, int pid_num) {
	int i;

	struct RAM *ram = new_RAM(bnum);
	for (i = 0; i < pid_num; i++) {
		printf("%d block coming, pid(%d)...\n", i + 1, pid_list[i]);
		add_block(ram, pid_list[i]);	
		_print_RAM(ram);
		printf("missing page count: %d\n", ram->mcount);
		printf("missing page rate: %f\n\n", (float)ram->mcount / (i + 1));
	}

	free_RAM(ram);
}

int main() {
	int bnum = 2;
	int pid_list[] = {1, 3, 4, 3, 1};
	int pid_num = sizeof(pid_list) / sizeof(int);

	FIFO(bnum, pid_list, pid_num);

	return 0;
}