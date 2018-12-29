#include "file_sys.h"
#include "base.h"
#include "debug.h"

int _create_inode(struct superblock *sb, int uid, int content_size) {
	int iindex = _find_near_inode_index(sb);
	
	struct inode *inode = _get_inode(sb, iindex);
	_init_base_time(&(inode->time), time(NULL));
	inode->mode = M_ALL;
	inode->uid = uid;
	inode->size = content_size;
	
	return iindex;
}

int _add_block_by_inode(struct superblock *sb, int iindex) {
	int gindex = iindex / sb->inodes_per_group;
	int bindex = _find_near_block_index(sb, gindex);

	struct inode *inode = _get_inode(sb, iindex);
	(inode->block_list)[inode->size / (sb->block).log_size] = bindex;
	
	return bindex;
}

void _bitmap_add_index(struct superblock *sb, int iindex) {
	int i;

	struct inode *inode = _get_inode(sb, iindex);
	int gindex = iindex / sb->inodes_per_group;
	struct group_desc *group = _get_group_desc(sb, gindex);
	(group->inode_bitmap)[iindex % sb->inodes_per_group] = 1;

	int block_size = (sb->block).log_size;
	for (i = 0; i <= inode->size / block_size; i++) {
		(group->block_bitmap)[
			(inode->block_list)[i] 
			- sb->first_data_block 
			- gindex * (sb->blocks_per_group + 1) - 1
			] = 1;
	}
}

void _bitmap_del_index(struct superblock *sb, int iindex) {
	int i;

	struct inode *inode = _get_inode(sb, iindex);
	int gindex = iindex / sb->inodes_per_group;
	struct group_desc *group = _get_group_desc(sb, gindex);
	(group->inode_bitmap)[iindex % sb->inodes_per_group] = 0;

	int block_size = (sb->block).log_size;
	for (i = 0; i <= inode->size / block_size; i++) {
		(group->block_bitmap)[
			(inode->block_list)[i] 
			- sb->first_data_block 
			- gindex * (sb->blocks_per_group + 1) - 1
			] = 0;
	}
}

void _add_dir_entry_in_table(struct superblock *sb, int tindex, int iindex, enum file_type ft) {
	struct inode *tinode = _get_inode(sb, tindex);
	struct table *t = (struct table *)_get_block(sb, (tinode->block_list)[0]);
	struct dir_entry *de = (struct dir_entry *)(t + 1) + t->rec_len;
	de->inode_index = iindex;
	de->file_type = ft;
	(t->rec_len)++;
}

