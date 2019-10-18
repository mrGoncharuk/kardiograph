#include "kardiograph.h"

int     main()
{
    t_app   app;
    t_flags flags;

    ft_init_all(&app, &flags);
    ft_mainloop(&app, &flags);
    ft_clean(&app);
}