#include "simple.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	ssize_t ret;
  struct file_block root_file;

	if (argc != 2) {
		printf("Usage: mkfs-simplefs <device>\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("Error opening the device");
		return -1;
	}

  /* Initialize the root file */
  root_file.busy = 1;
  root_file.mode = S_IFDIR;
  root_file.idx = 0;
  root_file.parent_idx = 0; /* The parent of the root directory is itself */
  root_file.data[0] = '\0';
  root_file.filename[0] = '/';

	ret = write(fd, (char *)&root_file, sizeof(struct file_block));

	/* Just a redundant check. Not required ideally. */
	if (ret != sizeof(struct file_block))
		printf
		    ("bytes written [%d] are not equal to the default file_block size",
		     (int)ret);
	else
		printf("Super block written successfully");

	close(fd);

	return 0;
}
