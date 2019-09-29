#include "kardiograph.h"

void    ft_mainloop(t_app *app)
{
    bool	quit = false;
	bool	show_window = true;
	ImVec4 clearColor;
	clearColor.x = 0.45f;
	clearColor.y = 0.55f;
	clearColor.z = 0.60f;
	clearColor.w = 1.00f;

    while (!quit)
    {
		SDL_Event e;

    // we need to call SDL_PollEvent to let window rendered, otherwise
    // no window will be shown
        while (SDL_PollEvent(&e) != 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
            quit = true;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(app->window))
            quit = true;
        }

		// start imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(app->window);
		igNewFrame();
		static float f = 0.0f;
		static int counter = 0;

		igBegin("Hello, world!", &show_window, 0);
		igText("This is some useful text");
		igCheckbox("Demo window", &show_window);

		igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 1.0f);
		igColorEdit3("clear color", (float*)&clearColor, 0);

		ImVec2 buttonSize;
		buttonSize.x = 0;
		buttonSize.y = 0;
		if (igButton("Button", buttonSize))
		counter++;
		igSameLine(0.0f, -1.0f);
		igText("counter = %d", counter);

		igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
		igEnd();


			// render
		igRender();
		SDL_GL_MakeCurrent(app->window, app->gl_context);
		glViewport(0, 0, (int)app->io->DisplaySize.x, (int)app->io->DisplaySize.y);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
		SDL_GL_SwapWindow(app->window);
	}
}