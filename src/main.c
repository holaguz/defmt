#include <stdlib.h>
#include "defmt.h"
#include <time.h>
#include <unistd.h>

int main(void) {
  int a = 0x12345678, b = 0xABCDEF12;
  char c = 69;

  defmt_printf("defmt_2_ints %d %d\n", a, b);
  defmt_printf("defmt_1_char %c\n", c);

  while(1) {
    time_t secs = time(NULL);
    defmt_printf("time: %d\n", secs);
    sleep(1);
  }

  return 0;
}
