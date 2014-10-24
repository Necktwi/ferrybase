/* 
 * File:   mycurl.cpp
 * Author: Gowtham
 *
 * Created on Oct 22, 2014, 9:14:10 PM
 */

#include <stdlib.h>
#include <iostream>
#include "mycurl.h"
#include "logger.h"

/*
 * Simple C++ Test Suite
 */

int ff_log_type = FFL_DEBUG | FFL_INFO;
unsigned int ff_log_level = 1;
int child_exit_status = 0;

void test1() {
	std::cout << "mycurl test 1" << std::endl;
	std::string content =
			"{UbuntuVM:{cameras:{video0:{state:\"Stopped\",newState:?}}}}";
	std::string res = HTTPReq("fairplay.ferryfair.com", "/", "17291", content);
	std::cout << res << std::endl;
}

void test2() {
	std::cout << "mycurl test 2" << std::endl;
	std::cout << "%TEST_FAILED% time=0 testname=test2 (mycurl) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << "%SUITE_STARTING% mycurl" << std::endl;
	std::cout << "%SUITE_STARTED%" << std::endl;

	std::cout << "%TEST_STARTED% test1 (mycurl)" << std::endl;
	test1();
	std::cout << "%TEST_FINISHED% time=0 test1 (mycurl)" << std::endl;

	std::cout << "%TEST_STARTED% test2 (mycurl)\n" << std::endl;
	test2();
	std::cout << "%TEST_FINISHED% time=0 test2 (mycurl)" << std::endl;

	std::cout << "%SUITE_FINISHED% time=0" << std::endl;

	return (EXIT_SUCCESS);
}

