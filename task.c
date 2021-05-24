#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void remove_dir_and_files_if_exists(const char *dirname) {
	DIR *dir = opendir(dirname);
	
	if (dir){
		struct dirent *next_file;
		char filepath[256];

		while ((next_file = readdir(dir)) != NULL) {
			sprintf(filepath, "%s/%s", dirname, next_file->d_name);
			remove(filepath);
		}

		closedir(dir);
		rmdir(dirname);
	}
}

int can_open(const char *file_path) {
	int fd;
	if ((fd = open(file_path, O_RDONLY, 0666)) >= 0) {
		if (close(fd) != 0) {
			printf("Cannot close file! Errno: %d\n", errno);
			exit(-1);
		}
		return 1;
	}
	return 0;
}

void create_file(const char *file_path) {
	int fd;
	if ((fd = open(file_path, O_RDONLY | O_CREAT, 0666)) < 0) {
		printf("Cannot create file! Errno: %d\n", errno);
		exit(-1);
	}
	if (close(fd) != 0) {
		printf("Cannot close file! Errno: %d\n", errno);
		exit(-1);
	}
}

int main(int argc, char **argv) {
	const char *directory_name = "files";
	const char *file_name = "myfile";
	const char *link_prefix = "link";
	char link_from[20];
	char link_to[20];
	int i = 0;
	int fd = 0;

	// Delete directory "files" and files in it, if exists.
	remove_dir_and_files_if_exists(directory_name);

	// Create directory "files".
	mkdir(directory_name, 0777);

	sprintf(link_to, "%s/%s", directory_name, file_name);
	sprintf(link_from, "%s/%s%d", directory_name, link_prefix, i);

	// Create file "myfile".
	create_file(link_to);

	// Create first link to file.
	symlink(file_name, link_from);

	// While can open link, create new link to link.
	while (can_open(link_from)) {
		// Relative path, thus no directory name.
		sprintf(link_to, "%s%d", link_prefix, i);
		sprintf(link_from, "%s/%s%d", directory_name, link_prefix, ++i);
		symlink(link_to, link_from);
	}

	printf("Symbolic links max depth is %d.\n", i);

	// Delete directory "files" and files in it.
	remove_dir_and_files_if_exists(directory_name);

	return 0;
}