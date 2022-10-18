#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"

class GlobalShader : public Shader
{
public:
    GlobalShader();
    GlobalShader(Vector3D bgColor_, Vector3D ambient);

    virtual Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lsList) const;


    Vector3D Color;
    Vector3D ambient;
};

#endif // GLOBALSHADER_H