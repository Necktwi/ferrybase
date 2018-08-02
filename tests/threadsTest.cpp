/* 
 * File:   threadsTest.cpp
 * Author: Gowtham
 *
 * Created on Nov 18, 2014, 3:30:35 PM
 */

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <list>
#include <mutex>
#include <condition_variable>

/*
 * Simple C++ Test Suite
 */

using namespace std;

class semaphore {
private:
	mutex mtx;
	condition_variable cv;
	int count;

public:

	semaphore(int count_ = 0) : count(count_) {
		;
	}

	void notify() {
		unique_lock<mutex> lck(mtx);
		++count;
		cv.notify_one();
	}

	void wait() {
		unique_lock<mutex> lck(mtx);

		while (count == 0) {
			cv.wait(lck);
		}
		count--;
	}
};

int child_exit_status = 0;
int ff_log_type = 0;
unsigned int ff_log_level = 0;

//void (&myFunc)(... args);

list<string*> goodsBus;
int busSize = 60;
semaphore busSemaphore;

void mynap(int i, thread** t, thread *wt) {
	int j = 100;
	while (j > 0) {
		cout << i << " sleeping " << j << endl;
		usleep(500000);
		j--;
	}
	if (wt)wt->join();
	cout << i << " exiting" << endl;
	delete *t;
}

void producer(int i, thread** t, thread* wt) {
	int j = 3 * busSize;
	while (j > 0) {
		if (goodsBus.size() >= busSize) {
			cout << i << " kicking out the " << **goodsBus.begin() << endl;
			delete *goodsBus.begin();
			goodsBus.pop_front();
		}
		cout << i << " boarding a good " << j << " on to bus" << endl;
		goodsBus.push_back(new string(string("Good ") + to_string(j) + " by " + to_string(i)));
		busSemaphore.notify();
		j--;
		sleep(1);
	}
	if (wt) {
		wt->join();
		delete wt;
	}
	cout << i << " exiting" << endl;
}

void consumer(int i, thread** t, thread* wt) {
	int j = 3 * busSize;
	busSemaphore.wait();
	list<string*>::iterator li = goodsBus.begin();
	list<string*>::iterator lii = li;
	busSemaphore.notify();
	while (j > 0) {
		busSemaphore.wait();
		while (true) {
			cout << i << " reading " << **li << endl;
			j--;
			busSemaphore.notify();
			sleep(1);
			lii = li;
			if (++lii == goodsBus.end()) {
				break;
			}
			++li;
		}
	}
	if (wt) {
		wt->join();
		delete wt;
	}
	cout << i << " exiting" << endl;
}

void test1() {
	std::cout << "threadTest test 1" << std::endl;
	int i = 5;
	thread* t = NULL;
	thread* tt = t;
	t = new thread(producer, 1, &t, tt);
	while (i > 0) {
		tt = t;
		t = new thread(consumer, i, &t, tt);
		//sleep(1);
		i--;
	}
	t->join();
	delete t;
	while (!goodsBus.empty()) {
		delete *goodsBus.begin();
		goodsBus.pop_front();
	}
	cout << "main exiting" << endl;
}

void test2() {
	std::cout << "threadTest test 2" << std::endl;
	std::cout << "%TEST_FAILED% time=0 testname=test2 (threadTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << "%SUITE_STARTING% threadTest" << std::endl;
	std::cout << "%SUITE_STARTED%" << std::endl;

	std::cout << "%TEST_STARTED% test1 (threadTest)" << std::endl;
	test1();
	std::cout << "%TEST_FINISHED% time=0 test1 (threadTest)" << std::endl;

	std::cout << "%TEST_STARTED% test2 (threadTest)\n" << std::endl;
	test2();
	std::cout << "%TEST_FINISHED% time=0 test2 (threadTest)" << std::endl;

	std::cout << "%SUITE_FINISHED% time=0" << std::endl;

	return (EXIT_SUCCESS);
}

