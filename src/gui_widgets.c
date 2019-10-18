#include "kardiograph.h"

void	gui_create_widgets(float *azubec)
{
	igSliderFloat("Float", azubec, 0.0f, 1.0f, "%.3f", 1.0f);
}