#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"

class DirectShader : public Shader
{
public:
    DirectShader();

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;

    Vector3D Color;
};

#endif // DIRECTSHADER_H