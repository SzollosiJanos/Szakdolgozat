#include <SDL2/SDL_image.h>
#include "app.h"


void init_app(App *app, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        destroy_app(app);
    }

    app->window = SDL_CreateWindow(
        "Collision Detection: Szöllősi János (BC6X4X)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL || IMG_Init(IMG_INIT_PNG) == 0)
    {
        destroy_app(app);
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        destroy_app(app);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glClearColor((float)130 / 255, (float)202 / 255, (float)250 / 255, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    reshape(width, height);
    SDL_ShowCursor(SDL_DISABLE);
    init_scene(&(app->scene), &(app->camera));
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT)
    {
        w = (int)((double)height * VIEWPORT);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 100);
}

void handle_app_events(App *app)
{
    SDL_Event event;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                destroy_app(app);
                break;
            case SDL_SCANCODE_W:
                app->camera.speed.y = 2.5;
                break;
            case SDL_SCANCODE_S:
                app->camera.speed.y = -2.5;
                break;
            case SDL_SCANCODE_A:
                app->camera.speed.x = 2.5;
                break;
            case SDL_SCANCODE_D:
                app->camera.speed.x = -2.5;
                break;
            case SDL_SCANCODE_SPACE:
                app->camera.speed.z = 2.5;
                break;
            case SDL_SCANCODE_LCTRL:
                app->camera.speed.z = -2.5;
                break;
            case SDL_SCANCODE_E:
                app->scene.model_spawn_toggle = false;
                spawnmodel(&(app->scene), &(app->camera));
                break;
            case SDL_SCANCODE_Q:
                if (app->scene.display_player == true)
                {
                    app->scene.display_player = false;
                    break;
                }
                app->scene.display_player = true;
                break;
            case SDL_SCANCODE_N:
                scale_model(&(app->scene.player.model), 1.02, 1.02, 1.02);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_M:
                scale_model(&(app->scene.player.model), 0.98, 0.98, 0.98);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_I:
                rotate_model(&(app->scene.player.model), 0.0, 10.0, 0.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_K:
                rotate_model(&(app->scene.player.model), 0.0, -10.0, 0.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_J:
                rotate_model(&(app->scene.player.model), 10.0, 0.0, 0.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_L:
                rotate_model(&(app->scene.player.model), -10.0, 0.0, 0.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_U:
                rotate_model(&(app->scene.player.model), 0.0, 0.0, -10.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_O:
                rotate_model(&(app->scene.player.model), 0.0, 0.0, 10.0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_C:
                mirror_model(&(app->scene.player.model),0);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_V:
                mirror_model(&(app->scene.player.model),1);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            case SDL_SCANCODE_B:
                mirror_model(&(app->scene.player.model),2);
                app->scene.player.model.farestpoint = get_distance_modelelement(&(app->scene), -1);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                app->camera.speed.y = 0;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                app->camera.speed.x = 0;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_LCTRL:
                app->camera.speed.z = 0;
                break;
            case SDL_SCANCODE_E:
                app->scene.model_spawn_toggle = true;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (x != WIDTH / 2 || y != HEIGHT / 2)
            {
                SDL_WarpMouseInWindow(app->window, WIDTH / 2, HEIGHT / 2);
            }
            else
            {
                rotate_camera(&(app->camera), (mouse_x - x) * -1, (mouse_y - y) * -1);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0)
            {
                if (app->camera.distance > 1.5)
                {
                    app->camera.distance -= 0.5;
                }
            }
            else
            {
                if (app->camera.distance <= 10)
                {
                    app->camera.distance += 0.5;
                }
            }
            rotate_camera(&(app->camera), 0, 0);
            break;
        case SDL_QUIT:
            destroy_app(app);
            break;
        default:
            break;
        }
    }
}

void render_app(App *app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(app->camera.rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(app->camera.rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-app->camera.position.x, -app->camera.position.y, -app->camera.position.z);
    render_scene(&(app->scene));
    render_player(&(app->scene), &(app->camera));

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app)
{
    app->isrunning = false;
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}