#include "depthintersectionshader.h"
#include "../core/utils.h"

DepthIntersectionShader::DepthIntersectionShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

DepthIntersectionShader::DepthIntersectionShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DepthIntersectionShader::depthcomputeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList, Intersection& its) const
{
    if (Utils::getClosestIntersection(r, objList, its)) {
        int cx = std::max(1 - ((its.itsPoint.x - r.o.x) / r.maxT), 0.0);
        int cy = std::max(1 - ((its.itsPoint.y - r.o.y) / r.maxT), 0.0);
        int cz = std::max(1 - ((its.itsPoint.z - r.o.z) / r.maxT), 0.0);
        Vector3D depthColor = Vector3D(cx, cy, cz);
        return hitColor;
    }
    else
        return bgColor;
}
