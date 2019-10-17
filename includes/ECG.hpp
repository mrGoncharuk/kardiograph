#ifndef ECG_HPP
# define ECG_HPP

#
# include "Wave.hpp"

class ECG
{
private:
	Wave	P;
	Wave	Q;
	Wave	R;
	Wave	S;
	Wave	ST;
	Wave	T;

	float	t;
	float	tStep;
	float	minAmpl;
	float	maxAmpl;
	float	*signal;
	int		counts;
public:
	ECG(/* args */);
	void	calcFunction();
	float	*getSignal() const;
	int		getCounts() const;
	float	getMaxAmpl() const;
	float	getMinAmpl() const;
	~ECG();
};



#endif
