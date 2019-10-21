#include "ECG.hpp"
#include <iostream>
ECG::ECG()
	: Fh(60)
	, t(60 * 1000 / Fh)
	, minAmpl(-5)
	, maxAmpl(10) 
	, P("P", 1, 0, 14, 14)
	, Q("Q", -1.4, 0, 10, 10)
	, R("R", 7, 0, 11, 11)
	, S("S", -2.4, 0, 10, 10)
	, T("T", 1.5, 0, 20, 20)

{
	t = 60 * 1000 / Fh;
	counts = t;
	signal = new float[counts]();
	P.setTExtreme(t / 3.4);
	Q.setTExtreme(t / 2.7);
	R.setTExtreme(t / 2.6);
	S.setTExtreme(t / 2.5);
	T.setTExtreme(t / 2.0);
	isViewReal();
	std::cout << "Counts: " << counts << std::endl;
}

bool	ECG::isPViewReal()
{
	if ( 0 <= P.getTBegin() &&  P.getTBegin() < P.getTEnd())
		return (true);
	return (false);
}

bool	ECG::isQViewReal()
{
	if (P.getTEnd() <= Q.getTBegin() && Q.getTBegin() < Q.getTEnd())
		return (true);
	return (false);
}

bool	ECG::isRViewReal()
{
	if (Q.getTEnd() == R.getTBegin() && R.getTBegin() < R.getTEnd())
		return (true);
	return (false);
}

bool	ECG::isSViewReal()
{
	if (R.getTEnd() == S.getTBegin() && S.getTBegin() < S.getTEnd())
		return (true);
	return (false);
}

bool	ECG::isTViewReal()
{
	if (S.getTEnd() <= T.getTBegin() && T.getTBegin() < T.getTEnd())
		return (true);
	return (false);
}

bool	ECG::isViewReal()
{
	bool	res = true;
	if (isPViewReal() == false)
	{
		std::cout << "P wave parameters is bad!\n";
		res = false;
	}
	if (isQViewReal() == false)
	{
		std::cout << "Q wave parameters is bad!\n";
		res = false;
	}
	if (isRViewReal() == false)
	{
		std::cout << "R wave parameters is bad!\n";
		res = false;
	}
	if (isSViewReal() == false)
	{
		std::cout << "S wave parameters is bad!\n";
		res = false;
	}
	if (isTViewReal() == false)
	{
		std::cout << "T wave parameters is bad!\n";
		res = false;
	}
	return (res);
}

void	ECG::calcFunction()
{
	for (int i = 0; i < counts; i++)
	{
		signal[i] = 0;
		signal[i] += P.calcSignal(i);
		signal[i] += Q.calcSignal(i);
		signal[i] += R.calcSignal(i);
		signal[i] += S.calcSignal(i);
		signal[i] += T.calcSignal(i);	
	}
}

float	*ECG::getSignal() const { return (signal); }
int		ECG::getCounts() const { return (counts); }
float	ECG::getMaxAmpl() const { return (maxAmpl); }
float	ECG::getMinAmpl() const { return (minAmpl); }
int		ECG::getFH() const { return (Fh); }

void	ECG::setFH(int newFH)
{
	Fh = newFH;
	t = 60 * 1000 / Fh;
	counts = t;
	delete []signal;
	signal = new float[counts]();
	calcFunction();
}

ECG::~ECG()
{
	delete []signal;
}

Wave	&ECG::getP() { return P; }
Wave	&ECG::getQ() { return Q; }
Wave	&ECG::getR() { return R; }
Wave	&ECG::getS() { return S; }
Wave	&ECG::getT() { return T; }
