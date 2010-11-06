// emptystrcompare
if(str=="") //emptystrcompare
if(   str       ==		"") //emptystrcompare
if (str == "\"") //ok
if (""==str) //emptystrcompare
str += "if (str == "")"; //ok
if (   " " == str) //ok
if (   "" !=str) //emptystrcompare
if (   "" !=str) //krazy:exclude= emptystrcompare
