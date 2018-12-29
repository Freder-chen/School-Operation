#ifndef __FILE_SYS__
#define __FILE_SYS__

#include <time.h>

#define BLOCK_COUNT 256
#define BLOCK_SIZE 1024

#define BLOCKS_PER_GROUP 32
#define INODES_PER_GROUP 8

#define INODE_N_BLOCKS 11

#define NAME_LEN 255
#define USER_MAX 10

/*
 * Base struct
 */

struct base_size {
	int count;      // 块数
	int log_size;   // 每块大小
	int free_count; // 剩余块数
};

struct base_time {
	time_t ctime; // 创建时间
	time_t atime; // 最近读取时间
	time_t mtime; // 最近修改时间
};

/*
 * File struct
 */

// 记录文件内容
struct block {
	unsigned char content[BLOCK_SIZE];
};

/*
 * File sys struct
 */

// 记录文件属性
enum mode {
	M_NONE = 0,
	M_READ = 1,
	M_WREITE = 2,
	M_ALL = 3
};

struct inode {	
	enum mode mode;        // 文件权限
	int uid;               // 所属用户id
	int size;              // 文件大小, byte
	struct base_time time; // 时间信息
	int block_list[INODE_N_BLOCKS]; // block数组指针
};

struct group_desc {
	unsigned char block_bitmap[BLOCKS_PER_GROUP]; // 指向block bitmap
	unsigned char inode_bitmap[INODES_PER_GROUP]; // 指向inode bitmap
	int inode_table; // 指向inode 第一个块的块ID
	int free_blocks; // 剩余block块数
	int free_inodes; // 剩余inode块数
};

struct superblock {
	int valid_bit; // 是否挂载
	int groups;    // group个数
	int blocks_per_group;
	int inodes_per_group;
	
	int first_super_block;
	int first_group_block;
	int first_data_block;

	struct base_size block;
	struct base_size inode;
	struct base_time time;
};

/*
 * File struct
 */
enum file_type {
	FT_UNKOWN = 0,
	FT_TABLE = 1,
	FT_OTHER = 2
};

struct dir_entry {
	enum file_type file_type; // 文件类型
	int inode_index;          // 所属inode
};

struct file {
	unsigned char name_len;
	enum file_type file_type; // 文件类型
	char name[NAME_LEN];
};

struct table {
	unsigned char name_len;
	short int rec_len;        // 目录条目长度
	enum file_type file_type; // 文件类型
	char name[NAME_LEN];
};

/*
 * User struct
 */

struct user {
	int id;
	int property;
	char name[NAME_LEN];
	char password[NAME_LEN];
};

/*
 * File system
 */

struct file_sys {
	struct block *block_list;

	char *current_path;      // 文件系统的当前路径
	int inode_count;
	int inode_index_list[10];

	int current_uid;         // 文件系统的当前用户
	int user_count;          // user个数
	struct user user_list[USER_MAX];  // user数组指针
};

/*
 * Function prototypes
 */

// file sys

struct file_sys *new_file_sys(char *path);
void free_file_sys(struct file_sys *file_sys);
void format_file_sys(struct file_sys *file_sys);

// file and table

void create_file(struct file_sys *file_sys, char *name, char *content);
void delete_file(struct file_sys *file_sys, char *name);

void create_table(struct file_sys *file_sys, char *name);
void delete_table(struct file_sys *file_sys, char *name);

void move_table(struct file_sys *file_sys, char *path);
void show_table(struct file_sys *file_sys, char *path);

void show_current_path(struct file_sys *file_sys);

// user 

void add_user(struct file_sys *file_sys, char *user_name, char *password, int property);
void del_user(struct file_sys *file_sys, char *user_name);
void user_login(struct file_sys *file_sys, char *user_name, char *password);
void user_logout(struct file_sys *file_sys);

void show_current_user(struct file_sys *file_sys);

#endif