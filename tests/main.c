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


static int value;

MU_SETUP(sampleTest)
{
    value = 0;
}

MU_TEAR_DOWN(sampleTest)
{
    MU_ASSERT_NOT_EQUAL(value,0);
}

MU_TEST(sampleTest,test1)
{
    MU_ASSERT_EQUAL(value,0);
    value = 1;
}

MU_TEST(sampleTest,test2)
{
    MU_ASSERT_EQUAL(value,0);
    value = 2;
}

MU_TEST_SUITE(sampleTest) = {
    MU_ADD_TEST(test1),
    MU_ADD_TEST(test2),
    MU_TEST_SUITE_END
};



int main(int argc, char **argv)
{
    mu_run_test_suite(sampleTest);
    return 0;
}
