/* validate.h: validate a desktop entry file
 * 
 * Copyright (C) 2007 Vincent Untz <vuntz@gnome.org>
 *
 * A really small portion of this code comes from the old validate.h.
 * Authors of the old validate.h are:
 *  Mark McLoughlin
 *  Havoc Pennington
 *  Ray Strode
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

#include <glib.h>

#define CURRENT_SPEC_VERSION "1.0"

#define GROUP_KDE_DESKTOP_ENTRY "KDE Desktop Entry"
#define GROUP_DESKTOP_ACTION "Desktop Action "

gboolean desktop_file_validate (const char *filename,
				gboolean    warn_kde,
				gboolean    no_warn_deprecated);
gboolean desktop_file_fixup    (GKeyFile   *keyfile,
                                const char *filename);

