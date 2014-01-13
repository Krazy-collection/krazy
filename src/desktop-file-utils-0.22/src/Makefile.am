bin_PROGRAMS =					\
	desktop-file-validate			\
	desktop-file-install			\
	update-desktop-database

AM_CPPFLAGS =					\
	$(DESKTOP_FILE_UTILS_CFLAGS)		\
	-DDATADIR="\"$(datadir)\"" 		\
	-D_FILE_OFFSET_BITS=64			\
	-D_LARGEFILE64_SOURCE

desktop_file_validate_SOURCES =			\
	keyfileutils.c				\
	keyfileutils.h				\
	mimeutils.c				\
	mimeutils.h				\
	validate.c				\
	validate.h				\
	validator.c

desktop_file_install_SOURCES =			\
	keyfileutils.c				\
	keyfileutils.h				\
	mimeutils.c				\
	mimeutils.h				\
	validate.c				\
	validate.h				\
	install.c

update_desktop_database_SOURCES =		\
	mimeutils.c				\
	mimeutils.h				\
	update-desktop-database.c

desktop_file_validate_LDADD = $(DESKTOP_FILE_UTILS_LIBS)
desktop_file_install_LDADD = $(DESKTOP_FILE_UTILS_LIBS)
update_desktop_database_LDADD = $(DESKTOP_FILE_UTILS_LIBS)

install-exec-hook: desktop-file-install
	cd $(DESTDIR)$(bindir) && \
	rm -f desktop-file-edit && \
	$(LN_S) -f desktop-file-install desktop-file-edit

uninstall-hook:
	cd $(DESTDIR)$(bindir) && \
	rm -f desktop-file-edit

-include $(top_srcdir)/git.mk
