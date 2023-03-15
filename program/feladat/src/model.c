#include <stdlib.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "model.h"

void init_model(Model* model)
{
    model->i_v = 0;
    model->i_vt = 0;
    model->i_vn = 0;
    model->i_f = 0;
    model->v = NULL;
    model->vt = NULL;
    model->vn = NULL;
    model->f = NULL;
    model->farestpoint=0;
}

void allocate_model(Model* model)
{
    model->v =
        (Vertex*)malloc((model->i_v + 1) * sizeof(Vertex));
    model->vt =
        (VertexTexture*)malloc((model->i_vt + 1) * sizeof(VertexTexture));
    model->vn =
        (Vertex*)malloc((model->i_vn + 1) * sizeof(Vertex));
    model->f =
        (Triangle*)malloc(model->i_f * sizeof(Triangle));
}


void draw_model(const Model* model)
{
    int i, k;
    int vertex_index, texture_index, normal_index;

    glBegin(GL_TRIANGLES);
    
    for (i = 0; i < model->i_f; ++i) {
        for (k = 0; k < 3; ++k) {

            normal_index = model->f[i].points[k].normal_index;
            glNormal3f(model->vn[normal_index].x, model->vn[normal_index].y, model->vn[normal_index].z);
            texture_index = model->f[i].points[k].texture_index;
            glTexCoord2f(model->vt[texture_index].u, 1.0 - model->vt[texture_index].v);
            vertex_index = model->f[i].points[k].vertex_index;
            glVertex3f(model->v[vertex_index].x, model->v[vertex_index].y, model->v[vertex_index].z);
        }
    }

    glEnd();
}


GLuint load_texture(char* filename)
{
    SDL_Surface* surface;
    GLuint texture_name;

    surface = IMG_Load(filename);

    glGenTextures(1, &texture_name);

    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, (Pixel*)(surface->pixels));

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture_name;
}