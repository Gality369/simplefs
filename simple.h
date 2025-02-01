#include <stdint.h>
#include <sys/types.h> 

#define SIMPLEFS_MAX_FILENAME_LEN 8
#define SIMPLEFS_MAX_FILES  32
#define SIMPLEFS_MAX_DATA_LEN 32

/**
 * `file` includes the regular file and directory.
 */
struct file_block {
  char filename[SIMPLEFS_MAX_FILENAME_LEN]; // file name
  uint8_t busy;                // 0: free, 1: busy
  mode_t mode;                 // file mode (regular file or directory)
  uint8_t idx;                 // file index
  uint8_t parent_idx;          // parent directory index
    
  char data[SIMPLEFS_MAX_DATA_LEN];     // file data
};