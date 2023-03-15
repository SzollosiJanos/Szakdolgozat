#include <math.h>
#include "collision_triangle.h"

float getmin(float a, float b, float c)
{
    return fminf(fminf(a, b), c);
}

float getmax(float a, float b, float c)
{
    return fmaxf(fmaxf(a, b), c);
}

bool isSeparated(float a1, float a2, float b0, float b1, float b2)
{
    float a0 = 0;
    if (fmaxf(getmin(a0, a1, a2), getmax(b0, b1, b2)) == getmin(a0, a1, a2))
    {
        return true;
    }
    if (fmaxf(getmax(a0, a1, a2), getmin(b0, b1, b2)) == getmin(b0, b1, b2))
    {
        return true;
    }
    return false;
}

vec3 sub(vec3 A, vec3 B)
{
    vec3 C;
    C.x = A.x - B.x;
    C.y = A.y - B.y;
    C.z = A.z - B.z;
    return C;
}

vec3 cross(vec3 A, vec3 B)
{
    vec3 C;
    C.x = A.y * B.z - A.z * B.y;
    C.y = -(A.x * B.z - A.z * B.x);
    C.z = A.x * B.y - A.y * B.x;
    return C;
}

float dot(vec3 A, vec3 B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

// source:   https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf    section: 4.1  Separation of Triangles

bool checkCollisionTriangle(vec3 A0, vec3 A1, vec3 A2, vec3 B0, vec3 B1, vec3 B2)
{
    /*  A-B f points in the 3rd dimension
        C-E edges of the f
        D-F normal
        G   move the triangles
    */
    vec3 C0 = sub(A1, A0);
    vec3 C1 = sub(A2, A0);
    vec3 C2 = sub(C1, C0);
    vec3 D = cross(C0, C1);
    vec3 E0 = sub(B1, B0);
    vec3 E1 = sub(B2, B0);
    vec3 E2 = sub(E1, E0);
    vec3 F = cross(E0, E1);
    vec3 G = sub(B0, A0);
    float h1, i0, i1, i2;

    // Separate: D
    i0 = dot(D, G);
    i1 = i0 + dot(D, E0);
    i2 = i0 + dot(D, E1);
    if (isSeparated(0, 0, i0, i1, i2))
    {
        return false;
    }

    // Separate: G
    i0 = dot(F, G);
    if (isSeparated(dot(F, C0), dot(F, C1), i0, i0, i0))
    {
        return false;
    }

    // Separate: C0 x E0
    i0 = dot(cross(C0, E0), G);
    if (isSeparated(0, -(dot(D, E0)), i0, i0, i0 + dot(F, C0)))
    {
        return false;
    }

    // Separate: C0 x E1
    i0 = dot(cross(C0, E1), G);
    if (isSeparated(0, -(dot(D, E1)), i0, i0 - dot(F, C0), i0))
    {
        return false;
    }

    // Separate: C0 x E2
    i0 = dot(cross(C0, E2), G);
    i1 = i0 - dot(F, C0);
    if (isSeparated(0, -(dot(D, E2)), i0, i1, i1))
    {
        return false;
    }

    // Separate: C1 x E0
    i0 = dot(cross(C1, E0), G);
    if (isSeparated(dot(D, E0), 0, i0, i0, i0 + dot(F, C1)))
    {
        return false;
    }

    // Separate: C1 x E1
    i0 = dot(cross(C1, E1), G);
    if (isSeparated(dot(D, E1), 0, i0, i0 - dot(F, C1), i0))
    {
        return false;
    }

    // Separate: C1 x E2
    i0 = dot(cross(C1, E2), G);
    i1 = i0 - dot(F, C1);
    if (isSeparated(dot(D, E2), 0, i0, i1, i1))
    {
        return false;
    }

    // Separate: C2 x E0
    h1 = dot(D, E0);
    i0 = dot(cross(C2, E0), G);
    if (isSeparated(h1, h1, i0, i0, i0 + dot(F, C2)))
    {
        return false;
    }

    // Separate: C2 x E1
    h1 = dot(D, E1);
    i0 = dot(cross(C2, E1), G);
    if (isSeparated(h1, h1, i0, i0 - dot(F, C2), i0))
    {
        return false;
    }

    // Separate: C2 x E2
    h1 = dot(D, E2);
    i0 = dot(cross(C2, E2), G);
    i1 = i0 - dot(F, C2);
    if (isSeparated(h1, h1, i0, i1, i1))
    {
        return false;
    }

    return true;
}

bool check_collision(Model *model1, Model *model2, vec3 model1_position, vec3 model2_position)
{
    int k = 0, l = 0;
    float limit, distance;
    limit = model1->farestpoint + model2->farestpoint + 0.5;
    distance = get_distance(model1_position, model2_position);
    if (fminf(distance, limit) == limit)
    {
        return false;
    }
    while (k < model1->i_f)
    {
        while (l < model2->i_f)
        {
            if (check_collision_helper(k, l, model1, model2, model1_position, model2_position))
            {
                return true;
            }
            l++;
        }
        k++;
        l = 0;
    }
    return false;
}

bool check_collision_helper(int k, int l, Model *model1, Model *model2, vec3 model1_position, vec3 model2_position)
{
    vec3 A0, A1, A2, B0, B1, B2;

    A0.x = model1->v[model1->f[k].points[0].vertex_index].x + model1_position.x;
    A0.y = model1->v[model1->f[k].points[0].vertex_index].y + model1_position.y;
    A0.z = model1->v[model1->f[k].points[0].vertex_index].z + model1_position.z;
    A1.x = model1->v[model1->f[k].points[1].vertex_index].x + model1_position.x;
    A1.y = model1->v[model1->f[k].points[1].vertex_index].y + model1_position.y;
    A1.z = model1->v[model1->f[k].points[1].vertex_index].z + model1_position.z;
    A2.x = model1->v[model1->f[k].points[2].vertex_index].x + model1_position.x;
    A2.y = model1->v[model1->f[k].points[2].vertex_index].y + model1_position.y;
    A2.z = model1->v[model1->f[k].points[2].vertex_index].z + model1_position.z;

    B0.x = model2->v[model2->f[l].points[0].vertex_index].x + model2_position.x;
    B0.y = model2->v[model2->f[l].points[0].vertex_index].y + model2_position.y;
    B0.z = model2->v[model2->f[l].points[0].vertex_index].z + model2_position.z;
    B1.x = model2->v[model2->f[l].points[1].vertex_index].x + model2_position.x;
    B1.y = model2->v[model2->f[l].points[1].vertex_index].y + model2_position.y;
    B1.z = model2->v[model2->f[l].points[1].vertex_index].z + model2_position.z;
    B2.x = model2->v[model2->f[l].points[2].vertex_index].x + model2_position.x;
    B2.y = model2->v[model2->f[l].points[2].vertex_index].y + model2_position.y;
    B2.z = model2->v[model2->f[l].points[2].vertex_index].z + model2_position.z;

    return checkCollisionTriangle(A0, A1, A2, B0, B1, B2);
}

void scale_model(Model *model, float scalex, float scaley, float scalez)
{
    int k = 0;
    while (k < model->i_f)
    {
        for (int i = 0; i < 3; i++)
        {
            model->v[k].x *= scalex;
            model->v[k].y *= scaley;
            model->v[k].z *= scalez;
        }

        k++;
    }
}

void rotate_model(Model *model, float anglex, float angley, float anglez)
{
    int k = 0;
    float cord1, cord2, angle;
    while (k < model->i_f)
    {
        cord1 = model->v[k].x;
        cord2 = model->v[k].y;
        angle = anglex * (float)(M_PI / 180);
        model->v[k].x = cord1 * cosf(angle) - cord2 * sinf(angle);
        model->v[k].y = cord1 * sinf(angle) + cord2 * cosf(angle);

        cord1 = model->v[k].x;
        cord2 = model->v[k].z;
        angle = angley * (float)(M_PI / 180);
        model->v[k].x = cord1 * cosf(angle) - cord2 * sinf(angle);
        model->v[k].z = cord1 * sinf(angle) + cord2 * cosf(angle);

        cord1 = model->v[k].y;
        cord2 = model->v[k].z;
        angle = anglez * (float)(M_PI / 180);
        model->v[k].y = cord1 * cosf(angle) - cord2 * sinf(angle);
        model->v[k].z = cord1 * sinf(angle) + cord2 * cosf(angle);
        k++;
    }
}

void mirror_model(Model *model, int axis)
{
    int k = 0;
    while (k < model->i_f)
    {
        if (axis == 0)
        {
            model->v[k].x *= -1;
            model->v[k].y *= -1;
        }
        else if (axis == 1)
        {
            model->v[k].x *= -1;
            model->v[k].z *= -1;
        }
        else
        {
            model->v[k].y *= -1;
            model->v[k].z *= -1;
        }

        k++;
    }
}

float get_distance(vec3 A, vec3 B)
{
    return (sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2) + pow(B.z - A.z, 2)));
}