#include "debug.h"




void _print_bitmap(unsigned char *bitmap, int len) {
	int i;
	for (i = 0; i < len; i++) {
		printf("%d ", (int)bitmap[i]);
	}
	printf("\n");
}


void print_bitmap(struct superblock *sb, int group_index) {
	struct group_desc *gd = _get_group_desc(sb, group_index);
	printf("inode_bitmap: ");
	_print_bitmap(gd->inode_bitmap, sb->inodes_per_group);
	printf("block_bitmap: ");
	_print_bitmap(gd->block_bitmap, sb->blocks_per_group);
}