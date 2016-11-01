#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

double now()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int count = 0;

void* run(int cnt)
{
  double beg = now();
  for (register int i = 0; i < cnt; i++) {
    asm volatile("incl %0"
                 : "+m"(count)
                 : "m"(count)
                 : "memory");
  }
  double end = now();

  printf("%d: %.2f sec\n", getpid(), end - beg);

  return NULL;
}

int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("[usage] %s [#proc] [count] [cmd]\n", argv[0]);
    exit(1);
  }

  if (getuid() != 0) {
    printf("need to run as a root\n");
    exit(2);
  }

  int ncpu = atoi(argv[1]);
  int upto = atoi(argv[2]);

  // invoke n processes
  int *child = malloc(sizeof(int) * ncpu);
  for (int i = 0; i < ncpu; i ++) {
    if ((child[i] = fork()) == 0) {
      printf("%d: runs\n", getpid());
      run(upto);
      exit(0);
    }
  }

  // run a command over child pids
  for (int i = 3; i < argc; i ++) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "%s %d", argv[i], child[i-3]);
    printf("execute: %s\n", cmd);
    system(cmd);
  }

  int status;
  for (int i = 0; i < ncpu; i ++)
    wait(&status);

  return 0;
}
