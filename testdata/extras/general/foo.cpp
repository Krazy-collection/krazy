// test data for krazy

//contractions
str = i18n( "don't" ); // contractions
str = i18n( "do notwon't"); // ok
now is the time for all //ok (not in string)
str += i18n("We'd,probably do it's job better") // contractions
str = i18n("we can't") + "'" + i18n("do this"); // contractions
str = i18n("we can" + "'t" + "'" + "do this"); // ok
I18N_NOOP("it's am we've don't it") // contractions

