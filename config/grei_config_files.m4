# -*- shell-script -*-
#
# Copyright (c) 2013-2014 Intel, Inc. All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

AC_DEFUN([GREI_CONFIG_FILES],[
    AC_CONFIG_FILES([
    orcm/test/Makefile
    orcm/test/gtest_example/Makefile
    orcm/test/mca/Makefile
    orcm/test/mca/sensor/Makefile
    orcm/test/mca/sensor/ft_tester/Makefile
    orcm/test/mca/sensor/ipmi/Makefile
    ])
])