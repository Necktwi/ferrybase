//
//  FerryTimeStamp.cpp
//  base
//
//  Created by SatyaGowthamKudupudi on 18/10/16.
//
//

#include "FerryTimeStamp.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>
#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
#endif
using namespace std;

FerryTimeStamp::FerryTimeStamp() {
    tv_sec = 0;
    tv_nsec = 0;
    ferryTimesList.push_back(static_cast<time_t*> (&tv_sec));
}

FerryTimeStamp::FerryTimeStamp(const string& sFTS) {
    assign(sFTS);
    ferryTimesList.push_back(static_cast<time_t*> (&tv_sec));
}

FerryTimeStamp& FerryTimeStamp::operator=(const string& sFTS) {
    assign(sFTS);
    ferryTimesList.push_back(static_cast<time_t*> (&tv_sec));
    return *this;
}

FerryTimeStamp::~FerryTimeStamp() {
    std::list<time_t*>::iterator i;
    i = ferryTimesList.begin();
    while (i != ferryTimesList.end()) {
        if (*i == static_cast<time_t*> (&tv_sec)) {
            i = ferryTimesList.erase(i);
            break;
        }
        i++;
    }
};

std::list<time_t*> FerryTimeStamp::ferryTimesList;

timespec FerryTimeStamp::sub(timespec a, timespec b) {
    timespec result = {0, 0};
    result.tv_sec = a.tv_sec - b.tv_sec;
    if (b.tv_nsec > a.tv_nsec) {
        if (a.tv_sec > b.tv_sec) {
            result.tv_sec++;
        } else {
            result.tv_sec--;
        }
        result.tv_nsec = b.tv_nsec - a.tv_nsec;
    } else {
        result.tv_nsec = a.tv_nsec - b.tv_nsec;
    }
    return result;
};

timespec FerryTimeStamp::add(timespec a, timespec b) {
    timespec result = {0, 0};
    result.tv_sec = a.tv_sec + b.tv_sec;
    result.tv_nsec = (a.tv_nsec + b.tv_nsec) % 1000000000;
    if (result.tv_nsec < a.tv_nsec || result.tv_nsec < b.tv_nsec) {
        result.tv_sec++;
    }
    return result;
}

FerryTimeStamp FerryTimeStamp::operator+(FerryTimeStamp ftsAddand) {
    FerryTimeStamp result;
    result.tv_sec = tv_sec + ftsAddand.tv_nsec;
    result.tv_nsec = (tv_nsec + ftsAddand.tv_nsec) % 1000000000;
    if (result.tv_nsec < tv_nsec || result.tv_nsec < ftsAddand.tv_nsec) {
        result.tv_sec++;
    }
    return result;
}

FerryTimeStamp FerryTimeStamp::operator-(FerryTimeStamp ftsSubtrahend) {
    FerryTimeStamp result;
    result.tv_sec = tv_sec - ftsSubtrahend.tv_sec;
    if (ftsSubtrahend.tv_nsec > tv_nsec) {
        if (tv_sec > ftsSubtrahend.tv_sec) {
            result.tv_sec++;
        } else {
            result.tv_sec--;
        }
        result.tv_nsec = ftsSubtrahend.tv_nsec - tv_nsec;
    } else {
        result.tv_nsec = tv_nsec - ftsSubtrahend.tv_nsec;
    }
    return result;
}

bool FerryTimeStamp::operator<(const FerryTimeStamp competer) {
    if (tv_sec < competer.tv_sec) {
        return true;
    } else if (tv_sec == competer.tv_sec) {
        if (tv_nsec < competer.tv_nsec) {
            return true;
        }
        return false;
    }
    return false;
}

FerryTimeStamp& FerryTimeStamp::operator=(time_t t) {
    tv_sec = t;
    return *this;
}

FerryTimeStamp::operator time_t() {
    return (time_t) tv_sec;
}

FerryTimeStamp::operator string() {
    return std::to_string(tv_sec) + "." + std::to_string(tv_nsec);
}

void FerryTimeStamp::Update() {
    clock_gettime(CLOCK_REALTIME, this);
}

void FerryTimeStamp::Clear() {
    tv_sec=0;
    tv_nsec=0;
}

void FerryTimeStamp::assign(const std::string& sTS) {
    size_t iPeriodNail = sTS.find('.');
    if (iPeriodNail == string::npos) return;
    tv_sec = stol(sTS.substr(0, iPeriodNail));
    tv_nsec = stol(sTS.substr(iPeriodNail + 1));
}

string FerryTimeStamp::GetTime() {
    struct tm ti;
    char tb[20];
    localtime_r(&tv_sec,&ti);
    strftime(tb, 20, "%Y-%m-%d %H:%M:%S", &ti);
    return std::string(tb);
}

string FerryTimeStamp::GetUTime() {
    char tb[20];
    char buf[23];
    struct tm ti;
    localtime_r(&tv_sec, &ti);
    strftime(tb, 20, "%d%b%H:%M:%S", &ti);
    sprintf(buf, "%s.%09ld", tb, tv_nsec);
    return std::string(buf);
}

ostream& operator<<(ostream& out, const FerryTimeStamp& f) {
    out << setfill('0') << setw(10) << f.tv_sec << '.' << left << setw(9) <<
    f.tv_nsec;
    return out;
}

FerryTimeStamp::DateFormat& operator<<(ostream& out, FerryTimeStamp::DateFormat& f) {
    f.pos=&out;
    return f;
}

ostream& operator<<(FerryTimeStamp::DateFormat& rDF, const FerryTimeStamp& f) {
    (*rDF.pos) << setfill('0') << setw(10) << f.tv_sec << '.' << left << setw(9) << f.tv_nsec;
    return (*rDF.pos);
}

// Windows clock_gettime(int, timespec* spec)
#if defined(_WIN32) || defined(_WIN64)
#define exp7           10000000i64     //1E+7     //C-file part
#define exp9         1000000000i64     //1E+9
#define w2ux 116444736000000000i64     //1.jan1601 to 1.jan1970
void unix_time(struct timespec *spec)
{
  __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
  wintime -= w2ux;  spec->tv_sec = wintime / exp7;
  spec->tv_nsec = wintime % exp7 * 100;
}
int clock_gettime(int, timespec *spec)
{
  static  struct timespec startspec; static double ticks2nano;
  static __int64 startticks, tps = 0;    __int64 tmp, curticks;
  QueryPerformanceFrequency((LARGE_INTEGER*)&tmp); //some strange system can
  if (tps != tmp) {
    tps = tmp; //init ~~ONCE         //possibly change freq ?
    QueryPerformanceCounter((LARGE_INTEGER*)&startticks);
    unix_time(&startspec); ticks2nano = (double)exp9 / tps;
  }
  QueryPerformanceCounter((LARGE_INTEGER*)&curticks); curticks -= startticks;
  spec->tv_sec = startspec.tv_sec + (curticks / tps);
  spec->tv_nsec = startspec.tv_nsec + (double)(curticks % tps) * ticks2nano;
  if (!(spec->tv_nsec < exp9)) { spec->tv_sec++; spec->tv_nsec -= exp9; }
  return 0;
}

struct tm* localtime_r(const time_t *clock, struct tm *result) {
  if (!clock || !result) return NULL;
  memcpy(result, localtime(clock), sizeof(*result));
  return result;
}
#endif