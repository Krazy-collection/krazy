//qstring + qchar
str += "(";  //doublequote_chars
str += "allen" + "d" + "winter"; //doublequote_chars
str += "allen"; //ok
str = "al" + "llen"; //ok
str = "a" + "bc" + "d"; //doublequote_chars
str.replace("abc", "" ); //doublequote_chars
str.replace("abc", QString( )) //doublequote_chars
str.remove("a"); //doublequote_chars

