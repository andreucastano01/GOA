#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
    Color(Vector3D(1, 0, 0))
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    Vector3D Lo = Vector3D(0.0);
    if (Utils::getClosestIntersection(r, objList, its)) {
        for (int s = 0; s < lsList.size(); s++) {
            PointLightSource light = lsList[s];
            Vector3D Li = light.getIntensity(its.itsPoint);
            Vector3D ref = its.shape->getMaterial().getReflectance(its.normal, -r.d, light.getPosition() - its.itsPoint);
            Ray rl;
            rl.o = light.getPosition();
            rl.d = its.itsPoint;
            int Vi = 1;
            if (Utils::hasIntersection(rl, objList)) Vi = 0;
            Lo += (Li * ref * Vi);
        }
    }
    return Lo;
}