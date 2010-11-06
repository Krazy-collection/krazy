// postfix op
for( int i = 0; i < foo; i++ ) // Hit
for( int i = 0; i < foo; i++) // Hit
for( int i = 0; i < foo;i++) // Hit
for( int j = 0; j < foo; j-- ) // Hit
for( int j = 0; j < foo; j--) // Hit
for( int j = 0; j < foo;j--) // Hit
for( int i = 0; i < foo
	; i++ ) // Hit
for( int i = 0; i < foo; ++i ) // not a hit
	 someVar++ ) // Not a hit
someVar++; // Not a hit. See note in postfixop script.
someVar--; // Not a hit. See note in postfixop script.
some = var++ - 3; // Not a hit. See note in postfixop script.
some = var-- + 3; // Not a hit. See note in postfixop script.
for (i=0;
     i<N;
     i++) { //hit
	for (i = 0; i < N;
             i++) // hit
        { j++; //not a hit
for (int i = 0, j = 1; j < result.count(); j++ ) { // Hit
for (int i = 0, j = 1; j < result.count(); i = j++ ) { // Not a hit
result.removeAt( j-- );  //Not a hit

