#include "file_sys.h"
#include "base.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"

/* inode func */

int _create_inode(struct superblock *sb, int uid, int content_size);
int _add_block_by_inode(struct superblock *sb, int iindex);
void _bitmap_add_index(struct superblock *sb, int iindex);
void _bitmap_del_index(struct superblock *sb, int iindex);
void _add_dir_entry_in_table(struct superblock *sb, int tindex, int iindex, enum file_type ft);

/* file sys func */

struct file_sys *_new_file_sys() {
	struct file_sys *fs = (struct file_sys *)_alloc_init_space(sizeof(struct file_sys));
	fs->block_list = _new_block_list(BLOCK_COUNT, BLOCK_SIZE);
	fs->current_uid = 1; // root
	fs->current_path = "/";

	// table init
	struct superblock *sb = (struct superblock *)(fs->block_list);
	int iindex = _create_inode(sb, fs->current_uid, sizeof(struct table));
	int bindex = _add_block_by_inode(sb, iindex);
	_write_table_head(_get_block(sb, bindex), fs->current_path, 0);
	_bitmap_add_index(sb, iindex);

	// file sys init
	_set_current_inode_index(fs, iindex);
	(fs->inode_count)--; // 应该指向当前文件夹
	fs->user_count = 1;
	
	// user init
	struct user *user = fs->user_list;
	user->id = 1;
	user->property = 3;
	strcpy(user->name, "root");
	strcpy(user->password, "root");

	return fs;
}

struct file_sys *new_file_sys(char *path) {
	if (!path) {
		return _new_file_sys();
	}

	// read path file
	return NULL;
}

void free_file_sys(struct file_sys *file_sys) {
	if (file_sys) {
		if (file_sys->block_list) {
			free(file_sys->block_list);
		}
		free(file_sys);
	}
}

void format_file_sys(struct file_sys *file_sys) {
	free_file_sys(file_sys);
	file_sys = new_file_sys(NULL);
}

/* file and table */

void create_file(struct file_sys *file_sys, char *name, char *content) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);

	int iindex = _create_inode(sb, file_sys->current_uid, strlen(content) + sizeof(struct file));

	// head
	int bindex = _add_block_by_inode(sb, iindex);
	struct block *block = _get_block(sb, bindex);
	_write_file_head(block, name);
	_write_content((char *)((struct file *)block + 1), content);
	_bitmap_add_index(sb, iindex);
	_add_dir_entry_in_table(sb,  _get_current_inode_index(file_sys), iindex, FT_OTHER);
}

void create_table(struct file_sys *file_sys, char *name) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);

	int iindex = _create_inode(sb, file_sys->current_uid, sizeof(struct table));
	int bindex = _add_block_by_inode(sb, iindex);
	_write_table_head(_get_block(sb, bindex), name, 0);
	_bitmap_add_index(sb, iindex);
	_add_dir_entry_in_table(sb,  _get_current_inode_index(file_sys), iindex, FT_TABLE);
}

void _delete_file(struct superblock *sb, int iindex) {
	struct inode *inode = _get_inode(sb, iindex);
	_bitmap_del_index(sb, iindex);
	inode->size = 0;
}

void delete_file(struct file_sys *file_sys, char *name) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	int findex = _find_file_iindex(file_sys, name);
	if (findex == -1) return;
	_delete_file(sb, findex);
	_del_dir_entry(sb,  _get_current_inode_index(file_sys), findex);
}

void _delete_table(struct superblock *sb, int iindex) {
	int i;

	struct inode *inode = _get_inode(sb, iindex);
	struct table *t = (struct table *)_get_block(sb, (inode->block_list)[0]);
	struct dir_entry *del = (struct dir_entry *)(t + 1);
	for (i = 0; i < t->rec_len; i++) {
		if (del[i].file_type == FT_OTHER) {
			_delete_file(sb, del[i].inode_index);
		} else {
			_delete_table(sb, del[i].inode_index);
		}
	}
}

void delete_table(struct file_sys *file_sys, char *name) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	int findex = _find_table_iindex(file_sys, name);
	if (findex == -1) return;
	_delete_table(sb, findex);
	_del_dir_entry(sb, _get_current_inode_index(file_sys), findex);
}

void move_table(struct file_sys *file_sys, char *path) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	int ic = file_sys->inode_count;
	
	char str[256]; strcpy(str, path);
	char *ptr = strtok(str, "/");

	if (ptr != NULL && strcmp(ptr, ".") == 0) {
		ptr = strtok(NULL, "/");
	}

	while (ptr != NULL && strcmp(ptr, "..") == 0) {
		if (_get_current_inode_index(file_sys) == 0) {
			printf("root have no '..'\n");
			return;
		}
		(file_sys->inode_count)--;
		ptr = strtok(NULL, "/");
	}
	
	while(ptr != NULL) {
		char *name = ptr;

		int iindex = _find_table_iindex(file_sys, name);
		if (iindex == -1) {
			printf("'%s' not find in '%s'\n", name, path);
			file_sys->inode_count = ic;
			return;
		}

		_set_current_inode_index(file_sys, iindex);
		ptr = strtok(NULL, "/");
	}
}

void _show_table(struct superblock *sb, int iindex) {
	int i;

	struct inode *inode = _get_inode(sb, iindex);
	struct table *t = (struct table *)_get_block(sb, (inode->block_list)[0]);
	struct dir_entry *del = (struct dir_entry *)(t + 1);
	char *name;

	for (i = 0; i < t->rec_len; i++) {
		if (del[i].file_type == FT_OTHER) {
			name = _read_file_name(sb, del[i].inode_index);
		} else {
			name = _read_table_name(sb, del[i].inode_index);
		}
		printf("%s\t", name);
	}
	printf("\n");
}

void show_table(struct file_sys *file_sys, char *path) {
	int ic = file_sys->inode_count;
	struct superblock *sb = (struct superblock *)(file_sys->block_list);

	move_table(file_sys, path);
	_show_table(sb, _get_current_inode_index(file_sys));
	
	file_sys->inode_count = ic;
}

void show_current_path(struct file_sys *file_sys) {
	struct superblock *sb = (struct superblock *)(file_sys->block_list);
	char name[64];
	char cpath[256] = "\0";
	char path[256] = "\0";

	int ic = file_sys->inode_count;
	while (ic > 0) {
		int iindex = (file_sys->inode_index_list)[ic];
		strcpy(name, _read_file_name(sb, iindex));
		sprintf(path, "%s/%s", name, cpath);
		strcpy(cpath, path);
		ic--;
	}
	printf("/%s\n", path);
}

/* user log */

void add_user(struct file_sys *file_sys, char *user_name, char *password, int property) {
	if (file_sys->current_uid != 1) {
		printf("current user is not root, can not create user!\n");
		return;
	}
	struct user *user = file_sys->user_list + file_sys->user_count;
	user->id = file_sys->user_count + 1;
	user->property = property;
	strcpy(user->name, user_name);
	strcpy(user->password, password);
	(file_sys->user_count)++;
}

void del_user(struct file_sys *file_sys, char *user_name) {
	if (file_sys->current_uid != 1) {
		printf("current user is not root, can not delete user!\n");
		return;
	}

	int i;
	struct user *ul = file_sys->user_list;
	int user_index = _find_user_index(file_sys, user_name);
	if (user_index == 0) {
		printf("root user can not delete!\n");
		return;
	} else if (user_index == -1) {
		printf("%s is not exist!\n", user_name);
	}

	memcpy(ul + user_index, ul + user_index + 1, sizeof(struct user) * (file_sys->user_count - user_index - 1));
	(file_sys->user_count)--;
}

void user_login(struct file_sys *file_sys, char *user_name, char *password) {
	struct user *ul = file_sys->user_list;
	int user_index = _find_user_index(file_sys, user_name);
	if (user_index == -1) {
		printf("%s is not exist!\n", user_name);
		return;
	}

	if (strcmp(password, ul[user_index].password)) {
		printf("password is not right!\n");
		return;
	}

	file_sys->current_uid = ul[user_index].id;
}

void user_logout(struct file_sys *file_sys) {
	file_sys->current_uid = 0;
}

void show_current_user(struct file_sys *file_sys) {
	char *name = _find_name_of_uid(file_sys, file_sys->current_uid);
	if (name) printf("current user: %s\n", name);
	else printf("you are tourist.\n");
}

