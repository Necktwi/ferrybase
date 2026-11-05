#ifndef THRDPL
#define THRDPL

#include <cstddef>
#include <functional>
#include <thread>
#include <queue>
#include <shared_mutex>
#include <condition_variable>

using namespace std;

class ThreadPool {
public:
	ThreadPool () {};
	ThreadPool (size_t n);
	~ThreadPool ();
	void init (size_t n);
	void enqueue(function<void(int)> job);
	void join ();
	void printThrdStats ();

private:
	vector<thread> workers_;
	vector<char> isRunning_;
	queue<function<void(int)>> jobs_;
	mutex mutex_;
	condition_variable cv_;
	condition_variable cvJoin_;
	bool stopping_ = false;
	bool started;
	atomic<int> jc = {0};
	void start (size_t n);
	void stop ();
};
extern thread_local int tid;
extern ThreadPool* tpoolPtr;

#endif //THRDPL
