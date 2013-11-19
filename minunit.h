/*
  Minunit is a very minimal library for performing unit tests in C.
  Copyright (C) 2013 Michaël Hauspie
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Test declaration macro */

#define MU_SETUP(test_suite) int test_suite##_setup(void)
#define MU_TEAR_DOWN(test_suite) int test_suite##_tear_down(void)
#define MU_TEST(test_suite, test_name) int test_suite##_##test_name(void)
#define MU_TEST_SUITE(test_suite)		\
    typedef int (*test_suite##_tests)(void);	\
    test_suite##_tests test_suite##_tests_array[]

#define MU_ADD_TEST(test_suite, test_name) test_suite##_##test_name
#define MU_TEST_SUITE_END ((void*)0)

#define MU_RUN_TEST_SUITE(test_suite)


/* Assertion macro */
#define MU_ASSERT(test) if ( !(test) ) return -1;
#define MU_ASSERT_EQUAL(val_to_test,reference) MU_ASSERT((val_to_test) == (reference))
#define MU_ASSERT_NOT_EQUAL(val_to_test,reference) MU_ASSERT((val_to_test) != (reference))


#ifdef __cplusplus
} /* extern 'C' */
#endif

#endif
