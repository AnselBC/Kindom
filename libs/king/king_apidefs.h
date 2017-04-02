//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_KING_APIDEFS_H
#define KINDOM_KING_APIDEFS_H

#define inkliapi
#define kcoreapi
#define ink_undoc_liapi
#define ink_undoc_coreapi inkcoreapi

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

#if !defined(KNORETURN)
#if defined(__GNUC__) || defined(__clang__)
#define KNORETURN __attribute__((noreturn))
#else
#define TS_NORETURN
#endif
#endif

/*  Enable this to get printf() style warnings on the Inktomi functions. */
/* #define PRINTFLIKE(IDX, FIRST)  __attribute__((format (printf, IDX, FIRST))) */
#if !defined(KPRINTFLIKE)
#if defined(__GNUC__) || defined(__clang__)
#define KPRINTFLIKE(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define KPRINTFLIKE(fmt, arg)
#endif
#endif

#if !defined(KNONNULL)
#if defined(__GNUC__) || defined(__clang__)
#define KNONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define KNONNULL(...)
#endif
#endif

#if !defined(KINLINE)
#define TS_INLINE inline
#endif

#endif // KINDOM_KING_APIDEFS_H
