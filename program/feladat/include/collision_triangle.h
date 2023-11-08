#ifndef COLLISION_TRIANGLES_H
#define COLLISION_TRIANGLES_H

#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <stdbool.h>
#include "model.h"

typedef struct vec3
{
	float x;
	float y;
	float z;
} vec3;

/**
 * Get the lowest number from 3 number
 */
float getmin(float a, float b, float c);

/**
 * Get the biggest number from 3 number
 */
float getmax(float a, float b, float c);

/**
 * Check the variebles if they are separated or not
 */
bool isSeparated(float a1,float a2, float b0, float b1, float b2);

/**
 * Substitute one vector from another vector
 */
vec3 sub(vec3 A, vec3 B);

/**
 * Cross one vector with another vector
 */
vec3 cross(vec3 A, vec3 B);

/**
 * Multiply every number
 */
float dot(vec3 A, vec3 B);

/**
 * Check collision of 2 triangles
 * source:   https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf    section: 4.1  Separation of Triangles
 */
bool checkCollisionTriangle(vec3 A0,vec3 A1,vec3 A2, vec3 B0, vec3 B1, vec3 B2);

/**
 * Check collision for 2 models.
 */
bool check_collision(Model* model1,Model* model2,vec3 model1_position, vec3 model2_position);

/**
 * Help the check_collision function.
 */
bool check_collision_helper(int k, int l, Model* model1, Model* model2,vec3 model1_position, vec3 model2_position);

/**
 * Scale the model.
 */
void scale_model(Model* model,float scalex,float scaley,float scalez);

/**
 * Rotate the model.
 */
void rotate_model(Model* model,float anglex,float angley,float anglez);

/**
 * Rotate the model.
 */
void mirror_model(Model* model,int axis);

/**
 * Calculate distance between 2 point.
 */
float get_distance(vec3 A, vec3 B);

#endif /* COLLISION_TRIANGLES_H */