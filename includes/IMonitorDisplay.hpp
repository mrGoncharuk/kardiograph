#ifndef IMONITORDISPLAY_HPP
# define IMONITORDISPLAY_HPP

class IMonitorDisplay
{
public:
	virtual ~IMonitorDisplay() = 0;
	virtual void	mainloop() = 0;
	virtual void	events() = 0;
	virtual void	update() = 0;
	virtual void	render() = 0;
};


#endif