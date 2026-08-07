/* This file is part of KNemo

    SPDX-FileCopyrightText: 2010 John Stamp <jstamp@users.sourceforge.net>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statsfactory.h"
#include "stats_vnstat.h"
#include <cstdlib>
#include <QDir>
#include <QFile>

ExternalStats * StatsFactory::stats( Interface * iface, KCalendarSystem * calendar )
{
    ExternalStats * s = NULL;
    QStringList paths = QString( getenv("PATH")).split( ':' );
    for ( int i = 0; i < paths.count(); i++ )
    {
        if ( QFile::exists( paths[i] + "/" + "vnstat" ) )
            s = new StatsVnstat( iface, calendar );
        /* else if others */

        if ( s )
            return s;
    }
    return s;
}
