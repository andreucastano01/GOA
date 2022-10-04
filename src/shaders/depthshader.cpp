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
        int cx = std::max(1.0 - ((its.itsPoint.x - r.o.x) / maxDist), 0.0);
        int cy = std::max(1.0 - ((r.d.length() - r.o.length()) / maxDist), 0.0); //cambiar esto
        int cz = std::max(1.0 - ((its.itsPoint.z - r.o.z) / maxDist), 0.0);
        Vector3D depthColor = Vector3D(0, cy, 0);
        return depthColor;
    }
    else
        return bgColor;
}