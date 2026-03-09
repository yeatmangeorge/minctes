#include "minctes.h"

#define PERSISTANT_VARIABLE_RESET false
#define PERSISTANT_VARIABLE_ALTERED true

static bool persistant_variable = PERSISTANT_VARIABLE_RESET;

static void setup() { persistant_variable = PERSISTANT_VARIABLE_ALTERED; }
static void teardown(void) { persistant_variable = PERSISTANT_VARIABLE_RESET; }

MINCTES(setup_teardown, setup(), teardown()) {
  MINCTES_EQUALS(persistant_variable, PERSISTANT_VARIABLE_ALTERED, "%b");
}

MINCTES(no_setup_teardown, minctes_no_setup(), minctes_no_teardown()) {
  MINCTES_EQUALS(persistant_variable, PERSISTANT_VARIABLE_RESET, "%b");
}