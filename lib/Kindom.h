//
// Created by 宋辰伟 on 2017/4/16.
//

#ifndef PROJECT_KINDOM_H
#define PROJECT_KINDOM_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/param.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <arpa/nameser_compat.h>
#include <signal.h>
#include <wait.h>
#include <syslog.h>
#include <pwd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <values.h>
#include <alloca.h>
#include <errno.h>
#include <dirent.h>
#include <cpio.h>
#include <net/if.h>
#include <stropts.h>
#include <endian.h>
#include <sys/ioctl.h>
#include <resolv.h>
#include <sys/sysinfo.h>
#include <sys/sysctl.h>
#include <dlfcn.h>
#include <math.h>
#include <float.h>
#include <sys/sysmacros.h>
#include <sys/prctl.h>

// #include "List.h"

#define DEBUG 1

// mem interface
// #define kmalloc malloc
// #define kfree free

// Log interface
#define Debug(_f, ...) \
  do {                 \
  } while (0)

#define Warn(_f, ...) \
  do {                \
  } while (0)

#define Error(_f, ...) \
  do {                 \
  } while (0)

#define Fatal(_f, ...) \
  do {                 \
    assert(!(_f));     \
  } while (0)

#define kassert assert
#define krelease_assert assert

// Thread interface
#define MAX_THREAD_NAME 30
// typedef pthread_t kthread;
// typedef pthread_cond_t kcond;
// typedef pthread_key_t kthread_key;

// time interface
// typedef int64_t ktime;

#if defined(__GNUC__) || defined(__clang__)
#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif
#else
#ifndef likely
#define likely(x) (x)
#endif
#ifndef unlikely
#define unlikely(x) (x)
#endif
#endif

#define HAVE_POSIX_MEMALIGN 1
#define HAVE_EVENTFD 1

#endif // PROJECT_KINDOM_H
