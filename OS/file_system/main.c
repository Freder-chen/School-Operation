#include "file_sys.h"
#include "base.h"
#include <stdio.h>
#include "debug.h"
#include <string.h>

void _print_head(struct file_sys *file_sys) {
	printf("%s >", _find_name_of_uid(file_sys, file_sys->current_uid));
}

void shell(struct file_sys *file_sys) {
	char comm[256], str[256];

	while (1) {
		_print_head(file_sys);
		
		gets(comm);
		strcpy(str, comm);

		char *ptr = strtok(str, " ");
		if (strcmp(ptr, "exit") == 0) {
			break;
		}
		else if (strcmp(ptr, "ls") == 0) {
			ptr = strtok(NULL, " ");
			if (!ptr) ptr = ".";
			show_table(file_sys, ptr);
		}
		else if (strcmp(ptr, "cd") == 0) {
			ptr = strtok(NULL, " ");
			move_table(file_sys, ptr);
		}
		else if (strcmp(ptr, "pwd") == 0) {
			show_current_path(file_sys);
		}
		else if (strcmp(ptr, "mkdir") == 0) {
			ptr = strtok(NULL, " ");
			create_table(file_sys, ptr);
		}
		else if (strcmp(ptr, "touch") == 0) {
			ptr = strtok(NULL, " ");
			create_file(file_sys, ptr, "abcd");
		}
		else if (strcmp(ptr, "rm") == 0) {
			ptr = strtok(NULL, " ");
			delete_file(file_sys, ptr);
		}
		else if (strcmp(ptr, "rmdir") == 0) {
			ptr = strtok(NULL, " ");
			delete_table(file_sys, ptr);	
		}
		else if (strcmp(ptr, "logout") == 0) {
			user_logout(file_sys);
			printf("please input user name: ");
		}
	}
}

int main() {
	struct file_sys *file_sys = new_file_sys(NULL);
	
	create_table(file_sys, "test");
	create_file(file_sys, "a.txt", "abcd");
	
	move_table(file_sys, "./test/");
	create_table(file_sys, "test");
	create_file(file_sys, "b.txt", "abcd");
	
	move_table(file_sys, "../");

	show_current_user(file_sys);

	add_user(file_sys, "freder", "123", 1);
	user_login(file_sys, "freder", "123");
	show_current_user(file_sys);

	user_logout(file_sys);
	show_current_user(file_sys);

	user_login(file_sys, "root", "root");
	del_user(file_sys, "freder");
	show_current_user(file_sys);

	printf("--------- into shell -------\n");

	shell(file_sys);
	
	free_file_sys(file_sys);
	// save(dms, path);
	return 0;
}
