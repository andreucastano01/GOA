#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

Vector3D NormalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        Vector3D color = (its.normal + (1.0, 1.0, 1.0)) / 2;
        return color;
    }
    else
        return bgColor;
}