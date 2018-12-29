#ifndef __BASE__
#define __BASE__

#include "file_sys.h"
#include <stdio.h>

/* base func */
void *_alloc_init_space(int size);

void _write_table_head(void *p, char *name, int rec_len);
void _write_file_head(void *p, char *name);
void _write_content(void *p, char *content);

char *_read_file_name(struct superblock *sb, int iindex);
char *_read_table_name(struct superblock *sb, int iindex);

void _del_dir_entry(struct superblock *sb, int findex, int iindex);

/* get func */
struct group_desc *_get_group_desc(struct superblock *sb, int group_index);
struct inode *_get_inode(struct superblock *sb, int inode_index);
struct block *_get_block(struct superblock *sb, int block_index);

int _get_current_inode_index(struct file_sys *file_sys);
void _set_current_inode_index(struct file_sys *file_sys, int iindex);

/* find func */
int _find_near_inode_index(struct superblock *sb);
int _find_near_block_index(struct superblock *sb, int group_index);
int _find_file_iindex(struct file_sys *file_sys, char *file_name);
int _find_table_iindex(struct file_sys *file_sys, char *file_name);

int _find_user_index(struct file_sys *file_sys, char *user_name);
char *_find_name_of_uid(struct file_sys *file_sys, int uid);

/* init func */
void _init_base_size(struct base_size *s, int count, int size);
void _init_base_time(struct base_time *t, time_t now);
void _init_group_list(struct block *bl);
void _init_superblock(struct block *bl, int block_count, int block_size);

/* new func */
struct block *_new_block_list(int block_count, int block_size);

#endif