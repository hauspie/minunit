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

/* Output macro */

/* Define these before including minunit.h to use something else than printf */
#ifndef MU_PRINT_STR
#include <stdio.h>
#define MU_PRINT_STR(str) printf("%s", (str))
#define MU_PRINT_INT(val) printf("%d", (val))
#define MU_PRINT_CHAR(ch) printf("%c", (ch))
#endif

/* Define this macro to enable verbose assert */
#define MU_ASSERT_USE_PRINT 

#ifndef NULL
#define NULL ((void*)0)
#endif

/* Test declaration macro */
#define MAX_REPORTED_FAILED (5)
#define INIT_FAILED 0,NULL,{-1,-1,-1,-1,-1}
/* 0,NULL,-1,-1,-1,-1,-1    */
/* Type for test suite arrays */
    struct mu_test_desc;
    typedef void (*mu_test_func)(struct mu_test_desc *desc);
    struct mu_test_desc
    {
	mu_test_func test;
	const char *test_name;
	int success;
	int performed;
	/* space to record failed asserts */
	int failed ;
    const char *filename ;
	int detail[MAX_REPORTED_FAILED] ;
    };

#define MU_SETUP(test_suite) void test_suite##_setup(struct mu_test_desc *desc)
#define MU_TEAR_DOWN(test_suite) void test_suite##_tear_down(struct mu_test_desc *desc)
#define MU_TEST(test_suite, test_name) void test_suite##_##test_name(struct mu_test_desc *desc)
#define MU_TEST_SUITE(test_suite) static struct mu_test_desc test_suite##_tests_array[]

/* Convert non string litteral to string using preprocessor */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
    
#define MU_ADD_TEST(test_suite, test_name) {test_suite##_##test_name, #test_name "(" __FILE__ ":" STR(__LINE__) ")", 0, 0, INIT_FAILED}
#define MU_TEST_SUITE_END {NULL,NULL,0,0,INIT_FAILED}
#define MU_DESC_SUCCESS(d) ((d)->performed != 0 && ((d)->success == (d)->performed))

    static int mu_run_test_suite(mu_test_func setup, mu_test_func tear_down, struct mu_test_desc *tests_array, int *out_success, int *out_total)
    {
	int i;
	int success = 0;
	for (i = 0 ; tests_array[i].test != NULL ; ++i)
	{
	    struct mu_test_desc *desc = &tests_array[i];
	    /* These needs to be 0 before running the test.	*/
	    desc->success = desc->performed = desc->failed = 0;
	    setup(desc);
	    desc->test(desc);
	    tear_down(desc);
	    if (MU_DESC_SUCCESS(desc))
		success++;
	}
	if (out_total)
	    *out_total = i;
	if (out_success)
	    *out_success = success;
	if (i == success) return 0;
	return -1;
    }

    static void mu_report_test_suite_report(const char *suite_name, struct mu_test_desc *tests_array, int success, int total)
    {
	MU_PRINT_STR("Suite ");
	MU_PRINT_STR(suite_name);
	MU_PRINT_STR(": ");
	MU_PRINT_INT(success);
	MU_PRINT_CHAR('/');
	MU_PRINT_INT(total);
	MU_PRINT_CHAR('\n');
	if (success != total)
	{
	    int i;
	    MU_PRINT_STR("\tFailing tests:\n");
	    for (i = 0 ; tests_array[i].test != NULL ; ++i)
	    {
		struct mu_test_desc *desc = &tests_array[i];
		if (!MU_DESC_SUCCESS(desc))
		{
		    MU_PRINT_STR("\t\t");
		    MU_PRINT_STR(desc->test_name);
		    MU_PRINT_STR(": ");
		    if (desc->performed != 0)
		    {
			MU_PRINT_INT(desc->success);
			MU_PRINT_CHAR('/');
			MU_PRINT_INT(desc->performed);
			MU_PRINT_STR(" passed");
#ifdef MU_ASSERT_USE_PRINT
			{
				int j ;
				MU_PRINT_STR(" [") ;
				MU_PRINT_STR(desc->filename) ;
				MU_PRINT_STR(" : ") ;
				for (j = 0 ; j < desc->failed ; j++) 
	                        {
	                            if (j < MAX_REPORTED_FAILED) 
	                            {
	                                    MU_PRINT_INT(desc->detail[j]) ;
	                                    if (j != desc->failed-1) 
	                                    {
	                                        MU_PRINT_STR(", ") ;
	                                    }
	                            } 
	                            else 
	                            {
	                            	MU_PRINT_STR(".") ;
	                            }
			}
			MU_PRINT_STR(" ]") ;
			}
#endif
		    MU_PRINT_STR("\n") ;
		    }
		    else
		    {
			MU_PRINT_STR("no test performed\n");
		    }
		}
	    }
	}
    }

#define MU_RUN_TEST_SUITE(test_suite, success, total) mu_run_test_suite(test_suite##_setup, test_suite##_tear_down, test_suite##_tests_array, (success), (total))
#define MU_RUN_TEST_SUITE_WITH_REPORT(test_suite) do {			\
	int success = 0;						\
	int total = 0;							\
	MU_RUN_TEST_SUITE(test_suite, &success, &total);		\
	mu_report_test_suite_report(#test_suite, test_suite##_tests_array, success, total); \
    } while(0)

#define MU_RECORD(file, line) do { \
    if (desc->failed < MAX_REPORTED_FAILED) {   \
        desc->detail[desc->failed] = line ; \
        desc->filename = file ; \
    }   \
} while (0)

/* Assertion macro */
#define MU_ASSERT_NOPRINT(test) do {			\
	if ( (test) )					\
	    desc->success++;				\
    desc->performed++;					\
    } while (0)						\

#define MU_ASSERT_PRINT(test,file,line) do {		\
	if ( (test) )					\
	    desc->success++;				\
	else {						\
	    MU_RECORD(file, line) ;	\
        desc->failed++ ;    \
	}						\
	desc->performed++;				\
    } while (0)

#ifdef MU_ASSERT_USE_PRINT
#define MU_ASSERT(a) MU_ASSERT_PRINT((a), __FILE__, __LINE__)
#else
#define MU_ASSERT(a) MU_ASSERT_NOPRINT((a))
#endif

#define MU_ASSERT_EQUAL(val_to_test,expected) MU_ASSERT((val_to_test) == (expected))
#define MU_ASSERT_NOT_EQUAL(val_to_test,expected) MU_ASSERT((val_to_test) != (expected))
#define MU_ASSERT_LESS(val_to_test, expected) MU_ASSERT((val_to_test) < (expected))
#define MU_ASSERT_GREATER(val_to_test, expected) MU_ASSERT((val_to_test) > (expected))
#define MU_ASSERT_TRUE(val_to_test) MU_ASSERT((val_to_test))
#define MU_ASSERT_FALSE(val_to_test) MU_ASSERT(!(val_to_test))

#ifdef __cplusplus
} /* extern 'C' */
#endif

#endif
