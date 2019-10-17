#include "ECG.hpp"

ECG::ECG()
	: P("P", 0.1f, 0.35f, 0.45f, 0.4f, 0.1f, 0.1f, 0.1f)
	, Q("Q", 0.025f, 0.45f, 0.475f, 0.47f, 0.1f, 0.1f, -0.1f)
	, R("R", 0.1f, 0.475f, 0.525f, 0.5f, 0.1f, 0.1f, 1.0f)
	, S("S", 0.1f, 0.525f, 0.575f, 0.525f, 0.1f, 0.1f, -0.18f)
	, ST("ST", 0.25f, 0.575f, 0.6f, 0.585f, 0.1f, 0.1f, 0.1f)
	, T("P", 0.2f, 0.6f, 0.8f, 0.7f, 0.1f, 0.1f, 0.2)
	, t(1.0f)
	, tStep(0.001)
	, minAmpl(-0.5f)
	, maxAmpl(1.3f)
{
	counts = t / tStep;
	signal = new float[counts];
}

void	ECG::calcFunction()
{
	float	currT = 0;
	for (int i = 0; i < counts; i++)
	{
		if (P.isReady(currT))
			signal[i] = P.calcSignal(currT);
		else if (Q.isReady(currT))
			signal[i] = Q.calcSignal(currT);
		else if (R.isReady(currT))
			signal[i] = R.calcSignal(currT);
		else if (S.isReady(currT))
			signal[i] = S.calcSignal(currT);
		else if (ST.isReady(currT))
			signal[i] = ST.calcSignal(currT);	
		else if (T.isReady(currT))
			signal[i] = T.calcSignal(currT);	
		currT += tStep;
	}
}

float	*ECG::getSignal() const { return (signal); }
int		ECG::getCounts() const { return (counts); }
float	ECG::getMaxAmpl() const { return (maxAmpl); }
float	ECG::getMinAmpl() const { return (minAmpl); }

ECG::~ECG()
{
	delete []signal;
}





