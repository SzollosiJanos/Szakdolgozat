#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "scene.h"
#include "load.h"

void init_scene(Scene *scene, Camera *camera)
{
    scene->model_counter = 0;
    scene->models = NULL;
    scene->model_spawn_toggle = true;
    scene->display_player = false;
    load_model(&(scene->player.model), "assets/models/cube.obj");
    // scene->player.texture = load_texture("assets/textures/test.png");
    scene->map.material.ambient.red = 1.0;
    scene->map.material.ambient.green = 1.0;
    scene->map.material.ambient.blue = 1.0;

    scene->map.material.diffuse.red = 1.0;
    scene->map.material.diffuse.green = 1.0;
    scene->map.material.diffuse.blue = 1.0;

    scene->map.material.specular.red = 0.0;
    scene->map.material.specular.green = 0.0;
    scene->map.material.specular.blue = 0.0;
    scene->map.material.shininess = 0.0;
    scene->light = 1.0;
    set_material(&(scene->map.material));
    float ambient_light[] = {scene->light, scene->light, scene->light, 1.0f};
    float diffuse_light[] = {scene->light, scene->light, scene->light, 1.0f};
    float specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float position[] = {0.0f, 0.0f, 10.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    scene->player.model.farestpoint = get_distance_modelelement(scene, -1);
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 0.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
    camera->offset.x = 0.0;
    camera->offset.y = 0.0;
    camera->offset.z = 0.0;
    camera->distance = 2.0;
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void render_scene(Scene *scene)
{
    for (int i = 0; i < scene->model_counter; i++)
    {
        glBindTexture(GL_TEXTURE_2D, scene->models[i].texture);
        glPushMatrix();
        glTranslatef(scene->models[i].position.x, scene->models[i].position.y, scene->models[i].position.z);
        draw_model(&(scene->models[i].model));
        glPopMatrix();
    }
}

void render_player(Scene *scene, Camera *camera)
{

    glPushMatrix();
    glTranslatef(camera->offset.x, camera->offset.y, camera->offset.z); // move player model with the camera

    if (scene->display_player == false)
    {
        glBindTexture(GL_TEXTURE_2D, scene->player.texture);
        draw_model(&(scene->player.model));
    }
    glPopMatrix();
}

void load_new_model(Scene *scene, const char *filename, char *filename2)
{
    if (scene->model_counter == MAX_MODELS)
    {
        return;
    }
    scene->model_counter++;
    if (scene->models == NULL)
    {
        scene->models = (Models *)malloc((scene->model_counter + 1) * sizeof(Models));
    }
    else
    {
        scene->models = (Models *)realloc(scene->models, (scene->model_counter + 1) * sizeof(Models));
    }
    load_model(&(scene->models[scene->model_counter - 1].model), filename);
    scene->models[scene->model_counter - 1].texture = load_texture(filename2);
}

void spawnmodel(Scene *scene, Camera *camera)
{
    if (scene->model_spawn_toggle == true || scene->model_counter >= MAX_MODELS)
    {
        return;
    }
    char models[MAX_MODELS][50];
    char textures[MAX_MODELS][50];
    strcpy(models[0], "assets/models/default_map.obj");
    strcpy(models[1], "assets/models/house.obj");
    strcpy(models[2], "assets/models/cylinder.obj");
    strcpy(models[3], "assets/models/monkey.obj");
    strcpy(models[4], "assets/models/sphere.obj");

    strcpy(textures[0], "assets/textures/test.png");
    strcpy(textures[1], "assets/textures/house.png");
    strcpy(textures[2], "assets/textures/test.png");
    strcpy(textures[3], "assets/textures/test.png");
    strcpy(textures[4], "assets/textures/test.png");
    load_new_model(scene, models[scene->model_counter], textures[scene->model_counter]);
    scene->models[scene->model_counter - 1].position.x = camera->offset.x + 11;
    scene->models[scene->model_counter - 1].position.y = camera->offset.y + 11;
    scene->models[scene->model_counter - 1].position.z = camera->offset.z - 5;
    scene->models[scene->model_counter - 1].model.farestpoint = get_distance_modelelement(scene, scene->model_counter - 1);
}

float get_distance_modelelement(Scene *scene, int i)
{
    vec3 A, B;
    int k = 0;
    B.x = 0.0;
    B.y = 0.0;
    B.z = 0.0;
    float max = 0, temp = 0;
    if (i >= 0)
    {
        while (k < scene->models[i].model.i_f)
        {
            for (int l = 0; l < 3; l++)
            {
                A.x = scene->models[i].model.v[scene->models[i].model.f[k].points[l].vertex_index].x;
                A.y = scene->models[i].model.v[scene->models[i].model.f[k].points[l].vertex_index].y;
                A.z = scene->models[i].model.v[scene->models[i].model.f[k].points[l].vertex_index].z;
                temp = get_distance(A, B);
                if (fmaxf(temp, max) == temp)
                {
                    max = temp;
                }
            }
            k++;
        }
    }
    else
    {
        while (k < scene->player.model.i_f)
        {
            for (int l = 0; l < 3; l++)
            {
                A.x = scene->player.model.v[scene->player.model.f[k].points[l].vertex_index].x;
                A.y = scene->player.model.v[scene->player.model.f[k].points[l].vertex_index].y;
                A.z = scene->player.model.v[scene->player.model.f[k].points[l].vertex_index].z;
                temp = get_distance(A, B);
                if (fmaxf(temp, max) == temp)
                {
                    max = temp;
                }
            }
            k++;
        }
    }

    return max;
}

void update_camera(Camera *camera, double time, Scene *scene)
{
    double angle;
    double side_angle;

    angle = camera->rotation.z * M_PI / 180.0;
    side_angle = (camera->rotation.z + 90.0) * M_PI / 180.0;
    camera->offset.x += cos(angle) * camera->speed.y * time;
    camera->offset.y += sin(angle) * camera->speed.y * time;
    camera->offset.x += cos(side_angle) * camera->speed.x * time;
    camera->offset.y += sin(side_angle) * camera->speed.x * time;
    camera->offset.z += camera->speed.z * time;

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;
    camera->position.z += camera->speed.z * time;

    int i = 0;
    bool okay = false;
    if (scene->display_player == false)
    {
        while (i < scene->model_counter && okay == false)
        {
            if (check_collision(&(scene->models[i].model), &(scene->player.model), scene->models[i].position, camera->offset))
            {
                okay = true;
            }
            i++;
        }
    }
    if (okay == true)
    {
        camera->offset.x -= cos(angle) * camera->speed.y * time;
        camera->offset.y -= sin(angle) * camera->speed.y * time;
        camera->offset.x -= cos(side_angle) * camera->speed.x * time;
        camera->offset.y -= sin(side_angle) * camera->speed.x * time;
        camera->offset.z -= camera->speed.z * time;

        camera->position.x -= cos(angle) * camera->speed.y * time;
        camera->position.y -= sin(angle) * camera->speed.y * time;
        camera->position.x -= cos(side_angle) * camera->speed.x * time;
        camera->position.y -= sin(side_angle) * camera->speed.x * time;
        camera->position.z -= camera->speed.z * time;
    }
}

void rotate_camera(Camera *camera, double horizontal, double vertical)
{
    vec3 temp_position;

    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;
    temp_position.x = camera->position.x;
    temp_position.y = camera->position.y;
    temp_position.z = camera->position.z;
    camera->position.x = camera->offset.x + camera->distance * -cosf(camera->rotation.z * (M_PI / 180)) * cosf(camera->rotation.x * (M_PI / 180));
    camera->position.y = camera->offset.y + camera->distance * -sinf(camera->rotation.z * (M_PI / 180)) * cosf(camera->rotation.x * (M_PI / 180));
    camera->position.z = camera->offset.z + camera->distance * -sinf(camera->rotation.x * (M_PI / 180)) + 1;

    if (camera->rotation.x > 89.0)
    {
        camera->rotation.x = 89.0;
        camera->position.x = temp_position.x;
        camera->position.y = temp_position.y;
        camera->position.z = temp_position.z;
    }

    if (camera->rotation.x < -89.0)
    {
        camera->rotation.x = -89.0;
        camera->position.x = temp_position.x;
        camera->position.y = temp_position.y;
        camera->position.z = temp_position.z;
    }

    if (camera->rotation.z < 0)
    {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0)
    {
        camera->rotation.z -= 360.0;
    }
}