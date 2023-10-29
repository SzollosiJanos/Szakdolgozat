#include <stdio.h>
#include "app.h"

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    App app;
	argc=argc;argv=argv;			//unused parameter error fix
    init_app(&app, WIDTH, HEIGHT);
    app.isrunning=true;
    double current_time;
    double elapsed_time;
    while (app.isrunning) {
        handle_app_events(&app);
        current_time = (double)SDL_GetTicks() / 1000;
        elapsed_time = current_time - app.uptime;
        app.uptime = current_time;
        update_camera(&(app.camera), elapsed_time,&(app.scene));
        render_app(&app);
    }

    return 0;
}