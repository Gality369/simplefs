/*
 * A simple Filesystem for Linux Kernel 6.8.0.
 *
 * Initial Author:  Gality <gality369@gmail.com>
 * License: GNU General Public License v3 - https://www.gnu.org/licenses/gpl-3.0.html
 * Date: 2025-01-30
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

struct inode *simplefs_get_inode(struct super_block *sb,
				 const struct inode *dir, umode_t mode,
				 dev_t dev);
int simplefs_fill_super(struct super_block *sb, void *data, int silent);

/**
 * @sb: The super block of the filesystem (inodes, blocks, etc).
 * @dir: The parent directory's inode.
 * @mode: The mode of the inode to be created (S_IFDIR, S_IFREG, etc).
 * @dev: The device id of the inode to be created.
 * @return: The inode created.
 * 
 * This function creates an inode for the filesystem, initializes it 
 * and returns it. For now, it only creates inodes for root directories.
 */
struct inode *simplefs_get_inode(struct super_block *sb,
				 const struct inode *dir, umode_t mode,
				 dev_t dev)
{
	struct inode *inode = new_inode(sb);

	if (inode) {
		inode->i_ino = get_next_ino();
		inode_init_owner(&nop_mnt_idmap, inode, dir, mode);

		inode->__i_atime = inode->__i_mtime = inode->__i_ctime = inode_set_ctime_current(inode);

		switch (mode & S_IFMT) {
		case S_IFDIR:
			/* i_nlink will be initialized to 1 in the inode_init_always function
			 * (that gets called inside the new_inode function),
			 * We change it to 2 for directories, for covering the "." entry */
			inc_nlink(inode);
			break;
		case S_IFREG:
		case S_IFLNK:
		default:
			printk(KERN_ERR
			       "simplefs can create meaningful inode for only root directory at the moment\n");
			return NULL;
			break;
		}
	}
	return inode;
}

/**
 * @sb: The superblock which is passed from the VFS to the filesystem.
 * @data: The mount arguments data that might be passed to the filesystem while mounting.
 * @silent: A flag to indicate whether the filesystem should print logs or not.
 * @return: 0 on success, and error code on failure.
 * 
 * This function fills the super block of the filesystem with necessary information.
 */
int simplefs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *inode;

	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = 0x20250130;

	inode = simplefs_get_inode(sb, NULL, S_IFDIR, 0);
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
		return -ENOMEM;

	return 0;
}

/**
 * @fs_type: The filesystem type structure that is registered with the kernel.
 * @flags: Mount flags (e.g. MS_RDONLY for read-only mounts).
 * @dev_name: The name of the device to be mounted (/dev/sda1, /dev/sdb1, etc).
 * @data: Extra data that might be passed to the filesystem while mounting.
 * @return: The root dentry of the filesystem that is mounted.
 * 
 * This function is called when the VFS is asked to mount this filesystem 
 * and returns the root dentry of the filesystem.
 */
static struct dentry *simplefs_mount(struct file_system_type *fs_type,
				     int flags, const char *dev_name,
				     void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, simplefs_fill_super);

	if (unlikely(IS_ERR(ret)))
		printk(KERN_ERR "Error mounting simplefs");
	else
		printk(KERN_INFO "simplefs is successfully mounted on [%s]\n",
		       dev_name);

	return ret;
}

static void simplefs_kill_superblock(struct super_block *s)
{
	printk(KERN_INFO
	       "simplefs superblock is destroyed. Unmount successful.\n");
	/* This is just a dummy function as of now. As our filesystem gets matured,
	 * we will do more meaningful operations here */
	return;
}

struct file_system_type simplefs_fs_type = {
	.owner = THIS_MODULE,
	.name = "simplefs",
	.mount = simplefs_mount,
	.kill_sb = simplefs_kill_superblock,
};

static int simplefs_init(void)
{
	int ret;

	ret = register_filesystem(&simplefs_fs_type);
	if (likely(ret == 0))
		printk(KERN_INFO "Successfully registered simplefs\n");
	else
		printk(KERN_ERR "Failed to register simplefs. Error:[%d]", ret);

	return ret;
}

static void simplefs_exit(void)
{
	int ret;

	ret = unregister_filesystem(&simplefs_fs_type);

	if (likely(ret == 0))
		printk(KERN_INFO "Successfully unregistered simplefs\n");
	else
		printk(KERN_ERR "Failed to unregister simplefs. Error:[%d]",
		       ret);
}

module_init(simplefs_init);
module_exit(simplefs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gality");