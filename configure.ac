AC_INIT([bobble], [0.0.0], [rjmars97@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_PROG_CC
AC_PROG_CXX

#AC_CHECK_HEADER([opencv2/opencv.hpp], [], 
#                AC_MSG_ERROR([opencv2/opencv.hpp not found])
#)

AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

#AX_CXX_CHECK_LIB()
AC_SEARCH_LIBS([gps_open], [gps], [], [AC_MSG_ERROR([unable to find libgps])])
AC_SEARCH_LIBS([pthread_create], [pthread], [], 
               [AC_MSG_ERROR([unable to find libpthread])])

PKG_CHECK_MODULES(OPENCV, opencv)

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
