
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdint.h>

#define LETTERS "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ23456789"

int main(int argc, char *argv[]) {

  long int num_letters = strlen(LETTERS);
  
  int rnd_fd = open("/dev/urandom", O_RDONLY);

  char *passwd;

  size_t len = argc>1 ? strtol(argv[1],NULL,10) : 12;

  long int letterno;

  ssize_t bytes_read;
  const long int num_rnds = 4;
  uint64_t rnds[num_rnds];

  long int offset;
  
  passwd = malloc(len+1);
  if (passwd==NULL) {
    perror("malloc");
    return -1;
  }

  memset(passwd, 0, len+1);

  for (letterno = 0; letterno < len; letterno++) {

    bytes_read = read(rnd_fd, rnds, sizeof(uint64_t) * num_rnds);
    if (bytes_read != sizeof(uint64_t) * num_rnds) {
      perror("read");
      return -1;
    }

    offset = ((double) num_letters) * rnds[0] / 18446744073709551615.0;
    
    passwd[letterno] = LETTERS[offset];

  }

  write(1, passwd, len);
  
  return 0;

}
