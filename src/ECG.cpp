#include "ECG.hpp"
#include <iostream>
#include <math.h>
ECG::ECG()
	: Fh(60)
	, cycleAmount(10)
	, t(60 * 1000 / Fh)
	, minAmpl(-3)
	, maxAmpl(10)
	, noisePower(0)
	, P("P", 1, 0, 14, 14)
	, Q("Q", -1.4, 0, 10, 10)
	, R("R", 7, 0, 11, 11)
	, S("S", -2.4, 0, 10, 10)
	, T("T", 1.5, 0, 20, 20)

{

	t = 60 * 1000 / Fh;
	counts = t;
	etalon = new float[t];
	signal = new float[t * cycleAmount];
	P.setTExtreme(t / 3.4);
	Q.setTExtreme(t / 2.7);
	R.setTExtreme(t / 2.6);
	S.setTExtreme(t / 2.5);
	T.setTExtreme(t / 2.0);
	T.setIsAltered(true);
	// isViewReal();
	// std::cout << "Counts: " << counts << std::endl;
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

void	ECG::generateSignal()
{
	if (counts != t * cycleAmount)
	{
		counts = t* cycleAmount;
		delete[] signal;
		signal = new float[counts];
	}

	int		k;
	for (int i = 0; i < (counts); i++)
	{
		k = (i) % t;
		signal[i] = 0;
		signal[i] += P.calcSignal(k);
		signal[i] += Q.calcSignal(k);
		signal[i] += R.calcSignal(k);
		signal[i] += S.calcSignal(k);
		signal[i] += T.calcSignal(k);
		signal[i] +=  noisePower * cos(i / 4.0f);
	}

}


void	ECG::calcEtalon()
{
	for (int i = 0; i < t; i++)
	{
		etalon[i] = 0;
		etalon[i] += P.calcSignal(i);
		etalon[i] += Q.calcSignal(i);
		etalon[i] += R.calcSignal(i);
		etalon[i] += S.calcSignal(i);
		etalon[i] += T.calcSignal(i);	
	}
}

float	*ECG::getEtalon() const { return (etalon); }
float	*ECG::getSignal() const { return (signal); }
int		ECG::getEtalonDuration() const { return (t); }
float	ECG::getMaxAmpl() const { return (maxAmpl); }
float	ECG::getMinAmpl() const { return (minAmpl); }
int		ECG::getFH() const { return (Fh); }
int		ECG::getCycleAmount() const { return (cycleAmount); }
int		ECG::getSignalDuration() const { return counts; }
float	ECG::getNoisePower() const { return noisePower; }

void	ECG::setCycleAmount(const int amount){ this->cycleAmount = amount; }
void	ECG::setNoisePower(float p) { noisePower = p; }
void	ECG::setFH(int newFH)
{
	Fh = newFH;
	t = 60 * 1000 / Fh;
	delete []etalon;
	etalon = new float[t];
	calcEtalon();
}

ECG::~ECG()
{
	delete []etalon;
}

Wave	&ECG::getP() { return P; }
Wave	&ECG::getQ() { return Q; }
Wave	&ECG::getR() { return R; }
Wave	&ECG::getS() { return S; }
Wave	&ECG::getT() { return T; }
