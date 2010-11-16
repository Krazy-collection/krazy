/***************************************************************************
*   Copyright © 2003 Jason Kivlighn <jkivlighn@gmail.com>                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef MIXEDNUMBER_H
#define MIXEDNUMBER_H

#include <QString>

/** remove any extra zeros on the end of the string and the decimal if a whole number */
QString beautify( const QString &num );

class KLocale;

/** A class to hold and manipulate a mixed number.
  * @author Jason Kivlighn
  */
class MixedNumber
{
public:
	MixedNumber( int whole, int numerator, int denominator );

	/** Create a mixed number from the given @param decimal.  This uses a method that will
	  * approximate the actual fraction with precision equal to @param precision.
	  * The closer @param precision is to zero without being zero, the more precisely
	  * it will try to approximate the fraction.
	  */
	MixedNumber( double decimal, double precision = 1e-6 );

	/** Creates a mixed number with an initial value of zero. */
	MixedNumber();

	~MixedNumber();

	MixedNumber& operator+=( const MixedNumber & );
	MixedNumber& operator+=( double );
	bool operator!=( const MixedNumber &fraction );
	bool operator>( double d )
	{
		return ( toDouble() > d );
	}

	enum Format { DecimalFormat, MixedNumberFormat };

	/** The input as a decimal. */
	double toDouble() const;

	/** Returns the fraction as a string */
	QString toString( Format = MixedNumberFormat, bool locale_aware = true ) const;

	/** The whole part of the input */
	int whole() const
	{
		return m_whole;
	}

	/** The numerator of the fractional part of the input. */
	int numerator() const
	{
		return m_numerator;
	}

	/** The denominator of the fractional part of the input. */
	int denominator() const
	{
		return m_denominator;
	}

	void setNumerator( int n )
	{
		m_numerator = n;
	}
	void setDenominator( int d )
	{
		m_denominator = d;
	}

	/** Ensure that the fraction is simplified to its lowest terms. */
	void simplify();

	/** Parses the given QString as a mixed number.  The input can be
	  * expressed as a mixed number in the form "a b/c", or as a decimal.
	  */
	static MixedNumber fromString( const QString &input, bool *ok = 0, bool locale_aware = true );

	static bool isFraction( const QString &input );

private:
	static int getNumerator( const QString &input, int space_index, int slash_index, bool *ok );
	static int getDenominator( const QString &input, int slash_index, bool *ok );

	int gcd( int, int );

	int m_whole;
	int m_numerator;
	int m_denominator;

	KLocale *locale;
};

inline const MixedNumber operator+( const MixedNumber &mn1, const MixedNumber &mn2 )
{
	MixedNumber tmp = mn1;
	tmp += mn2;
	return tmp;
}


inline const MixedNumber operator+( double d, const MixedNumber &mn )
{
	MixedNumber tmp = mn;
	tmp += d;
	return tmp;
}

inline const MixedNumber operator+( const MixedNumber &mn, double d )
{
	return operator+(d,mn);
}


#endif //MIXEDNUMBER_H
