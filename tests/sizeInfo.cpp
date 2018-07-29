/* 
 * File:   sizeInfo.cpp
 * Author: Gowtham
 *
 * Created on Dec 6, 2014, 11:47:08 PM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */


#include "logger.h"
#include <iostream>

using namespace std;

int ff_log_type = FFL_DEBUG | FFL_INFO;
unsigned int ff_log_level = 1;
int child_exit_status = 0;

void test1() {
	std::cout << "sizeInfo test 1" << std::endl;

	std::cout << "size of char: " << sizeof (char) << std::endl;
	std::cout << "size of short: " << sizeof (short) << std::endl;
	std::cout << "size of int: " << sizeof (int) << std::endl;
	std::cout << "size of long: " << sizeof (long) << std::endl;
	std::cout << "size of long long: " << sizeof (long long) << std::endl;

	std::cout << "size of float: " << sizeof (float) << std::endl;
	std::cout << "size of double: " << sizeof (double) << std::endl;

	std::cout << "size of pointer: " << sizeof (int *) << std::endl;
}

void test2() {
	std::cout << "sizeInfo test 2" << std::endl;
	std::cout << "%TEST_FAILED% time=0 testname=test2 (sizeInfo) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << "%SUITE_STARTING% sizeInfo" << std::endl;
	std::cout << "%SUITE_STARTED%" << std::endl;

	std::cout << "%TEST_STARTED% test1 (sizeInfo)" << std::endl;
	test1();
	std::cout << "%TEST_FINISHED% time=0 test1 (sizeInfo)" << std::endl;

	std::cout << "%TEST_STARTED% test2 (sizeInfo)\n" << std::endl;
	test2();
	std::cout << "%TEST_FINISHED% time=0 test2 (sizeInfo)" << std::endl;

	std::cout << "%SUITE_FINISHED% time=0" << std::endl;

	return (EXIT_SUCCESS);
}

