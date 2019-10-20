#ifndef WAVE_HPP
# define WAVE_HPP

# include <math.h>
# include <string>

class Wave
{
private:
	std::string label;
	float		tExtreme;		//	point in time when wave takes extreme value(mu)
	float		b1;			//	a parameter that defines symmetry of wave
	float		b2;
	float		amplitude;	//	amplitude of wave
	float		tBegin;
	float		tEnd;
public:
	Wave();
	Wave(Wave const &cpy);
	Wave &operator =(Wave const &cpy);
	Wave(std::string p_label, float p_ampl, float p_textreme, float p_b1, float p_b2);
	~Wave();
	float	calcSignal(float currT);


	void		setLabel(std::string p_label);
	void		setTExtreme(float p);
	void		setB1(float p);
	void		setB2(float p);
	void		setAmplitude(float p);
	void		setTBegin(float p);
	void		setTEnd(float p);
	std::string getLabel();
	float		&getTExtreme();
	float		&getB1();
	float		&getB2();
	float		&getAmplitude();
	float		&getTBegin();
	float		&getTEnd();
};







#endif
