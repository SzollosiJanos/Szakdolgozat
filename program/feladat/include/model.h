#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include <GL/gl.h>

/**
 * Three dimensional vertex
 */
typedef struct Vertex
{
    double x;
    double y;
    double z;
} Vertex;

/**
 * Two dimensional texture vertex
 */
typedef struct VertexTexture
{
    double u;
    double v;
} VertexTexture;

/**
 * Point of the face
 */
typedef struct FacePoint
{
    int vertex_index;
    int texture_index;
    int normal_index;
} FacePoint;

/**
 * Triangle as facepoint triplet
 */
typedef struct Triangle
{
    struct FacePoint points[3];
} Triangle;

/**
 * Three dimensional model with texture
 */
typedef struct Model
{
    int i_v;  // vertex
    int i_vt; // vertex textures
    int i_vn; // vertex normals
    int i_f;  // faces
    Vertex *v;
    VertexTexture *vt;
    Vertex *vn;
    Triangle *f;
    float farestpoint;
} Model;

/**
 * Types of the considered elements
 */
typedef enum
{
    NONE,
    VERTEX,
    VERTEX_TEXTURE,
    VERTEX_NORMAL,
    FACE
} ElementType;

typedef GLubyte Pixel[3];


/**
 * Three dimensional vector
 */
typedef struct vec3
{
	float x;
	float y;
	float z;
} vec3;

/**
 * Color with RGB components
 */
typedef struct Color
{
	float red;
	float green;
	float blue;
} Color;

/**
 * Materials
 */
typedef struct Material
{
	struct Color ambient;
	struct Color diffuse;
	struct Color specular;
	float shininess;
} Material;


/**
 * Initialize the model structure.
 */
void init_model(Model *model);

/**
 * Allocate model.
 */
void allocate_model(Model *model);

/**
 * Draw the model.
 */
void draw_model(const Model *model);

/**
 * Load texture from file and returns with the texture name.
 */
GLuint load_texture(char *filename);

#endif /* OBJ_MODEL_H */
