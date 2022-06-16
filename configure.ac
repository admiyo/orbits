
AC_INIT([orbits], [0.1], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AM_PROG_AR
LT_INIT
AC_CHECK_LIB([glut], [glutInit])
AC_CHECK_LIB([GL],[glVertex2f])
PKG_CHECK_MODULES([GL], [gl])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
