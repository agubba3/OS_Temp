#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  char buf[1024];
  for (int i = 0; i < sizeof(buf); i ++)
    buf[i] = i;

  printf("/proc/%d/pagemap, buf=%p\n", getpid(), buf);
  getchar();

  return 0;
}
