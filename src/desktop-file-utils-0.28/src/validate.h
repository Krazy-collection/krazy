/* validate.c: validate a desktop entry file
 * vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2007 Vincent Untz <vuntz@gnome.org>
 *
 * A really small portion of this code comes from the old validate.c.
 * The old validate.c was Copyright (C) 2002  Red Hat, Inc.
 * It was written by:
 *  Havoc Pennington <hp@pobox.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef __DFU_VALIDATE_H__
#define __DFU_VALIDATE_H__

#include <glib.h>

#define CURRENT_SPEC_VERSION "1.5"

#define GROUP_KDE_DESKTOP_ENTRY "KDE Desktop Entry"
#define GROUP_DESKTOP_ACTION "Desktop Action "

gboolean desktop_file_validate (const char *filename,
				gboolean    warn_kde,
				gboolean    no_warn_deprecated,
				gboolean    no_hints);
gboolean desktop_file_fixup    (GKeyFile   *keyfile,
                                const char *filename);

#endif /* __DFU_VALIDATE_H__ */
