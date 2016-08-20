/* 
 * File:   mystdlib.h
 * Author: Satya Gowtham Kudupudi
 *
 * Created on 26 March, 2013, 10:25 AM
 */

#ifndef MYSTDLIB_H
#define MYSTDLIB_H
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <list>
#include <map>

#if defined(__APPLE__) || defined(__CYGWIN__)
#if defined(__MACH__) || defined(__CYGWIN__)
typedef mode_t __mode_t;
#endif
#endif

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);
int copyfile(std::string src, std::string dst);
std::string getCurrentDir();
std::string getMachineName();
int rmmydir(std::string dirn);
std::string inputPass();
std::string inputText();
std::string getStdoutFromCommand(std::string cmd);
std::string getTime();
std::string getuTime();
std::string get_command_line(pid_t pid);
int poke(std::string ip);
int getIp();
std::string GetPrimaryIp();
std::string get_fd_contents(int fd);
char const * sperm(__mode_t mode);
timespec UTimeDiff(timespec& tsEnd, timespec& tsStart);

/*Corrected on system time change*/
class FerryTimeStamp : timespec {
public:
	FerryTimeStamp();
	~FerryTimeStamp();
	FerryTimeStamp& operator=(time_t t);
	operator time_t();
	bool operator<(const FerryTimeStamp competer);
	FerryTimeStamp operator+(FerryTimeStamp ftsAddand);
	FerryTimeStamp operator-(FerryTimeStamp ftsSubtrahend);
	static timespec sub(timespec a, timespec b);
	static timespec add(timespec a, timespec b);
	static std::list<time_t*> ferryTimesList;
};

extern int child_exit_status;

class spawn {
private:
	int cpstdinp[2];
	int cpstdoutp[2];
	int cpstderrp[2];

public:
	static bool processCleaned;
	static void defaultOnStopHandler(spawn* process);
	pid_t cpid = 0;
	int cpstdin = -1;
	int cpstdout = -1;
	int cpstderr = -1;
	int childExitStatus = 0;
	std::string cmd = "";
	std::string cmdName = "";
	void (*onStopHandler)(spawn*);

	spawn();
	spawn(std::string command, bool daemon = false, void (*onStopHandler)(spawn*) = NULL, bool freeChild = false, bool block = false);
	int getChildExitStatus();
	int pkill(int signal = SIGTERM);
};
extern std::map<pid_t, spawn*> processMap;

/**
 * returns a string of base64 encoded data
 * @param data:input string
 * @param input_length: the length of the input string
 * @param output_length: pointer to the variable that gets length of the output
 * string.
 * @return pointer to the string encoded in base65 and should be freed using 
 * free() by the calling function.
 */
char* base64_encode(const unsigned char *data,
		size_t input_length,
		size_t *output_length);

/**
 * @param data : input base64 encoded string
 * @param input_length : length of the @param data
 * @param output_length : pointer to the variable that gets filled with length 
 * returned binary value.
 * @return binary block; should be freed by calling function using free().
 */
unsigned char* base64_decode(const char *data,
		size_t input_length,
		size_t *output_length);
void base64_cleanup();
void build_decoding_table();
#endif /* MYSTDLIB_H */

