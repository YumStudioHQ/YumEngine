#ifndef __LIBYUM_VERSION_H__
# define __LIBYUM_VERSION_H__
# define LIBYUM_VERSION_ENCODE(major, minor, patch) ((major) * 10000 + (minor) * 100 + (patch))
# ifndef LIBYUM_VERSION
#   define LIBYUM_VERSION LIBYUM_VERSION_ENCODE(1, 2, 14)
# endif
# define LIBYUM_VERSION_MAJOR (LIBYUM_VERSION / 10000)
# define LIBYUM_VERSION_MINOR ((LIBYUM_VERSION / 100) % 100)
# define LIBYUM_VERSION_PATCH (LIBYUM_VERSION % 100)
# define LIBYUM_BRANCH "main"
# define LIBYUM_STUDIO "yum_official"
#endif // __LIBYUM_VERSION_H__