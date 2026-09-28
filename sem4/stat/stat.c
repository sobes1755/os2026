#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>  // major(), minor()
#include <limits.h> // PATH_MAX
#include <unistd.h>  // readlink
#include <pwd.h>  // getpwuid()
#include <grp.h>  // getgrgid()
#include <time.h>  // ctime

int
main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage: %s file ...\n", argv[0]);
    return EXIT_FAILURE;
  }

  while (*++argv != nullptr) {

    // Retrieving file metadata....

    struct stat sb;  // stat buffer

    if (lstat(*argv, &sb) == -1) {
      perror("Error: lstat failed");
      continue;
    }

    // Printing file name, file type...

    printf("File name: %s\n", *argv);
    printf("File type: ");

    // using either bitwise AND operator and "object like macros" S_IFREG, ..., S_IFSOCK...

    switch (sb.st_mode & S_IFMT) {
      case S_IFREG: printf("regular file\n"); break;
      case S_IFDIR: printf("directory\n"); break;
      case S_IFCHR: printf("character device\n"); break;
      case S_IFBLK: printf("block device\n"); break;
      case S_IFLNK: printf("symbolic (soft) link\n"); break;
      case S_IFIFO: printf("FIFO or pipe\n"); break;
      case S_IFSOCK: printf("socket\n"); break;
      default: printf("unknown file type?\n");
    }

    // or "function-like macros" S_ISREG(), ..., S_ISSOCK()...

    /*

    if (S_ISREG(sb.st_mode))
      printf("regular file\n");
    else if (S_ISDIR(sb.st_mode))
      printf("directory\n");
    else if (S_ISCHR(sb.st_mode))
      printf("character device\n");
    else if (S_ISBLK(sb.st_mode))
      printf("block device\n");
    else if (S_ISLNK(sb.st_mode))
      printf("symbolic (soft) link\n");
    else if (S_ISFIFO(sb.st_mode))
      printf("FIFO or pipe\n\n");
    else if (S_ISSOCK(sb.st_mode))
      printf("socket\n");
    else
      printf("unknown file type?\n");

    */

    // Printing link target (in case of symnlink)...

    if (S_ISLNK(sb.st_mode)) {

      ssize_t lb_size = (sb.st_size > 0) ? sb.st_size + 1: PATH_MAX;
      char *lb = malloc(lb_size);

      if (lb == nullptr) {
        perror("Error: malloc failed");
        exit(EXIT_FAILURE);
      }

      ssize_t lb_read = readlink(*argv, lb, lb_size);

      if (lb_read == -1) {
        perror("Error: readlink failed");
      } else if (lb_read == lb_size) {
        printf("Symbolic link (may be truncated): %s -> %.*s\n", *argv, (int) lb_read, lb);
      } else {
        printf("Symbolic link: %s -> %.*s\n", *argv, (int) lb_read, lb);
      }

      free(lb);

    }

    // Printing device major and minor numbers and i-node number...

    printf("Device containing i-node: major = %ld, minor = %ld\n", (long) major(sb.st_dev), (long) minor(sb.st_dev));
    printf("I-node number of file: %llu\n", (unsigned long long) sb.st_ino);
    printf("Number of (hard) links to file: %llu\n", (unsigned long long)sb.st_nlink);

    // Printing file size...

    printf("Total file size (bytes): %lld\n", (long long) sb.st_size);
    printf("512B blocks allocated: %lld\n", (long long) sb.st_blocks);
    printf("Optimal I/O block size (bytes): %ld\n", (long) sb.st_blksize);

    // Printing file user and file group...

    struct passwd *pw;
    struct group *gr;

    pw = getpwuid(sb.st_uid);
    gr = getgrgid(sb.st_gid);

    printf("User: ");

    if (pw)
      printf("%s (UID: %d)\n", pw->pw_name, sb.st_uid);
    else
      printf("unknown (UID: %d)\n", sb.st_uid);

    printf("Group: ");
    
    if (gr)
      printf("%s (GID: %d)\n", gr->gr_name, sb.st_gid);
    else
      printf("unknown (GID: %d)\n", sb.st_gid);

    // Printing permissions...

    printf("Permissions: %c%c%c%c%c%c%c%c%c (0%o)\n",

      (sb.st_mode & S_IRUSR) ? 'r' : '-',
      (sb.st_mode & S_IWUSR) ? 'w' : '-',
      (sb.st_mode & S_IXUSR) ? ((sb.st_mode & S_ISUID) ? 's' : 'x') : ((sb.st_mode & S_ISUID) ? 'S' : '-'),  // SUID bit taken into account

      (sb.st_mode & S_IRGRP) ? 'r' : '-',
      (sb.st_mode & S_IWGRP) ? 'w' : '-',
      (sb.st_mode & S_IXGRP) ? ((sb.st_mode & S_ISGID) ? 's' : 'x') : ((sb.st_mode & S_ISGID) ? 'S' : '-'),  // SGID bit taken into account

      (sb.st_mode & S_IROTH) ? 'r' : '-',
      (sb.st_mode & S_IWOTH) ? 'w' : '-',
      (sb.st_mode & S_IXOTH) ? ((sb.st_mode & S_ISVTX) ? 't' : 'x') : ((sb.st_mode & S_ISVTX) ? 'T' : '-'),  // VTX (sticky) bit taken into account

      sb.st_mode & (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO));

    // Prinnting timestamps...

    // Format specifier "%.24s" is used because ctime(...)[24] == '\n'.

    printf("Last file access: %.24s\n", ctime(&sb.st_atime));
    printf("Last file modification: %.24s\n", ctime(&sb.st_mtime));
    printf("Last status change: %.24s\n\n", ctime(&sb.st_ctime));

  }

  return EXIT_SUCCESS;

}
