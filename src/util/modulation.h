#ifndef DHE_MODULES_UTIL_MODULATION_H
#define DHE_MODULES_UTIL_MODULATION_H

#include "interval.h"

namespace DHE {

static inline float modulated(float normalized_input, float cv) {
  return NORMAL.clamp(normalized_input + cv * 1e-1f);
}

}
#endif //DHE_MODULES_MODULATION_H
