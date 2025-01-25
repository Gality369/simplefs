/*
 * A simple Filesystem for Linux Kernel 6.8.0.
 *
 * Initial Author:  Gality <gality369@gmail.com>
 * License: Creative Commons Zero v1.0 Universal -
 * https://creativecommons.org/publicdomain/zero/1.0/
 * Date: 2025-01-25
 */

#include <linux/init.h>
#include <linux/module.h>

static int simplefs_init(void) {
  printk(KERN_ALERT "Hello world from simplefs!\n");
  return 0;
}

static void simplefs_exit(void) {
  printk(KERN_ALERT "Goodbye from simplefs!\n");
}

module_init(simplefs_init);
module_exit(simplefs_exit);

MODULE_LICENSE("CC0");
MODULE_AUTHOR("Gality");