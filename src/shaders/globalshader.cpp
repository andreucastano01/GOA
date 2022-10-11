#include "globalshader.h"
#include "../core/utils.h"

GlobalShader::GlobalShader() :
    Color(Vector3D(1, 0, 0))
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    return bgColor;
}