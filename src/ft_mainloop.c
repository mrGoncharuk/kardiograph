#include "kardiograph.h"




void	graph_test()
{
	float	*values;

	values = (float *)malloc(sizeof(float) * 10);
	for (int i = 0; i < 10; i++)
	{
		values[i] = i;
	}

	igPlotLines("EKG", values, 10, 5, "Overlay text", -10, 10, (ImVec2){0, 200}, sizeof(float));
	free(values);
}

void	ig_handler(t_app *app, t_flags *flags)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window);
	igNewFrame();

	igBegin("Hello, world!", &(flags->ig_window_shown), 0);
	igSliderFloat("Float1", app->ekgs + 1, 0.0f, 1.0f, "%.3f", 1.0f);
	igSliderFloat("Float2", app->ekgs + 2, 0.0f, 1.0f, "%.3f", 1.0f);
	graph_test();
	igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
	igEnd();
}

void    ft_mainloop(t_app *app, t_flags *flags)
{
    while (flags->running)
    {
		ft_event_handler(flags, app);

		// start imgui frame
		ig_handler(app, flags);
		ft_render(app);
	}
}