/* 
 * Gowtham Kudupudi 26/03/2013
 * MIT License
 */
                                                                               
#ifndef MYSTDLIB_H
#define MYSTDLIB_H
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <list>
#include <map>
#include <iostream>
#include <fstream>

#if ! defined(__mode_t)
#  if defined(mode_t)
typedef mode_t __mode_t;
#  endif
#endif

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#if defined(__linux__)
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
std::string get_command_line(pid_t pid);
int poke(std::string ip);
int getIp();
std::string GetPrimaryIp();
std::string get_fd_contents(int fd);
char const * sperm(__mode_t mode);

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
	spawn (std::string command, bool daemon = false, 
      void (*onStopHandler)(spawn*) = NULL, bool freeChild = false,
      bool block = false
   );
	int getChildExitStatus();
	int pkill(int signal = SIGTERM);
};
extern std::map<pid_t, spawn*> processMap;

#endif /* __linux__ */
struct cfout_ : std::ofstream {
   cfout_ (const std::string& fileName) : std::ofstream(fileName) {};
};

template <typename Input_>
cfout_& operator<<(cfout_& strm, const Input_& var) {
    std::cout << var;
    static_cast<std::ofstream&>(strm) << var;
    return strm;
};
#endif /* MYSTDLIB_H */

