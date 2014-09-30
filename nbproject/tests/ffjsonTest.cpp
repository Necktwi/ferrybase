/* 
 * File:   ffjsonTest.cpp
 * Author: Gowtham
 *
 * Created on Sep 22, 2014, 2:12:12 PM
 */

#include <stdlib.h>
#include <iostream>
#include "FFJSON.h"
#include "logger.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

/*
 * Simple C++ Test Suite
 */

using namespace std;

int ff_log_type = FFL_DEBUG | FFL_INFO;
unsigned int ff_log_level = 1;
int child_exit_status = 0;

void test1() {
    std::cout << "ffjsonTest test 1" << std::endl;
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof (cCurrentPath))) {
        return;
    }

    cCurrentPath[sizeof (cCurrentPath) - 1] = '\0'; /* not really required */
    ffl_info(1, "The current working directory is %s", cCurrentPath);
    fflush(stdout);
    //string fn = "sample.ffjson";
    string fn = "/home/gowtham/Projects/ferrymediaserver/output.ffjson";
    ifstream ifs(fn.c_str(), ios::in | ios::ate);
    string ffjsonStr;
    ifs.seekg(0, std::ios::end);
    ffjsonStr.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ffjsonStr.assign((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
    FFJSON ffo(ffjsonStr);
    std::string ps = ffo.stringify();
    //cout << ps << endl;
    FFJSON ffo2(ps);
    //ffo2["amphibians"]["genome"].base64encode = true;
    std::string ps2 = ffo2.prettyString();
    //cout << ps2 << endl;
    std::cout << ffo2["framesizes"].prettyString() << endl;
    std::cout << ffo2["ferryframes"].prettyString() << endl;
    return;
}

void test2() {
    std::cout << "ffjsonTest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (ffjsonTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% ffjsonTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (ffjsonTest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (ffjsonTest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (ffjsonTest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (ffjsonTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

