#include "ECG.hpp"
#include <iostream>
ECG::ECG()
	: P("P", 1, 0, 14, 14)
	, Q("Q", -1.4, 0, 10, 10)
	, R("R", 7, 0, 11, 11)
	, S("S", -2.4, 0, 10, 10)
	, T("T", 1.5, 0, 20, 20)
	, Fh(60)
	, tStep(0.001)
	, minAmpl(-5)
	, maxAmpl(10)
{
	t = 60 * 1000 / Fh;
	counts = t;
	signal = new float[counts]();
	P.setTExtreme(t / 3.4);
	Q.setTExtreme(t / 2.7);
	R.setTExtreme(t / 2.6);
	S.setTExtreme(t / 2.5);
	T.setTExtreme(t / 2.0);
	std::cout << "Counts: " << counts << std::endl;
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
		// signal[i] += ST.calcSignal(i);	
		signal[i] += T.calcSignal(i);	
		// currT += tStep;
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
