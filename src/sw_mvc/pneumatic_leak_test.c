/**
 * @file pneumatic_leak_test.c
 *
 * @copyright
 * COPYRIGHT 2025
 * DEKA RESEARCH AND DEVELOPMENT CORPORATION
 *
 * Contains confidential and proprietary information which
 * may not be copied, disclosed or used by others except as expressly
 * authorized in writing by DEKA Research & Development Corporation.
 *
 * @critical <Minor, Moderate, Major Single-Point, Major Multi-Point>
 * @brief <brief description of the file>
 * @subsystem <subsystem name>
 */

#include "pneumatic_leak_test.h"

G_DEFINE_BOXED_TYPE(PneumaticLeakTest, pneumatic_leak_test, pneumatic_leak_test_copy, pneumatic_leak_test_free)

PneumaticLeakTest *pneumatic_leak_test_new()
{
   PneumaticLeakTest *ret = g_new(PneumaticLeakTest, 1);
   ret->test_started = FALSE;
   ret->test_completed = FALSE;
   ret->test_passed = FALSE;
   return ret;
}

void pneumatic_leak_test_reset(PneumaticLeakTest *self)
{
   self->test_started = FALSE;
   self->test_completed = FALSE;
   self->test_passed = FALSE;
}

PneumaticLeakTest *pneumatic_leak_test_copy(PneumaticLeakTest *source)
{
   PneumaticLeakTest *ret = g_new(PneumaticLeakTest, 1);
   ret->test_started = source->test_started;
   ret->test_completed = source->test_completed;
   ret->test_passed = source->test_passed;
   return ret;
}

gboolean pneumatic_leak_test_equal(PneumaticLeakTest *left, PneumaticLeakTest *right)
{
   return ( (left->test_started == right->test_started) &&
            (left->test_completed == right->test_completed) &&
            (left->test_passed == right->test_passed) );
}

void pneumatic_leak_test_free(PneumaticLeakTest *self)
{
   g_free(self);
}