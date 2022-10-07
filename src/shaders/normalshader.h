#ifndef NORMALSHADER_H
#define NORMALSHADER_H

#include "shader.h"

class NormalShader : public Shader
{
public:
    NormalShader();

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

    Vector3D hitColor;
};

#endif // NORMALSHADER_H
