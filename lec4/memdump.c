#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
    errx(1, "[usage] %s [mem-addr]", argv[0]);

  unsigned char *addr = (char *)strtol(argv[1], NULL, 16);
  for (int i = 0; i < 4; i ++) {
    printf("%016lx: ", (unsigned long)(addr + i*0x10));
    for (int j = 0; j < 0x10; j ++)
      printf("%02x ", *(addr + i*0x10 + j));
    printf("| ");
    for (int j = 0; j < 0x10; j ++) {
      char c = *(addr + i*0x10 + j);
      printf("%c", isprint(c) ? c : '.');
    }
    printf("\n");
  }
  return 0;
}
