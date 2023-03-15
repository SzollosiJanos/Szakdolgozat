#ifndef SCENE_H
#define SCENE_H
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <stdbool.h>
#include "model.h"


#define MAX_MODELS 6

/**
 * Camera, as a moving point with direction
 */
typedef struct Camera
{
	vec3 position;
	vec3 rotation;
	vec3 speed;
	float distance;
	vec3 offset;
} Camera;

/**
 * Struct for map elements.
 */
typedef struct Map
{
	Model model;
	Material material;
	GLuint texture;
} Map;

/**
 * Struct for player elements.
 */

typedef struct Player
{
	Model model;
	GLuint texture;
} Player;

/**
 * Dynamic array of models
 */

typedef struct Models
{
	Model model;
	GLuint texture;
	vec3 position;
} Models;

/**
 * Default scene: structure structures
 */
typedef struct Scene
{
	Map map;
	Player player;
	Models *models;
	float light;
	int model_counter;
	bool model_spawn_toggle;
	bool display_player;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene, Camera *camera);

/**
 * Set the current materials[0].
 */
void set_material(const Material *material);

/**
 * Render the scene objects.
 */
void render_scene(Scene *scene);

/**
 * Load OBJ model from file into a struct and load texture for the model.
 */
void load_new_model(Scene *scene, const char *filename, char *filename2);

/**
 * Spawn a random model to test the collisions.
 */
void spawnmodel(Scene *scene, Camera *camera);

/**
 * Render the player model.
 */
void render_player(Scene *scene, Camera *camera);

/**
 * Calculate distance of the farest element of the model.
 */
float get_distance_modelelement(Scene *scene, int i);

/**
 * Update the position of the camera.
 */
void update_camera(Camera *camera, double time, Scene *scene);

/**
 * Set the horizontal and vertical rotation of the view angle.
 */
void rotate_camera(Camera *camera, double horizontal, double vertical);

#endif /* SCENE_H */