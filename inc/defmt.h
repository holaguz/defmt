// File: defmt.h
// AC, 2024

#ifndef DEFMT_H_
#define DEFMT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFMT_PRINT_MAGIC
#define DEFMT_PRINT_MAGIC "\01\02\03\04 "
#endif

#define DEFMT_CAT_(a, b) a##b
#define DEFMT_MAKE_VAR_ID_(line) DEFMT_CAT_(defmt_id, line)
#define DEFMT_MAKE_FMT_ID_(line) DEFMT_CAT_(defmt_fmt, line)

#define DEFMT_SECT_FMT __attribute((section("defmt_fmt")))
#define DEFMT_SECT_ID __attribute((section("defmt_id")))

#include <unistd.h>
extern size_t defmt_comptime_hash(void *const data, size_t n);

#define defmt_print_id_(id) printf(DEFMT_PRINT_MAGIC "%zu", id)

#define STRINGIZE(arg) STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2) CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2) CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2) arg1##arg2

#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_1(what, __VA_ARGS__);
#define FOR_EACH_3(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_2(what, __VA_ARGS__);
#define FOR_EACH_4(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_3(what, __VA_ARGS__);
#define FOR_EACH_5(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_4(what, __VA_ARGS__);
#define FOR_EACH_6(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_5(what, __VA_ARGS__);
#define FOR_EACH_7(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_6(what, __VA_ARGS__);
#define FOR_EACH_8(what, x, ...)                                               \
  what(x);                                                                     \
  FOR_EACH_7(what, __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...)                                             \
  CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...)                                                 \
  FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, x, __VA_ARGS__)

#include <unistd.h>

#define WRITE(field) write(0, &(field), sizeof(typeof(field)));

#define defmt_printf(fmt, ...)                                                 \
  do {                                                                         \
    __attribute__((unused))                                                    \
    DEFMT_SECT_FMT static const char DEFMT_MAKE_FMT_ID_(__LINE__)[] = fmt;     \
    DEFMT_SECT_ID static size_t DEFMT_MAKE_VAR_ID_(__LINE__);                  \
    FOR_EACH(WRITE, DEFMT_MAKE_VAR_ID_(__LINE__), __VA_ARGS__);                \
  } while (0)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // DEFMT_H_
