/* Self tests of the prologue_value for GDB, the GNU debugger.

   Copyright (C) 2024-2024 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "defs.h"
#include "gdbsupport/selftest.h"
#include "prologue-value.h"

namespace selftests {

static void
pv_subtract_test (void)
{
  pv_t reg13_100 = pv_register (13, 100);
  pv_t reg17_20 = pv_register (17, 20);
  pv_t reg13_1 = pv_register (13, 1);
  pv_t constant10 = pv_constant (10);
  pv_t constant24 = pv_constant (24);
  pv_t unknown = pv_unknown ();
  // 24 - 10 == 14
  pv_t constant14 = pv_subtract (constant24, constant10);
  SELF_CHECK (pv_is_constant (constant14));
  SELF_CHECK (constant14.k == 14);
  // (reg13 + 100) - 10 == reg13 + 90
  SELF_CHECK (pv_is_register_k (pv_subtract (reg13_100, constant10), 13, 90));
  // (reg13 + 100) - (reg13 + 1) == 99
  pv_t constant99 = pv_subtract (reg13_100, reg13_1);
  SELF_CHECK (pv_is_constant (constant99));
  SELF_CHECK (constant99.k == 99);
  // (reg17 + 20) - (reg13 + 100) is unknown
  SELF_CHECK (pv_subtract (reg17_20, reg13_100).kind == pvk_unknown);
  // formerly buggy:
  // 10 - (reg13 + 100) is unknown, because a negative register is not
  // representable
  SELF_CHECK (pv_subtract (constant10, reg13_100).kind == pvk_unknown);

  // just for completeness, the unknown cases
  SELF_CHECK (pv_subtract (constant10, unknown).kind == pvk_unknown);
  SELF_CHECK (pv_subtract (unknown, constant10).kind == pvk_unknown);
  SELF_CHECK (pv_subtract (reg13_1, unknown).kind == pvk_unknown);
  SELF_CHECK (pv_subtract (unknown, reg13_1).kind == pvk_unknown);
  SELF_CHECK (pv_subtract (unknown, unknown).kind == pvk_unknown);
}

} /* namespace selftests */

void _initialize_prologue_value_selftests ();
void
_initialize_prologue_value_selftests ()
{
  selftests::register_test ("prologue_value", selftests::pv_subtract_test);
}
