#include <stdlib.h>
#include "load.h"

#define LINE_BUFFER_SIZE 1024

int load_model(Model *model, const char *filename)
{
    FILE *obj_file;

    obj_file = fopen(filename, "r");
    if (obj_file == NULL)
    {
        printf("ERROR with opening file: '%s'\n", filename);
        return 0;
    }
    count_elements(model, obj_file);
    allocate_model(model);
    fseek(obj_file, 0, SEEK_SET);
    if (read_elements(model, obj_file) == 0)
    {
        return 0;
    }
    return 1;
}

void count_elements(Model *model, FILE *file)
{
    char line[LINE_BUFFER_SIZE];

    init_model(model);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL)
    {
        switch (calc_element_type(line))
        {
        case NONE:
            break;
        case VERTEX:
            model->i_v++;
            break;
        case VERTEX_TEXTURE:
            model->i_vt++;
            break;
        case VERTEX_NORMAL:
            model->i_vn++;
            break;
        case FACE:
            model->i_f++;
            break;
        }
    }
}

int read_elements(Model *model, FILE *file)
{
    char line[LINE_BUFFER_SIZE];
    int vertex_index = 1;
    int texture_index = 1;
    int normal_index = 1;
    int triangle_index = 0;
    allocate_model(model);

    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL)
    {
        switch (calc_element_type(line))
        {
        case NONE:
            break;
        case VERTEX:
            if (read_vertex(&(model->v[vertex_index]), line) == 0)
            {
                return 0;
            }
            vertex_index++;
            break;
        case VERTEX_TEXTURE:
            if (read_texture_vertex(&(model->vt[texture_index]), line) == 0)
            {
                return 0;
            }
            texture_index++;
            break;
        case VERTEX_NORMAL:
            if (read_normal(&(model->vn[normal_index]), line) == 0)
            {
                return 0;
            }
            normal_index++;
            break;
        case FACE:
            if (read_triangle(&(model->f[triangle_index]), line) == 0)
            {
                return 0;
            }
            triangle_index++;
            break;
        }
    }
    return 1;
}

ElementType calc_element_type(const char *text)
{
    int i = 0;

    while (text[i] != 0)
    {
        if (text[i] == 'v')
        {
            if (text[i + 1] == 't')
            {
                return VERTEX_TEXTURE;
            }
            else if (text[i + 1] == 'n')
            {
                return VERTEX_NORMAL;
            }
            return VERTEX;
        }
        else if (text[i] == 'f')
        {
            return FACE;
        }
        else if (text[i] != ' ' && text[i] != '\t')
        {
            return NONE;
        }
        i++;
    }
    return NONE;
}

int read_vertex(Vertex *vertex, const char *text)
{
    int i = 0;

    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    vertex->x = atof(&text[i]);
    while (text[i] != 0 && text[i] != ' ')
    {
        i++;
    }
    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    vertex->y = atof(&text[i]);
    while (text[i] != 0 && text[i] != ' ')
    {
        i++;
    }
    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    vertex->z = atof(&text[i]);
    return 1;
}

int read_texture_vertex(VertexTexture *texture_vertex, const char *text)
{
    int i = 0;

    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    texture_vertex->u = atof(&text[i]);
    while (text[i] != 0 && text[i] != ' ')
    {
        i++;
    }
    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    texture_vertex->v = atof(&text[i]);
    return 1;
}

int read_normal(Vertex *normal, const char *text)
{
    int i = 0;

    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    normal->x = atof(&text[i]);
    while (text[i] != 0 && text[i] != ' ')
    {
        i++;
    }
    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    normal->y = atof(&text[i]);
    while (text[i] != 0 && text[i] != ' ')
    {
        i++;
    }
    while (text[i] != 0 && is_numeric(text[i]) == 0)
    {
        i++;
    }
    if (text[i] == 0)
    {
        return 0;
    }
    normal->z = atof(&text[i]);
    return 1;
}

int read_triangle(Triangle *triangle, const char *text)
{
    int point_index;
    int i = 0;
    for (point_index = 0; point_index < 3; point_index++)
    {
        while (text[i] != 0 && is_numeric(text[i]) == 0)
        {
            i++;
        }
        if (text[i] == 0)
        {
            return 0;
        }
        triangle->points[point_index].vertex_index = atoi(&text[i]);
        while (text[i] != 0 && text[i] != '/')
        {
            i++;
        }
        i++;
        if (text[i] == 0)
        {
            return 0;
        }
        triangle->points[point_index].texture_index = atoi(&text[i]);
        while (text[i] != 0 && text[i] != '/')
        {
            i++;
        }
        i++;
        if (text[i] == 0)
        {
            return 0;
        }
        triangle->points[point_index].normal_index = atoi(&text[i]);
        while (text[i] != 0 && text[i] != ' ')
        {
            i++;
        }
    }
    return 1;
}

int is_numeric(char c)
{
    if ((c >= '0' && c <= '9') || c == '-' || c == '.')
    {
        return 1;
    }
    return 0;
}
