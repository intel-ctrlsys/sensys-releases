#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2008      Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2013-2016 Intel, Inc.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

orcm_config_files = @OPAL_SITE_CONFIG_FILE@
orcm_config_file_found = @OPAL_SITE_CONFIG_FILE_FOUND@

orcm_default_config_file = @ORCM_DEFAULT_CONFIG_FILE@
orcm_default_config_file_found = @ORCM_DEFAULT_CONFIG_FILE_FOUND@
orcm_default_config_file_dist = orcm-default-config.xml

orcm_rsyslog_config_file = @OPAL_SYSLOG_CONFIG_FILE@
orcm_rsyslog_config_file_found = @OPAL_SYSLOG_CONFIG_FILE_FOUND@
orcm_rsyslog_supported_config_location = "/etc/rsyslog.d"

EXTRA_DIST = $(orcm_config_files) $(orcm_default_config_file_dist)

# Steal a little trickery from a generated Makefile to only install
# files if they do not already exist at the target.  Be sure to read
# thread starting here
# (http://www.open-mpi.org/community/lists/devel/2008/06/4080.php) for
# details why the mkdir is in install-data-local.

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(sysconfdir)
	@ p="$(orcm_config_files)"; \
        echo "**FOUND $(orcm_config_files) **"; \
        if test "$(orcm_config_file_found)" = "yes"; then \
	    if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	    echo " $(INSTALL) -m 600 $$d$$p $(DESTDIR)$(sysconfdir)/orcm-site.xml"; \
	    $(INSTALL) -m 600 $$d$$p $(DESTDIR)$(sysconfdir)/orcm-site.xml; \
        else \
            for file in $$p; do \
	            if test -f $(DESTDIR)$(sysconfdir)/orcm-site.xml; then \
	                echo "******************************* WARNING ************************************"; \
	                echo "*** Not installing new $$file over existing file in:"; \
	                echo "***   $(DESTDIR)$(sysconfdir)/$$file"; \
	                echo "******************************* WARNING ************************************"; \
	            else \
	                if test -f "$$file"; then d=; else d="$(srcdir)/"; fi; \
	                f="`echo $$file | sed -e 's|^.*/||'`"; \
	                echo " $(INSTALL_DATA) $$d$$file $(DESTDIR)$(sysconfdir)/$$f"; \
	                $(INSTALL_DATA) $$d$$file $(DESTDIR)$(sysconfdir)/$$f; \
	            fi; \
	        done \
        fi;
	@ p="$(orcm_default_config_file)"; \
	echo "**FOUND $(orcm_default_config_file)**"; \
        if test "$(orcm_default_config_file_found)" = "yes"; then \
		if test -f $(DESTDIR)$(sysconfdir)/orcm-default-config.xml; then \
			echo "******************************* WARNING ************************************"; \
			echo "*** Not installing new $$p over existing file in:"; \
			echo "***   $(DESTDIR)$(sysconfdir)/orcm-default-config.xml ";  \
			echo "******************************* WARNING ************************************"; \
		else \
			if test -f  "$$p"; then d=; else d="$(srcdir)/../"; fi; \
			echo "$(INSTALL_DATA) -m 600 $$d$$p $(DESTDIR)$(sysconfdir)/orcm-default-config.xml"; \
			$(INSTALL_DATA) -m 600 $$d$$p $(DESTDIR)$(sysconfdir)/orcm-default-config.xml; \
		fi \
	fi;
	@ p="$(orcm_rsyslog_config_file)"; \
        echo "**FOUND $(orcm_rsyslog_config_file)**"; \
        if test "$(orcm_rsyslog_config_file_found)" = "yes"; then \
	        if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
    	    echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(sysconfdir)/10-rsyslog_orcm.conf"; \
	        $(INSTALL_DATA) $$d$$p $(DESTDIR)$(sysconfdir)/10-rsyslog_orcm.conf; \
	        if test -d "$(orcm_rsyslog_supported_config_location)"; then \
	            $(INSTALL_DATA) $$d$$p $(orcm_rsyslog_supported_config_location)/10-rsyslog_orcm.conf; \
                echo "*** rsyslog support config file installed. Please, restart rsyslog."; \
            fi; \
        fi;

# Only remove if exactly the same as what in our tree
# NOTE TO READER: Bourne shell if ... fi evaluates the body if
#    the return of the evaluted command is 0 (as opposed to non-zero
#    as used by everyone else)
uninstall-local:
	@ p="$(orcm_config_files)"; \
	for file in $$p; do \
	  if test -f "$(DESTDIR)$(sysconfdir)/$$file"; then \
	  	if test -f "$$file"; then d=; else d="$(srcdir)/"; fi; \
	    if diff "$(DESTDIR)$(sysconfdir)/$$file" "$$d$$file" > /dev/null 2>&1 ; then \
	      echo "rm -f $(DESTDIR)$(sysconfdir)/$$file" ; \
	      rm -f "$(DESTDIR)$(sysconfdir)/$$file" ; \
	    fi ; \
	  fi ; \
	done
	@ p="$(orcm_default_config_file)"; \
	for file in $$p; do \
		if test -f "$(DESTDIR)$(sysconfdir)/$$file"; then \
			if test -f "$$file"; then d=; else d="$(srcdir)/../"; fi; \
			if diff "$(DESTDIR)$(sysconfdir)/$$file" "$$d$$file" > /dev/null 2>&1 ; then \
				echo "rm -f $(DESTDIR)$(sysconfdir)/$$file" ; \
				rm -f "$(DESTDIR)$(sysconfdir)/$$file" ; \
			fi ; \
		fi ; \
	done
