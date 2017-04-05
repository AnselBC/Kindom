//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef PROJECT_KING_HRTIME_H
#define PROJECT_KING_HRTIME_H

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#define khrtime int64_t

//////////////////////////////////////////////////////////////////////////////
//
//      Factors to multiply units by to obtain coresponding king_hrtime values.
//
//////////////////////////////////////////////////////////////////////////////

#define HRTIME_FOREVER (10 * HRTIME_DECADE)
#define HRTIME_DECADE (10 * HRTIME_YEAR)
#define HRTIME_YEAR (365 * HRTIME_DAY + HRTIME_DAY / 4)
#define HRTIME_WEEK (7 * HRTIME_DAY)
#define HRTIME_DAY (24 * HRTIME_HOUR)
#define HRTIME_HOUR (60 * HRTIME_MINUTE)
#define HRTIME_MINUTE (60 * HRTIME_SECOND)
#define HRTIME_SECOND (1000 * HRTIME_MSECOND)
#define HRTIME_MSECOND (1000 * HRTIME_USECOND)
#define HRTIME_USECOND (1000 * HRTIME_NSECOND)
#define HRTIME_NSECOND (static_cast<khrtime>(1))

#define HRTIME_APPROX_SECONDS(_x) ((_x) >> 30) // off by 7.3%
#define HRTIME_APPROX_FACTOR (((float)(1 << 30)) / (((float)HRTIME_SECOND)))

//////////////////////////////////////////////////////////////////////////////
//
//      Map from units to king_hrtime values
//
//////////////////////////////////////////////////////////////////////////////

// simple macros

#define HRTIME_YEARS(_x) ((_x)*HRTIME_YEAR)
#define HRTIME_WEEKS(_x) ((_x)*HRTIME_WEEK)
#define HRTIME_DAYS(_x) ((_x)*HRTIME_DAY)
#define HRTIME_HOURS(_x) ((_x)*HRTIME_HOUR)
#define HRTIME_MINUTES(_x) ((_x)*HRTIME_MINUTE)
#define HRTIME_SECONDS(_x) ((_x)*HRTIME_SECOND)
#define HRTIME_MSECONDS(_x) ((_x)*HRTIME_MSECOND)
#define HRTIME_USECONDS(_x) ((_x)*HRTIME_USECOND)
#define HRTIME_NSECONDS(_x) ((_x)*HRTIME_NSECOND)

// gratuituous wrappers

static inline khrtime
khrtime_from_years(unsigned int years)
{
  return (HRTIME_YEARS(years));
}

static inline khrtime
khrtime_from_weeks(unsigned int weeks)
{
  return (HRTIME_WEEKS(weeks));
}

static inline khrtime
khrtime_from_days(unsigned int days)
{
  return (HRTIME_DAYS(days));
}

static inline khrtime
khrtime_from_mins(unsigned int mins)
{
  return (HRTIME_MINUTES(mins));
}

static inline khrtime
khrtime_from_sec(unsigned int sec)
{
  return (HRTIME_SECONDS(sec));
}

static inline khrtime
khrtime_from_msec(unsigned int msec)
{
  return (HRTIME_MSECONDS(msec));
}

static inline khrtime
khrtime_from_usec(unsigned int usec)
{
  return (HRTIME_USECONDS(usec));
}

static inline khrtime
khrtime_from_nsec(unsigned int nsec)
{
  return (HRTIME_NSECONDS(nsec));
}

static inline khrtime
khrtime_from_timespec(const struct timespec *ts)
{
  return khrtime_from_sec(ts->tv_sec) + khrtime_from_nsec(ts->tv_nsec);
}

static inline khrtime
khrtime_from_timeval(const struct timeval *tv)
{
  return khrtime_from_sec(tv->tv_sec) + khrtime_from_usec(tv->tv_usec);
}

//////////////////////////////////////////////////////////////////////////////
//
//      Map from khrtime values to other units
//
//////////////////////////////////////////////////////////////////////////////

static inline khrtime
khrtime_to_years(khrtime t)
{
  return ((khrtime)(t / HRTIME_YEAR));
}

static inline khrtime
khrtime_to_weeks(khrtime t)
{
  return ((khrtime)(t / HRTIME_WEEK));
}

static inline khrtime
khrtime_to_days(khrtime t)
{
  return ((khrtime)(t / HRTIME_DAY));
}

static inline khrtime
khrtime_to_mins(khrtime t)
{
  return ((khrtime)(t / HRTIME_MINUTE));
}

static inline khrtime
khrtime_to_sec(khrtime t)
{
  return ((khrtime)(t / HRTIME_SECOND));
}

static inline khrtime
khrtime_to_msec(khrtime t)
{
  return ((khrtime)(t / HRTIME_MSECOND));
}

static inline khrtime
khrtime_to_usec(khrtime t)
{
  return ((khrtime)(t / HRTIME_USECOND));
}

static inline khrtime
khrtime_to_nsec(khrtime t)
{
  return ((khrtime)(t / HRTIME_NSECOND));
}

static inline struct timespec
khrtime_to_timespec(khrtime t)
{
  struct timespec ts;

  ts.tv_sec  = khrtime_to_sec(t);
  ts.tv_nsec = t % HRTIME_SECOND;
  return (ts);
}

static inline struct timeval
khrtime_to_timeval(khrtime t)
{
  int64_t usecs;
  struct timeval tv;

  usecs      = khrtime_to_usec(t);
  tv.tv_sec  = usecs / 1000000;
  tv.tv_usec = usecs % 1000000;
  return (tv);
}

/*
   using Jan 1 1970 as the base year, instead of Jan 1 1601,
   which translates to (365 + 0.25)369*24*60*60 seconds   */
#define NT_TIMEBASE_DIFFERENCE_100NSECS 116444736000000000i64

static inline khrtime
kget_hrtime_internal()
{
#if defined(freebsd) || HAVE_CLOCK_GETTIME
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return khrtime_from_timespec(&ts);
#else
  timeval tv;
  gettimeofday(&tv, nullptr);
  return khrtime_from_timeval(&tv);
#endif
}

static inline struct timeval
kgettimeofday()
{
  return khrtime_to_timeval(kget_hrtime_internal());
}

static inline int
ktime()
{
  return (int)khrtime_to_sec(kget_hrtime_internal());
}

static inline int
khrtime_diff_msec(khrtime t1, khrtime t2)
{
  return (int)khrtime_to_msec(t1 - t2);
}

static inline khrtime
khrtime_diff(khrtime t1, khrtime t2)
{
  return (t1 - t2);
}

static inline khrtime
khrtime_add(khrtime t1, khrtime t2)
{
  return (t1 + t2);
}

static inline void
khrtime_sleep(khrtime delay)
{
  struct timespec ts = khrtime_to_timespec(delay);
  nanosleep(&ts, nullptr);
}

#endif // PROJECT_KING_HRTIME_H
