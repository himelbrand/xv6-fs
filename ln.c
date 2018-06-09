#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  if (argc != 3 || (argc != 4 || (argc == 4 && !strcmp(argv[2], "-s"))))
  {
    printf(2, "Usage: ln old new\n");
    printf(2, "or\n");
    printf(2, "Usage for symbolic link: ln -s old new\n");
    exit();
  }
  if (argc == 3)
  {
    if (link(argv[1], argv[2]) < 0)
      printf(2, "link %s %s: failed\n", argv[1], argv[2]);
  }
  else
  {
    if (symlink(argv[2], argv[3]) < 0)
      printf(2, "symlink %s %s: failed\n", argv[2], argv[3]);
  }

  exit();
}
