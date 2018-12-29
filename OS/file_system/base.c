#include "base.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug.h"

/* base func */

void *_alloc_init_space(int size) {
	void *isp = malloc(size);
	memset(isp, 0, size);
	return isp;
}

void _write_table_head(void *p, char *name, int rec_len) {
	struct table *f = (struct table *)p;
	f->rec_len = rec_len;
	f->file_type = FT_TABLE;
	f->name_len = strlen(name);
	strcpy(f->name, name);
}

void _write_file_head(void *p, char *name) {
	struct file *f = (struct file *)p;
	f->file_type = FT_TABLE;
	f->name_len = strlen(name);
	strcpy(f->name, name);
}

void _write_content(void *p, char *content) {
	char *pc = (char *)p;
	strcpy(pc, content);
}

char *_read_file_name(struct superblock *sb, int iindex) {
	struct inode *inode = _get_inode(sb, iindex);
	struct file *f = (struct file *)_get_block(sb, (inode->block_list)[0]);
	return f->name;
}

char *_read_table_name(struct superblock *sb, int iindex) {
	struct inode *inode = _get_inode(sb, iindex);
	struct table *f = (struct table *)_get_block(sb, (inode->block_list)[0]);
	return f->name;
}

void _del_dir_entry(struct superblock *sb, int findex, int iindex) {
	int i, j;

	struct inode *finode = _get_inode(sb, findex);
	struct table *t = (struct table *)_get_block(sb, (finode->block_list)[0]);
	struct dir_entry *del = (struct dir_entry *)(t + 1);
	for (i = 0; i < t->rec_len; i++) {
		if (iindex == del[i].inode_index) {
			for (j = i; j < t->rec_len; j++) {
				memcpy(del + i, del + j, sizeof(struct dir_entry));
			}
			(t->rec_len)--;
			return;
		}
	}
}

/* get func */

struct block *_get_block(struct superblock *sb, int block_index) {
	return (struct block *)sb + block_index;
}

struct group_desc *_get_group_desc(struct superblock *sb, int group_index) {
	return (struct group_desc *)((struct block *)sb + sb->first_group_block) + group_index;
}

struct inode *_get_inode(struct superblock *sb, int inode_index) {
	struct group_desc *gd = _get_group_desc(sb, inode_index / sb->inodes_per_group);
	return (struct inode *)_get_block(sb, gd->inode_table) + (inode_index % sb->inodes_per_group);
}

int _get_current_inode_index(struct file_sys *file_sys) {
	return (file_sys->inode_index_list)[file_sys->inode_count];
}

void _set_current_inode_index(struct file_sys *file_sys, int iindex) {
	(file_sys->inode_index_list)[++(file_sys->inode_count)] = iindex;
}

/* find func */

int _find_near_inode_index(struct superblock *sb) {
	int i, j;

	struct group_desc *gdl = _get_group_desc(sb, 0);
	for (i = 0; i < sb->groups; i++) {
		for (j = 0; j < sb->inodes_per_group; j++) {
			if (gdl[i].inode_bitmap[j] == 0) {
				return i * sb->inodes_per_group + j;
			}
		}
	}
	
	return -1;
}

int _find_near_block_index(struct superblock *sb, int group_index) {
	int i;

	struct group_desc *gd = _get_group_desc(sb, group_index);
	for (i = 0; i < sb->groups; i++) {
		if (gd->block_bitmap[i] == 0) {
			return sb->first_data_block + group_index * (sb->blocks_per_group + 1) + i + 1;
		}
	}

	return -1;
}

int _find_file_iindex(struct file_sys *file_sys, char *file_name) {
	int i;

	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	struct inode *inode = _get_inode(sb, _get_current_inode_index(file_sys));
	struct table *t = (struct table *)_get_block(sb, (inode->block_list)[0]);
	struct dir_entry *del = (struct dir_entry *)(t + 1);
	for (i = 0; i < t->rec_len; i++) {
		if (del[i].file_type == FT_OTHER) {
			if (strcmp(file_name, _read_file_name(sb, del[i].inode_index)) == 0) {
				return del[i].inode_index;
			}
		}
	}

	return -1;
}

int _find_table_iindex(struct file_sys *file_sys, char *file_name) {
	int i;

	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	struct inode *inode = _get_inode(sb, _get_current_inode_index(file_sys));
	struct table *t = (struct table *)_get_block(sb, (inode->block_list)[0]);
	struct dir_entry *del = (struct dir_entry *)(t + 1);
	for (i = 0; i < t->rec_len; i++) {
		if (del[i].file_type == FT_TABLE) {
			if (strcmp(file_name, _read_file_name(sb, del[i].inode_index)) == 0) {
				return del[i].inode_index;
			}
		}
	}

	return -1;
}

int _find_user_index(struct file_sys *file_sys, char *user_name) {
	int i;

	struct user *ul = file_sys->user_list;
	for (i = 0; i < file_sys->user_count; i++) {
		if (strcmp(user_name, ul[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

char *_find_name_of_uid(struct file_sys *file_sys, int uid) {
	int i;
	struct user *ul = file_sys->user_list;
	for (i = 0; i < file_sys->user_count; i++) {
		if (ul[i].id == uid) {
			return ul[i].name;
		}
	}
	return NULL;
}

/* init func */

void _init_base_size(struct base_size *s, int count, int size) {
	s->count = count;
	s->log_size = size;
	s->free_count = count;
}

void _init_base_time(struct base_time *t, time_t now) {
	t->ctime = now;
	t->atime = now;
	t->atime = now;
}

void _init_group_list(struct block *bl) {
	int i;

	struct superblock *sb = (struct superblock *)(bl[0].content);
	struct group_desc *gdl = (struct group_desc *)(bl[sb->first_group_block].content);
	for (i = 0; i < sb->groups; i++) {
		gdl[i].free_blocks = sb->blocks_per_group;
		gdl[i].free_inodes = sb->inodes_per_group;
		gdl[i].inode_table = sb->first_data_block + (sb->blocks_per_group + 1) * i;
		// bitmap already init
	}
}

void _init_superblock(struct block *bl, int block_count, int block_size) {
	struct superblock *sb = (struct superblock *)(bl[0].content);

	sb->valid_bit = 1;
	sb->inodes_per_group = INODES_PER_GROUP;
	sb->blocks_per_group = BLOCKS_PER_GROUP;

	sb->first_super_block = 0;
	sb->first_group_block = (sizeof(struct superblock) + 1) / sizeof(struct block) + 1;
	sb->first_data_block = sb->first_group_block + 1;

	sb->groups = (block_count - sb->first_data_block - 1) / sb->blocks_per_group + 1;

	_init_base_size(&(sb->block), block_count, block_size);
	_init_base_size(&(sb->inode), sb->groups * sb->inodes_per_group, sizeof(struct inode));
	_init_base_time(&(sb->time), time(NULL));
}

/* new func */

struct block *_new_block_list(int block_count, int block_size) {
	struct block *bl = (struct block *)_alloc_init_space(block_size * block_count);
	_init_superblock(bl, block_count, block_size);
	_init_group_list(bl);
	return bl;
}





