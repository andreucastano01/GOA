#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
    Color(Vector3D(1, 0, 0))
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
        return bgColor;
}