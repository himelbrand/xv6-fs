# xv6-fs
## OUR ADDITIONS

* **Extended maximum file size from 70KB to 16MB by adding single-indirect and double-indirect links to data blocks to the inode**


* **Added support for symbolic links :**

```bash
  ln -s old_path new_path
```
### **int symlink (const char *oldpath, const char * newpath)**

• **oldpath** The path the new file contains. This will be written as the content of the file.
Note that it is OK to create a symbolic link to a file that doesn’t exists.

• **newpath** – The new symbolic link file path. If successful, a new symbolic link file will be
created in this path.

The function returns 0 upon success and -1 on failure. Note that if newpath exists the function
should fail.

### **int readlink (const char * pathname, char * buf, size_t bufsize)**

• **pathname** – A path to a symbolic link file.

• **buf** – A pointer to a buffer into which the content of pathname symbolic link will be
read.

• **bufsize** – the size of the buf buffer.

The function returns 0 upon success and -1 on failure. The function will fail if either: 1)
pathname does not exists, 2) it is not a symbolic link, or 3) bufsize is smaller than the length of
pathname.


* **Added file tagging :**

### **int ftag (int fd, const char * key, const char * value)**

This function adds the key=value tag to the file descriptor. If the key already exists, the value will
be overwritten.

• **fd** – The file descriptor of the file to which the tag will be added

• **key** – The key, a string with maximum length of 10 characters.

• **value** – The value, a string with maximum length of 30 characters

The function returns 0 upon success and -1 on failure.

### **int funtag (int fd, const char * key)**

Untags the key value tag referred to by key for file descriptor fd.

• **fd** – The file descriptor from which we want to remove the tag

• **key** – The key, a string with maximum length of 10 characters, identifying the tag to be
removed from the file

The function return 0 on success and -1 on failure.

### **int gettag (int fd, const char * key, char * buf)**

This function reads the value linked to key for the given file descriptor fd.

• **fd** – The file descriptor we want to read from.

• **key** – The key that we want to read.

• **buf** – The value will be written to this buffer.

The function returns the length of value if successful and -1 on failure


* **Find Application :**
```bash
  find <path> <options> <tests>
```
### Options
-**follow**
Dereference symbolic links. If a symbolic link is encountered, apply tests to the target of
the link. If a symbolic link points to a directory, then descend to it.

### Tests
**-name** <file name>
All files named (exactly, no wildcards) file name
  
**-size** <(+/-)n>
File is of size n (exactly), +n (more then n), -n (less then n)

**-type** <c>
File is of type c:
d directory
f regular file
s soft (symbolic) link
  
**-tag** <key>=<value>
File is tagged by a specified key-value pair. If value equals “?” then all files having the
key key are matched, regardless of the value. You may assume key and value do not
contain spaces.
  
### Examples

```bash
find / -type d -name home
```
Searches the whole file system for directories whose name is “home”

```bash
find /src -name xv6
```
Searches /src and all its subdirectories recursively for files, directories and links whose
name is “xv6”
```bash
find / -type f -size +1024
```
Finds all the files whose size is greater than 1KB (=1024 bytes)
```bash
find / -type f -tag episode=s03e07
```
Finds all files with a tag whose key equals ``episode’’ and whose key equals “s03e07”.
```bash
find / -d -tag genre=?
```
Finds all directories which have a tag with the key “genre”

# ORIGINAL XV6 README

xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

## ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also http://pdos.csail.mit.edu/6.828/2016/xv6.html, which
provides pointers to on-line resources for v6.

xv6 borrows code from the following sources:
    JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
    Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
    FreeBSD (ioapic.c)
    NetBSD (console.c)

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by Silas
Boyd-Wickizer, Anton Burtsev, Cody Cutler, Mike CAT, Tej Chajed, Nelson Elhage,
Saar Ettinger, Alice Ferrazzi, Nathaniel Filardo, Peter Froehlich, Yakir Goaron,
Shivam Handa, Bryan Henry, Jim Huang, Alexander Kapshuk, Anders Kaseorg,
kehao95, Wolfgang Keller, Eddie Kohler, Austin Liew, Imbar Marinescu, Yandong
Mao, Hitoshi Mitake, Carmi Merimovich, Joel Nider, Greg Price, Ayan Shafqat,
Eldar Sehayek, Yongming Shen, Cam Tenny, Rafael Ubal, Warren Toomey, Stephen Tu,
Pablo Ventura, Xi Wang, Keiichi Watanabe, Nicolas Wolovick, Grant Wu, Jindong
Zhang, Icenowy Zheng, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2016 Frans Kaashoek, Robert Morris, and Russ Cox.

## ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu). The main purpose of xv6 is as a teaching
operating system for MIT's 6.828, so we are more interested in
simplifications and clarifications than new features.

## BUILDING AND RUNNING XV6

To build xv6 on an x86 ELF machine (like Linux or FreeBSD), run
"make". On non-x86 or non-ELF machines (like OS X, even on x86), you
will need to install a cross-compiler gcc suite capable of producing
x86 ELF binaries. See http://pdos.csail.mit.edu/6.828/2016/tools.html.
Then run "make TOOLPREFIX=i386-jos-elf-". Now install the QEMU PC
simulator and run "make qemu".

