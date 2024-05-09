#ifndef PRINTSTATE_H_
#define PRINTSTATE_H_

#ifdef LADYBUG_PRINT_STATE

#include "ladybug.h"
#include "word.h"

void printword(const char* text, const uint64_t x);
void printstate(const char* text, const ladybug_state_t* s);

#else

#define printword(text, w) \
  do {                     \
  } while (0)

#define printstate(text, s) \
  do {                      \
  } while (0)

#endif

#endif /* PRINTSTATE_H_ */
