#include "Wave.hpp"


Wave::Wave(/* args */)
{
}

Wave::Wave(std::string p_label, float p_dur, float p_t1, float p_t2, float p_t_extreme, float p_b1, float p_b2, float p_ampl):
		label(p_label),
		duration(p_dur),
		tBegin(p_t1),
		tEnd(p_t2),
		tExtreme(p_t_extreme),
		b1(p_b1),
		b2(p_b2),
		amplitude(p_ampl)
{
}

bool	Wave::isReady(float currT)
{
	if (currT >= tBegin && currT < tEnd)
		return (true);
	return (false);
}

float	Wave::calcSignal(float currT)
{
	float res;

	res = amplitude * exp(pow(currT - tExtreme, 2) / (2 * pow(b1, 2)));
	return (res);
}

Wave::~Wave()
{
}
