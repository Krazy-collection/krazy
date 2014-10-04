/*
 * passbyvalue
 *
 * Q&D C++ symbol parser
 * ...to detect nasty pass-by-value function arguments for EBN
 *
 * Based on the Kate C/C++ symbol viewer plugin
 *
 * Copyright (C) Andreas Kling 2006
 * Copyright (C) Massimo Callegari 2003
 * Copyright Allen Winter 2007 <winter@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <QString>
#include <QList>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

struct Function
{
    Function( int l, const QString &t, const QString &n ) : lineNumber(l), text(t), name(n) {}
    int lineNumber; QString text; QString name;
};

int
main( int argc, char **argv )
{
    if( argc == 1 ) {
        fprintf( stderr, "usage: %s <options> <source-file>\n", argv[0] );
        return 0;
    }

    bool verbose = false;
    bool quiet = false;

    QString fileName;

    /* I *did* make it myself, how did you know!? */
    for( --argc, ++argv; argc; --argc, ++argv ) {
        if( !strcmp( *argv, "--verbose" ))
            verbose = true;
        else if( !strcmp( *argv, "--quiet" ))
            quiet = true;
        else if( !strcmp( *argv, "--version" )) {
            printf( "passbyvalue, version 0.4\n" );
            return 0;
        }
        else if( !strcmp( *argv, "--help" )) {
            printf( "Check for inappropriate pass-by-value function args\n" );
            return 0;
        }
        else if( !strcmp( *argv, "--explain" )) {
            printf( "Passing large objects by value is wasteful and slow. Use references-to-const instead. For example: 'func( QDateTime dt )' is better written as 'func( const QDateTime &dt )'\n" );
            return 0;
        } else {
            fileName = QString::fromLatin1( *argv );
            break;
        }
    }

    if( fileName.isNull() )
        return 1;

    if( !fileName.toLower().endsWith( ".cpp" ) &&
        !fileName.toLower().endsWith( ".cc"  ) &&
        !fileName.toLower().endsWith( ".cxx" )) {
        if( !quiet )
            printf( "okay\n" );
        return 0;
    }

    int nastyCount = 0;

    QStringList QTypes;
    QTypes << "QString" << "QStringList"
           << "QWidget" << "QWidgetBinding"
           << "QPixmap" << "QImage" << "QIcon"
           << "QPainter" << "QRect" << "QSize" << "QPoint"
           << "QPalette" << "QColor"
           << "QDomElement" << "QDomNode"
           << "QByteArray" << "QUrl" << "QRegExp"
           << "QVariant" << "QVariantList" << "QVariantMap" << "QUiLoader"
           << "QDate" << "QDateTime";
    QStringList KTypes;
    KTypes << "KIcon" << "KPixmap" << "KUrl" << "KDateTime"
           << "KBookmark" << "KBookmarkGroup";
    QStringList nastyTypes = QTypes + KTypes;

    QFile sourceFile( fileName );
    if( !sourceFile.open( QIODevice::ReadOnly ))
        return 1;

    QTextStream inputStream( &sourceFile );

    QStringList structs;
    QList<Function> functions;

    QString guiltyText;
    QString cl; // Current Line
    QString stripped;

    int guiltyLine = 0, lineNumber = 0;
    int j;
    int par = 0, graph = 0;
    char block = 0, comment = 0, macro = 0;
    bool structure = false;

    QList<QRegExp> skipFileMarkers;
    skipFileMarkers << QRegExp ("//.*[Kk]razy:excludeall=.*passbyvalue");
    skipFileMarkers << QRegExp ("//.*[Kk]razy:skip");

    QList<QRegExp> skipLineMarkers;
    skipLineMarkers << QRegExp ("//.*[Kk]razy:exclude=.*passbyvalue");
    skipLineMarkers << QRegExp ("foreach");
    skipLineMarkers << QRegExp ("[QK]_GLOBAL");
    skipLineMarkers << QRegExp ("[QK]_DECLARE");
    skipLineMarkers << QRegExp ("[QK]_EXPORT");
    skipLineMarkers << QRegExp ("operator[><=]");

    while (!sourceFile.atEnd()) {
        //cl = inputStream.readLine();
        cl = sourceFile.readLine();

        const QChar *text = cl.unicode();
        const int length  = cl.length();

        lineNumber++;

        bool skip = false;
        foreach(QRegExp const &re, skipFileMarkers) {
            if ( cl.contains(re) ) {
               skip = true;
               break;
            }
        }
        if ( skip ) {
            break;
        }

        if ( cl.trimmed().startsWith("//") ) {
            continue;
        }

        foreach(QRegExp const &re, skipLineMarkers) {
            if ( cl.contains(re) ) {
               skip = true;
               break;
            }
        }
        if ( skip ) {
            continue;
        }

        if(cl.contains("/*") && graph == 0) comment = 1;
        if(cl.contains("*/") && graph == 0) comment = 0;
        if (comment != 1) {
            if(macro == 1) {
                for (j = 0; j < length; j++) {
                    if(text[j]=='/' && text[j+1]=='/') { macro = 0; break; }
                    if(  cl.indexOf("define") == j &&
                         !(cl.indexOf("defined") == j)) {
                        macro = 2;
                        j += 6; // skip the word "define"
                    }
                    if(macro == 2 && text[j] != ' ') macro = 3;
                    if(macro == 3) {
                        if (text[j] >= 0x20) stripped += text[j];
                        if (text[j] == ' ' || j == length - 1)
                            macro = 4;
                    }
                    //qDebug(13000)<<"Macro -- Stripped : "<<stripped<<" macro = "<<macro<<endl;
                }
                // I didn't find a valid macro e.g. include
                if(j == length && macro == 1) macro = 0;
                if(macro == 4) {
                    //macros << stripped.simplified();
                    macro = 0;
                    stripped.clear();
                    //qDebug(13000)<<"Macro -- Inserted : "<<stripped<<" at row : "<<i<<endl;
                    if (text[length - 1] == '\\') macro = 5; // continue in rows below
                    continue;
                }
            }
            if (macro == 5 && cl.at(cl.length() - 1) != '\\') { macro = 0; continue; }

            /* ******************************************************************** */
            //if (cl.at(j) == '"' && comment == 2) { comment = 0; j++; }
            //else if (cl.at(j) == '"' && comment == 0) comment = 2;
            if(cl.contains('(') && text[0] != '#' && block == 0 && comment != 2)
            { structure = false; block = 1; }
            if((cl.contains("typedef") || cl.contains("struct") ) && graph == 0 && block == 0)
            { structure = true; block = 2; stripped.clear(); }
            //if(cl.contains(';') && graph == 0)
            //    block = 0;

            if(block > 0) {
                for (j = 0; j < length; j++) {
                    if (text[j] == '/' && (text[j + 1] == '*')) comment = 2;
                    if (text[j] == '*' && (text[j + 1] == '/')) {  comment = 0; j+=2; }
                    if (text[j] == '"' && comment == 2) { comment = 0; j++; }
                    else if (text[j] == '"' && comment == 0) comment = 2;
                    if(text[j]=='/' && text[j+1]=='/')
                    { if(block == 1 && stripped.isEmpty()) block = 0; break; }
                    if (comment != 2)
                    {
                        if (block == 1 && graph == 0 )
                        {
                            if(text[j] >= 0x20) stripped += text[j];
                            if(text[j] == '(') par++;
                            if(text[j] == ')')
                            {
                                par--;
                                if(par == 0)
                                {
                                    stripped = stripped.simplified();
                                    guiltyText = stripped;
                                    guiltyLine = lineNumber;
                                    stripped.remove("static ");
                                    stripped.remove("virtual ");
                                    //qDebug(13000)<<"Function -- Inserted : "<<stripped<<" at row : "<<i;
                                    block = 2;
                                }
                            }
                        } // BLOCK 1
                        if(block == 2 && graph == 0)
                        {
                            if(text[j]=='/' && text[j+1]=='/') break;
                            //if(text[j]==':' || text[j]==',') { block = 1; continue; }
                            if(text[j]==':') { block = 1; continue; }
                            if(text[j]==';')
                            {
                                stripped.clear();
                                block = 0;
                                structure = false;
                                break;
                            }

                            if(text[j]=='{' && structure == false && !cl.contains(';'))
                            {
                                functions << Function( guiltyLine, guiltyText.trimmed(), stripped.simplified() );
                                stripped.clear();
                                block = 3;
                            }
                            if(text[j]=='{' && structure == true)
                            {
                                block = 3;
                            }
                            if(text[j]=='(' && structure == true)
                            {
                                block = 0;
                                j = 0;
                                //qDebug(13000)<<"Restart from the beginning of line...";
                                stripped.clear();
                                break; // Avoid an infinite loop :(
                            }
                            if(structure == true && text[j] >= 0x20) stripped += text[j];
                        } // BLOCK 2

                        if (block == 3)
                        {
                            // A comment...there can be anything
                            if(text[j]=='/' && text[j+1]=='/') break;
                            if(text[j]=='{') graph++;
                            if(text[j]=='}')
                            {
                                graph--;
                                if (graph == 0 && structure == false) block = 0;
                                if (graph == 0 && structure == true) block = 4;
                            }
                        } // BLOCK 3

                        if (block == 4)
                        {
                            if(text[j] == ';')
                            {
                                stripped.remove('{');
                                stripped.replace('}', ' ');
                                structs << stripped;
                                //qDebug(13000)<<"Structure -- Inserted : "<<stripped<<" at row : "<<i;
                                stripped.clear();
                                block = 0;
                                structure = false;
                                //break;
                                continue;
                            }
                            if (text[j] >= 0x20) stripped += text[j];
                        } // BLOCK 4
                    } // comment != 2
                    //qDebug(13000)<<"Stripped : "<<stripped<<" at row : "<<i;
                } // End of For cycle
            } // BLOCK > 0
        } // Comment != 1
    }

    for( QList<Function>::iterator it = functions.begin(); it != functions.end(); ++it )
    {
        const int lineNumber = it->lineNumber;
        const QString &prototype = it->name;

        int beginParen = prototype.indexOf( '(' );
        int closeParen = prototype.indexOf( ')' );

        if( beginParen < 0 || closeParen < 0 )
            continue;

        QString betweenParens = prototype.mid( beginParen, closeParen - beginParen + 1 );
        betweenParens = betweenParens.left( betweenParens.length() - 1 );
        betweenParens = betweenParens.right( betweenParens.length() - 1 );

        if( betweenParens.isEmpty() )
            continue;

        QStringList arguments = betweenParens.split( ',' );

        for( QStringList::iterator iit = arguments.begin(); iit != arguments.end(); ++iit )
        {
            QStringList parameter = iit->simplified().split(QRegExp("\\s"));

            bool isPointer = false;
            bool isReference = false;
            bool isAssign = false;
            bool haveType = false;
            QString name;
            QString type;

            for( QStringList::iterator ait = parameter.begin(); ait != parameter.end(); ++ait )
            {
                if ( *ait == "const&" )
                    isReference = true;
                else if ( *ait == "const*" )
                    isPointer = true;
                else if( *ait == "&" )
                    isReference = true;
                else if( *ait == "*" )
                    isPointer = true;
                else if( *ait == "=" )
                    isAssign = true;
                else if( haveType )
                    name = *ait;
                else
                {
                    haveType = true;
                    type = *ait;
                }
            }

            if( name.startsWith( '*' ))
            {
                isPointer = true;
                name = name.right( name.length() - 1 );
            }

            if( type.endsWith( '*' ))
            {
                isPointer = true;
                type = type.left( type.length() - 1 );
            }
            if( name.startsWith( '&' ))
            {
                isReference = true;
                name = name.right( name.length() - 1 );
            }
            if( type.endsWith( '&' ))
            {
                isReference = true;
                type = type.left( type.length() - 1 );
            }
            betweenParens = betweenParens.left( betweenParens.length() - 1 );

            if( !isReference && !isPointer && !isAssign )
            {
                if( nastyTypes.contains( type ) )
                {
                    nastyCount++;
                    if( !quiet )
                    {
                        if( verbose )
                            printf( "%s:%d %s\n", qPrintable(fileName), lineNumber, qPrintable(it->text) );
                        else
                            printf( "line#%d[%s]\n", lineNumber, qPrintable(type) );
                    }
                }
            }
        }
    }

    if( nastyCount == 0 )
    {
        if( !quiet )
            printf( "okay\n" );
        return 0;
    }

    printf("ISSUES=%d\n", nastyCount);
    return nastyCount;
}
/* kate: space-indent on; indent-width 4; */
