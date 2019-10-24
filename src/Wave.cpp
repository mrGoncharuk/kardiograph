#include "Wave.hpp"
#include <iostream>
#include <cstdlib>

Wave::Wave(/* args */)
{
}

Wave::Wave(std::string p_label, float p_ampl, float p_textreme, float p_b1, float p_b2)
		: label(p_label)
		, amplitude(p_ampl)
		, tExtreme(p_textreme)
		, b1(p_b1)
		, b2(p_b2)
		, alternation(0)
		, isAltered(false)
{
	tBegin = tExtreme - 3 * b1;
	tEnd = tExtreme + 3 * b2;
}

Wave::Wave(Wave const &cpy)
{
	*this = cpy;
}

Wave &Wave::operator =(Wave const &cpy)
{
	this->label = cpy.label;
	this->tExtreme = cpy.tExtreme;
	this->amplitude = cpy.amplitude;
	this->b1 = cpy.b1;
	this->b2 = cpy.b2;
	return (*this);
}


// Wave::Wave(std::string p_label, float p_t1, float p_t2, float p_t_extreme, float p_b1, float p_b2, float p_ampl):
// 		label(p_label),
// 		tBegin(p_t1),
// 		tEnd(p_t2),
// 		tExtreme(p_t_extreme),
// 		b1(p_b1),
// 		b2(p_b2),
// 		amplitude(p_ampl)
// {}


float	Wave::calcSignal(float currT)
{
	static float lambda = 1;
	float	currAmpl = amplitude;
	float res;
	
	if (isAltered)
	{
		if (lambda == 1)
			lambda = (1 + alternation * (rand() % 100 / 100.0f) / amplitude);
		else
			lambda = 1;
		currAmpl = currAmpl * lambda;
	}
	if (currT <= tExtreme)
		res = currAmpl * exp(-(pow(currT - tExtreme, 2) / (2 * pow(b1, 2))));
	else
		res = currAmpl * exp(-(pow(currT - tExtreme, 2) / (2 * pow(b2, 2))));
	//std::cout << label << ": " << res << std::endl;
	return (res);
}

Wave::~Wave()
{
}

void	Wave::setLabel(std::string p_label) { label = p_label; }
void	Wave::setTExtreme(float p)
{
	tExtreme = p;
	tBegin = tExtreme - 3 * b1;
	tEnd = tExtreme + 3 * b2;
}
void	Wave::setB1(float p)
{
	b1 = p;
	tBegin = tExtreme - 3 * b1;
}
void	Wave::setB2(float p)
{
	b2 = p;
	tEnd = tExtreme + 3 * b2;
}

void	Wave::setAmplitude(float p) { amplitude = p; }
void	Wave::setTBegin(float p) { tBegin = p; }
void	Wave::setTEnd(float p) { tEnd = p; }
void	Wave::setAlternation(float p) { alternation = p; }
void	Wave::setIsAltered(bool p) { isAltered = p; }

std::string Wave::getLabel() { return label; }
float		&Wave::getTExtreme() { return tExtreme; }
float		&Wave::getB1() { return b1; }
float		&Wave::getB2() { return b2; }
float		&Wave::getAmplitude() { return amplitude; }
float		&Wave::getTBegin() { return tBegin; }
float		&Wave::getTEnd() { return tEnd; }
float		&Wave::getAlternation() { return alternation; }
bool		Wave::getIsAltered() const { return isAltered; }
