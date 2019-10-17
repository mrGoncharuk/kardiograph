#ifndef WAVE_HPP
# define WAVE_HPP

# include <math.h>
# include <string>

class Wave
{
private:
	std::string label;
	float		duration;	//	duration of wave
	float		tBegin;	//	time of wave begin
	float		tEnd;		//	time of wave end
	float		tExtreme;		//	point in time when wave takes extreme value(mu)
	float		b1;			//	a parameter that defines symmetry of wave
	float		b2;
	float		amplitude;	//	amplitude of wave
	Wave();
	Wave(std::string label);
public:
	void	setLabel(std::string p_label);
	void	setTBegin(float p);
	void	setTEnd(float p);
	void	setTExtreme(float p);
	void	setB1(float p);
	void	setB2(float p);
	void	setAmplitude(float p);
	bool	isReady(float currT);
	float	calcSignal(float currT);
	Wave(std::string p_label, float p_dur, float p_t1, float p_t2, float p_m, float p_b1, float p_b2, float p_ampl);
	~Wave();
};







#endif
