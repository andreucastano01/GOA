#include "intersectionshader.h"
#include "depthshader.h"
#include "../core/utils.h"

IntersectionShader::IntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

IntersectionShader::IntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D IntersectionShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
    if (Utils::hasIntersection(r, objList))
        return hitColor;
    else
        return bgColor;
}

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0)), maxDist(1000.00)
{ }

DepthShader::DepthShader(Vector3D color_, double maxDist, Vector3D bgColor_) :
    Shader(bgColor_), color(color_), maxDist(maxDist)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{   
    Utils::hasIntersection(r, objList);
    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its)) {
        int cx = std::max(1 - ((its.itsPoint.x - r.o.x) / maxDist), 0.0);
        int cy = std::max(1 - ((its.itsPoint.y - r.o.y) / maxDist), 0.0);
        int cz = std::max(1 - ((its.itsPoint.z - r.o.z) / maxDist), 0.0);
        Vector3D depthColor = Vector3D(cx, cy, cz);
        return depthColor;
    }
    else
        return bgColor;
}