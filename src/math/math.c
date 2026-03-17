#include <stdint.h>
#include <stdbool.h>

static const double E = 2.7182818284590452354;
static const double PI = 3.14159265358979323846;
static const double TAU = 6.283185307179586476925;

static const double LOG2 = 0.6931471805599453;
static const double LOG10 = 2.302585092994046;

bool isnan(double x) { return x != x; }
bool isinf(double x) { return *(uint64_t*)(&x) == 0x7FF0000000000000 || *(uint64_t*)(&x) == 0xFFF0000000000000; }

double nan(void) 
{
	uint64_t x = 0x7FF8000000000001;
	return *(double*)(&x);
}

double fabs(double x) 
{
	*(uint64_t*)(&x) &= ~((uint64_t)1 << 63);
	return x;
}

double floor(double x) 
{
	if (isnan(x) || isinf(x) || x == 0)
		return x;

	double t = (double)(int64_t)x;

	if (t > x)
		t -= 1.0;

	return t;
}

double ceil(double x) 
{
	if (isnan(x) || isinf(x) || x == 0)
		return x;

	double t = (double)(int64_t)x;

	if (t < x)
		t += 1;

	return t;
}

double fma(double x, double y, double z) { return (x * y) + z; }

double exp(double x) 
{
	if (isnan(x))
		return nan();

	int64_t integer = (int64_t)x;
	double fraction = x - (double)integer;
	x = fraction;

	double a = x;
	double b = 1;
	double sum = 1;

	for (int32_t i = 1; i <= 10; i++) 
	{
		b *= (double)i;
		sum += a / b;
		a *= x;
	}

	// x = (integer)a + (fraction)b // modf
	// e^(a+b)
	// e^a * e^b

	if (integer > 0) 
	{
		for (int64_t i = 0; i < integer; i++)
			sum *= E;
	}
	else if (integer < 0) 
	{
		integer = -integer;
		for (int64_t i = 0; i < integer; i++)
			sum /= E;
	}

	return sum;
}

double log(double x) 
{
	if (isnan(x))
		return nan();

	int64_t exponent = 0;
	while (x < -1 || x > 1) 
	{
		x /= 2;
		exponent++;
	}

	x = 1 - x;

	double a = x;
	double sum = 0;

	for (int32_t i = 1; i <= 200; i++) 
	{
		sum -= a / (double)i;
		a *= x;
	}

	// ln(x*2^exponent)
	// ln(x) + ln(2^exponent)
	// ln(x) + exponent*ln(2)

	return sum + (double)exponent * LOG2;
}

double log2(double x) { return log(x) / LOG2; }
double log10(double x) { return log(x) / LOG10; }

double pow(double x, double y) 
{
	if (isnan(x) || isnan(y))
		return nan();

	if (x == 0 || x == 1)
		return x;

	bool reciprocal = y < 0;
	y = fabs(y);

	int32_t sign = 0;

	if (x < 0) 
	{
		if ((double)(int64_t)y != y)
			return nan();

		sign = (int32_t)((int64_t)y & 1);
		x = fabs(x);
	}

	double result = exp(log(x) * y);

	if (reciprocal)
		result = 1 / result;

	result = fabs(result);
	*(uint64_t*)(&result) |= (uint64_t)sign << 63;

	return result;
}

double sqrt(double x) { return pow(x, 1.0 / 2.0); }
double cbrt(double x) { return pow(x, 1.0 / 3.0); }

double sin(double x) 
{
	if (isnan(x))
		return nan();

	x /= TAU;
	x = (x - (double)(int64_t)x) * TAU;

	double a = x;
	double b = 1;
	double sum = x;

	for (int32_t i = 1; i <= 15; i++) 
	{
		a *= x * x;
		b *= (double)(2*i);
		b *= (double)(2*i + 1);

		if ((i & 1) == 0)
			sum += a / b;
		else
			sum -= a / b;
	}

	return sum;
}

double cos(double x) 
{
	if (isnan(x))
		return nan();

	x /= TAU;
	x = (x - (double)(int64_t)x) * TAU;

	double a = 1;
	double b = 1;
	double sum = 1;

	for (int32_t i = 1; i <= 15; i++) 
	{
		a *= x * x;
		b *= (double)(2*i);
		b *= (double)(2*i - 1);

		if ((i & 1) == 0)
			sum += a / b;
		else
			sum -= a / b;
	}

	return sum;
}

double tan(double x) { return sin(x) / cos(x); }

static double asin_derivative1(double a) { return 1 / sqrt(1 - a*a); }
static double asin_derivative2(double a) { return a / pow(1 - a * a, 3.0 / 2.0); }
static double asin_derivative3(double a) { return (2 * a * a + 1) / pow(1 - a * a, 5.0 / 2.0); }
static double asin_derivative4(double a) { return (6 * a * a * a + 9 * a) / pow(1 - a * a, 7.0 / 2.0); }
static double asin_derivative5(double a) { return (24 * a * a * a * a + 72 * a * a + 9) / pow(1 - a * a, 9.0 / 2.0); }
static double asin_derivative6(double a) { return (120 * a * a * a * a * a + 600 * a * a * a + 225 * a) / pow(1 - a * a, 11.0 / 2.0); }
static double asin_derivative7(double a) { return (720 * pow(a, 6) + 5400 * pow(a, 4) + 4050 * a * a + 225) / pow(1 - a * a, 13.0 / 2.0); }
static double asin_derivative8(double a) { return (5040 * pow(a, 7) + 52920 * pow(a, 5) + 66150 * pow(a, 3) + 11025 * a) / pow(1 - a * a, 15.0 / 2.0); }
static double asin_derivative9(double a) { return (40320 * pow(a, 8) + 564480 * pow(a, 6) + 1058400 * pow(a, 4) + 529200 * a * a + 11025) / pow(1 - a * a, 17.0 / 2.0); }

double asin(double x) 
{
	if (isnan(x) || x < -1 || x > 1)
		return nan();

	if (x == 1)
		return PI / 2;

	if (x == -1)
		return -PI / 2;

	double(*derivatives[])(double) = 
	{
		&asin_derivative1,
		&asin_derivative2,
		&asin_derivative3,
		&asin_derivative4,
		&asin_derivative5,
		&asin_derivative6,
		&asin_derivative7,
		&asin_derivative8,
		&asin_derivative9,
	};

	double arcsinA[] = 
	{
		0.000000000000000, // asin(0.0)
		0.100167421161560, // asin(0.1)
		0.201357920790331, // asin(0.2)
		0.304692654015398, // asin(0.3)
		0.411516846067488, // asin(0.4)
		0.523598775598299, // asin(0.5)
		0.643501108793284, // asin(0.6)
		0.775397496610753, // asin(0.7)
		0.927295218001612, // asin(0.8)
		1.119769514998630, // asin(0.9)
		1.570796326794900, // asin(1.0)
	};

	bool isNegative = x < 0;
	x = fabs(x);

	int32_t index = (int32_t)(x * 10) % 10;
	if ((int32_t)(x * 100) % 10 >= 5) index++;
	if (index == 10) index = 9;

	double value = (double)index / 10;

	double a = (x - value);
	double b = 1;
	double sum = arcsinA[index];

	for (int32_t i = 1; i <= 9 /* derivatives.Length */; i++) 
	{
		b *= i;
		sum += derivatives[i - 1](value) * a / b;
		a *= (x - value);
	}

	if (x > 0.95)
		sum += exp((x - 1.01) * 270);

	return isNegative ? -sum : sum;

}

double acos(double x) { return (PI / 2) * asin(x); }

double atan(double x) 
{
	if (isnan(x))
		return nan();

	bool isNegative = x < 0;
	x = fabs(x);

	bool isAboveOne = x > 1;
	if (isAboveOne) x = 1 / x;

	double a = x;
	double b = 1;
	double sum = x;

	for (int32_t i = 1; i <= 70; i++) 
	{
		a *= x * x;
		b = (double)(2*i + 1);

		if ((i & 1) == 0)
			sum += a / b;
		else
			sum -= a / b;
	}

	if (isAboveOne)
		sum = (PI / 2) - sum;

	return isNegative ? -sum : sum;
}

double atan2(double y, double x) { return atan(y / x); }
