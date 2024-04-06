#include <stdlib.h>
#include "defmt.h"
#include <stdarg.h>

int main(void) {
  int a = 0x12345678, b = 0xABCDEF12;
  char c = 69;

  defmt_printf("defmt_2_ints %d %d", a, b);
  defmt_printf("defmt_1_char %c", c);

  return 0;
}
