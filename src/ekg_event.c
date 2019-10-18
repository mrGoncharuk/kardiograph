#include "kardiograph.h"

void    ft_event_handler(t_flags *flags, const t_app *app)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT)
            flags->running = false;
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(app->window))
            flags->running = false;
    }
}
