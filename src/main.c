#include <stdlib.h>
#include "defmt.h"
#include <stdarg.h>

int main(void) {
  int a = 69, b = 32;
  char c = 11;

  defmt_printf("defmt_2_ints %d %d", a, b);
  defmt_printf("defmt_1_char %c", c);

  return 0;
}
