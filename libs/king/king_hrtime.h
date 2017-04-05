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
//      Factors to multiply units by to obtain coresponding ink_hrtime values.
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
#define HRTIME_NSECOND (static_cast<ink_hrtime>(1))

#define HRTIME_APPROX_SECONDS(_x) ((_x) >> 30) // off by 7.3%
#define HRTIME_APPROX_FACTOR (((float)(1 << 30)) / (((float)HRTIME_SECOND)))

//////////////////////////////////////////////////////////////////////////////
//
//      Map from units to ink_hrtime values
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


#endif // PROJECT_KING_HRTIME_H
