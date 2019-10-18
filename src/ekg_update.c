#include "kardiograph.h"

void	put_sliders(t_app *app)
{
	int		i;
	char	name[3];

	name[0] = 'S';
	name[2] = '\0';
	i = 0;
	while (i < EKG_AMOUNT)
	{
		name[1] = '0' + i;
		igSliderFloat(name, (app->ekgs + i), 0.0f, 1.0f, "%.3f", 1.0f);
	}
}

void	ft_update(t_app *app, t_flags *flags)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window);
	igNewFrame();
	igBegin("Hello, world!", &(flags->ig_window_shown), 0);
	igText("This is some useful text");
	put_sliders(app);
	igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
	igEnd();
}