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
	int		Fh;
	int		t;
	float	minAmpl;
	float	maxAmpl;
	float	*etalon;
	float	*signal;
	int		counts;
	int		cycleAmount;
	float	noisePower;
	Wave	P;
	Wave	Q;
	Wave	R;
	Wave	S;
	Wave	T;
public:
	ECG(/* args */);
	void	generateSignal();
	void	calcEtalon();
	bool	isViewReal();
	bool	isPViewReal();
	bool	isQViewReal();
	bool	isRViewReal();
	bool	isSViewReal();
	bool	isTViewReal();
	float	*getEtalon() const;
	float	*getSignal() const;
	int		getCycleAmount() const;
	int		getEtalonDuration() const;
	float	getMaxAmpl() const;
	float	getMinAmpl() const;
	int		getSignalDuration() const;
	int		getFH() const;
	float	getNoisePower() const;

	void	setNoisePower(float p);
	void	setFH(int newFH);
	void	setCycleAmount(const int amount);
	Wave	&getP();
	Wave	&getQ();
	Wave	&getR();
	Wave	&getS();
	Wave	&getT();
	~ECG();
};



#endif
