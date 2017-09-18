#pragma once
#include <utility>

class rational
{
public:
	int _numerator, _denominator;
private:
	static int gcd(int a, int b)
	{
		if (a < b)
			std::swap(a, b);
		if (b == 0)
			return 1;
		else
		{
			if (a % b == 0)
				return b;
			else
				return gcd(b, a % b);
		}
	}
	static int lcm(int a, int b) { return (a * b < 0 ? -a * b : a * b) / gcd(a, b); }
	bool trim()
	{
		if (_numerator == 0)
		{
			_denominator = 1;
			return true;
		}

		if ((_numerator < 0 && _denominator < 0) || (_numerator >= 0 && _denominator < 0))
		{
			_numerator = -_numerator;
			_denominator = -_denominator;
		}

		int g = gcd(_numerator, _denominator);
		if (g == 1)
			return false;

		_numerator /= g;
		_denominator /= g;
		return true;
	}
public:
	rational() : _numerator(0), _denominator(1) {}
	rational(int numerator, int denominator) : _numerator(numerator), _denominator(denominator) { trim(); }
	~rational() {};
public:
	rational operator+ (const rational& rhs) const
	{
		int g = lcm(_denominator, rhs._denominator);
		return rational(_numerator * (g / _denominator) + rhs._numerator * (g / rhs._denominator), g);
	}
	rational& operator+= (const rational& rhs)
	{
		int g = lcm(_denominator, rhs._denominator);
		_numerator = _numerator * (g / _denominator) + rhs._numerator * (g / rhs._denominator);
		_denominator = g;
		return *this;
	}
	rational operator- (const rational& rhs) const
	{
		int g = lcm(_denominator, rhs._denominator);
		return rational(_numerator * (g / _denominator) - rhs._numerator * (g / rhs._denominator), g);
	}
	rational& operator-= (const rational& rhs)
	{
		int g = lcm(_denominator, rhs._denominator);
		_numerator = _numerator * (g / _denominator) - rhs._numerator * (g / rhs._denominator);
		_denominator = g;
		return *this;
	}
	operator double() const { return static_cast<double>(_numerator) / _denominator; }

};