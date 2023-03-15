#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "scene.h"
#include "collision_triangle.h"

#define VIEWPORT (16.0 / 9.0)
#define WIDTH 1920
#define HEIGHT 1080

typedef struct App
{
    SDL_Window *window;
    SDL_GLContext gl_context;
    double uptime;
    bool isrunning;
    Camera camera;
    Scene scene;
} App;

/**
 * Initialize the application.
 */
void init_app(App *app, int width, int height);

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Handle the events of the application.
 */
void handle_app_events(App *app);

/**
 * Render the application.
 */
void render_app(App *app);

/**
 * Destroy the application.
 */
void destroy_app(App *app);
