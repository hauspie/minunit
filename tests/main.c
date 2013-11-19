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
#include "../minunit.h"



/* A sample test suite that should pass (test the setup and teardown function)*/
static int value;

MU_SETUP(sampleTest)
{
    value = 0;
    return 0;
}

MU_TEAR_DOWN(sampleTest)
{
    MU_ASSERT_NOT_EQUAL(value,0);
    return 0;
}

MU_TEST(sampleTest,test1)
{
    MU_ASSERT_EQUAL(value,0);
    value = 1;
    return 0;
}

MU_TEST(sampleTest,test2)
{
    MU_ASSERT_EQUAL(value,0);
    value = 2;
    return 0;
}

MU_TEST_SUITE(sampleTest) = {
    MU_ADD_TEST(sampleTest,test1),
    MU_ADD_TEST(sampleTest,test2),
    MU_TEST_SUITE_END
};

/* A test suite that will pass 2 tests out of 4 */
MU_SETUP(half_success) {return 0;}
MU_TEAR_DOWN(half_success) {return 0;}
MU_TEST(half_success, success1) { MU_ASSERT_EQUAL(0, 0); return 0;}
MU_TEST(half_success, success2) { MU_ASSERT_NOT_EQUAL(1, 0); return 0;}
MU_TEST(half_success, fail1) { MU_ASSERT_EQUAL(0,1); return 0;}
MU_TEST(half_success, fail2) { MU_ASSERT_NOT_EQUAL(0,0); return 0;}
MU_TEST_SUITE(half_success) = {
    MU_ADD_TEST(half_success, success1),
    MU_ADD_TEST(half_success, success2),
    MU_ADD_TEST(half_success, fail1),
    MU_ADD_TEST(half_success, fail2),
    MU_TEST_SUITE_END
};


/* A test suite that test that previous suite gets 2 succes out of 4 tests */
MU_SETUP(test_half) {return 0;}
MU_TEAR_DOWN(test_half) {return 0;}
MU_TEST(test_half, run) {
    int success, total;
    /* Assert that the test suite fails (i.e. at least one test fail */
    MU_ASSERT_EQUAL(MU_RUN_TEST_SUITE(half_success, &success, &total), -1);
    MU_ASSERT_EQUAL(success, 2);
    MU_ASSERT_EQUAL(total, 4);
    return 0;
}
MU_TEST_SUITE(test_half) = {
    MU_ADD_TEST(test_half, run),
    MU_TEST_SUITE_END
};

/* A test suite that test all assertion operators */
MU_SETUP(test_assertions) {return 0;}
MU_TEAR_DOWN(test_assertions) {return 0;}
MU_TEST(test_assertions, assert) {
    MU_ASSERT( 0 == 0);
    return 0;
}

MU_TEST(test_assertions, equal) {
    MU_ASSERT_EQUAL(0,0);
    return 0;
}

MU_TEST(test_assertions, notequal) {
    MU_ASSERT_NOT_EQUAL(0,1);
    return 0;
}

MU_TEST_SUITE(test_assertions) = {
    MU_ADD_TEST(test_assertions, assert),
    MU_ADD_TEST(test_assertions, equal),
    MU_ADD_TEST(test_assertions, notequal),
    MU_TEST_SUITE_END
};


int main(void)
{
    MU_RUN_TEST_SUITE_WITH_REPORT(sampleTest);
    MU_RUN_TEST_SUITE_WITH_REPORT(test_half);
    MU_RUN_TEST_SUITE_WITH_REPORT(test_assertions);
    return 0;
}
