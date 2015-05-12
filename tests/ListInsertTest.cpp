/* 
 * File:   ListInsertTest.cpp
 * Author: gowtham
 *
 * Created on 4 May, 2015, 2:09:30 PM
 */

#include <stdlib.h>
#include <iostream>
#include <list>

/*
 * Simple C++ Test Suite
 */

using namespace std;

//int ff_log_type = FFL_DEBUG | FFL_INFO;
//unsigned int ff_log_level = 1;
//int child_exit_status = 0;

void test1() {
	std::cout << "ListInsertTest test 1" << std::endl;
	list<string> ls;
	ls.push_back("gowtham");
	ls.push_back("satish");
	ls.push_back("sasi");
	string* pGowtham=&(*ls.begin());
	cout << &(*pGowtham) << std::endl;
	list<string>::iterator itGowtham=ls.begin();
	list<string>::iterator itSasi=itGowtham;
	itSasi++;itSasi++;
	ls.splice(itSasi,ls,itGowtham);
	cout << &(*pGowtham) << std::endl;
}

void test2() {
	std::cout << "ListInsertTest test 2" << std::endl;
	std::cout << "%TEST_FAILED% time=0 testname=test2 (ListInsertTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << "%SUITE_STARTING% ListInsertTest" << std::endl;
	std::cout << "%SUITE_STARTED%" << std::endl;

	std::cout << "%TEST_STARTED% test1 (ListInsertTest)" << std::endl;
	test1();
	std::cout << "%TEST_FINISHED% time=0 test1 (ListInsertTest)" << std::endl;

	std::cout << "%TEST_STARTED% test2 (ListInsertTest)\n" << std::endl;
	test2();
	std::cout << "%TEST_FINISHED% time=0 test2 (ListInsertTest)" << std::endl;

	std::cout << "%SUITE_FINISHED% time=0" << std::endl;

	return (EXIT_SUCCESS);
}

