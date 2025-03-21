/* mimeutils.h: useful functions related to mime types
 * vim: set ts=2 sw=2 et: */

/*
 * Copyright (C) 2008 Novell, Inc.
 *
 * Written by Vincent Untz <vuntz@gnome.org>.
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

#ifndef __DFU_MIMEUTILS_H__
#define __DFU_MIMEUTILS_H__

#include <glib.h>

typedef enum {
  MU_VALID,
  MU_DISCOURAGED,
  MU_INVALID
} MimeUtilsValidity;

MimeUtilsValidity mu_mime_type_is_valid (const char  *mime_type,
                                         char       **error);

#endif /* __DFU_MIMEUTILS_H__ */
