//QString::null stuff
str = QString::null // nullstrassign
//str = QString::null; // nullstrassign
if (str == QString::null) // nullstrcompare
str = QString::null; //Krazy:exclude=nullstrcompare


str = QString() ; //ok
str += QString(); //ok
str.clear();      //ok
