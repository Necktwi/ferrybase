//
//  FerryTimeStamp.h
//  base
//
//  Created by SatyaGowthamKudupudi on 18/10/16.
//
//

#ifndef FerryTimeStamp_h
#define FerryTimeStamp_h
#include <cstdlib>
#include <string>
#include <set>
#include <time.h>
#include <mutex>

/*Corrected on system time change*/
struct FerryTimeStamp : public timespec {
   FerryTimeStamp ();
   FerryTimeStamp (time_t sec, long nsec);
   FerryTimeStamp (const std::string& sTS);
   ~FerryTimeStamp ();
   FerryTimeStamp& operator= (time_t t);
   FerryTimeStamp& operator= (const std::string& sTS);
   void assign (const std::string& sTS);
   operator time_t ();
   //operator bool (); // causing ambiguity with <, donno y
   bool operator< (const FerryTimeStamp& competer) const;
   FerryTimeStamp operator+ (const FerryTimeStamp& ftsAddand);
   FerryTimeStamp operator- (const FerryTimeStamp& ftsSubtrahend);
   static timespec sub (const timespec a, const timespec b);
   static timespec add (const timespec a, const timespec b);
   static std::set<time_t*> ferryTimeSet;
   static std::mutex ftLsMtx;
   void update ();
   void clear ();
   operator std::string () const;
   std::string getTime () const;
   std::string getUTime () const;
   struct DateFormat{
      std::ostream* pos=NULL;
   };
};

std::ostream& operator<<(std::ostream& out, const FerryTimeStamp& f);
FerryTimeStamp::DateFormat& operator<<(std::ostream& out, const FerryTimeStamp::DateFormat& f);

#if defined(_WIN64) || defined(_WIN32)
#define CLOCK_REALTIME 1
int clock_gettime (int, timespec *spec);
inline struct tm* localtime_r (const time_t *clock, struct tm *result);
#endif
typedef FerryTimeStamp FTS_;
#endif /* FerryTimeStamp_h */
