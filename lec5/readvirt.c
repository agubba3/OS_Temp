#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <err.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
    err(1, "[usage] %s [/proc/self/pagemap] [vaddr]", argv[0]);

  off_t vaddr = strtoul(argv[2], NULL, 16);
  size_t PAGE_SIZE = sysconf(_SC_PAGE_SIZE);

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    err(1, "failed to open pagemap");

  // Q?
  uint64_t off = ((uint64_t)vaddr / PAGE_SIZE) * sizeof(uint64_t);
  if (lseek(fd, off, SEEK_SET) != off)
    err(1, "failed to lseek");

  uint64_t pfn;
  if (read(fd, &pfn, sizeof(pfn)) != sizeof(pfn))
    err(1, "failed to read pfn");

  // Q?
  printf("0x%" PRIx64 ", pfn=%" PRIu64 "\n",
         pfn, pfn & 0x7FFFFFFFFFFFFF);

  close(fd);

  return EXIT_SUCCESS;
}
