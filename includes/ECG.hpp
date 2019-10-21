#ifndef ECG_HPP
# define ECG_HPP

# include "Wave.hpp"

enum	e_wave_type
{
	P_WAVE,
	Q_WAVE,
	R_WAVE,
	S_WAVE,
	T_WAVE
};

class ECG
{
private:
	Wave	P;
	Wave	Q;
	Wave	R;
	Wave	S;
	Wave	T;

	int		Fh;
	float	t;
	float	minAmpl;
	float	maxAmpl;
	float	*signal;
	int		counts;
public:
	ECG(/* args */);
	void	calcFunction();
	bool	isViewReal();
	bool	isPViewReal();
	bool	isQViewReal();
	bool	isRViewReal();
	bool	isSViewReal();
	bool	isTViewReal();
	float	*getSignal() const;
	int		getCounts() const;
	float	getMaxAmpl() const;
	float	getMinAmpl() const;
	int		getFH() const;
	void	setFH(int newFH);
	Wave	&getP();
	Wave	&getQ();
	Wave	&getR();
	Wave	&getS();
	Wave	&getT();
	~ECG();
};



#endif
