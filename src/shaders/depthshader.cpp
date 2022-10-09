#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0)), maxDist(1000.00)
{ }

DepthShader::DepthShader(Vector3D color_, double maxDist, Vector3D bgColor_) :
    Shader(bgColor_), color(color_), maxDist(maxDist)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        double x = its.itsPoint.x - r.o.x;
        double y = its.itsPoint.y - r.o.y;
        double z = its.itsPoint.z - r.o.z;
        double dist = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        double color = (1.0 - (dist / maxDist));
        Vector3D depthColor = Vector3D(0, color, 0);
        return depthColor;
    }
    else
        return bgColor;
}