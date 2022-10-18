#include "globalshader.h"
#include "../core/utils.h"

GlobalShader::GlobalShader() :
    ambient(Vector3D(0.1))
{ }

GlobalShader::GlobalShader(Vector3D bgColor_, Vector3D ambient) :
    Shader(bgColor_), ambient(ambient)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    // Get the closest Intersection along the ray
    Intersection its; // Auxiliar structure to store information about the intersection, in case there is one
    if (Utils::getClosestIntersection(r, objList, its))
    {
        Vector3D Lo(0.0);
        //Check if Phong Material
        if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
            for (auto const& light : lsList) {
                Vector3D P_L = light.getPosition() - its.itsPoint; //Vector from intersection point to lightsource
                Vector3D wi = P_L.normalized(); //Normalized Vector wi
                Ray ray_visibility(its.itsPoint, wi, 0, Epsilon, P_L.length());
                if (Utils::hasIntersection(ray_visibility, objList))
                    continue;
                Lo += light.getIntensity(its.itsPoint) * its.shape->getMaterial().getReflectance(its.normal, -r.d, wi) * dot(its.normal, wi);
            }
        }
        // Once all light sources have been taken into account, return the final result
        Vector3D Li = ambient * its.shape->getMaterial().getDiffuseCoefficient();
        return Lo + Li;
    }
    else
        return bgColor;

}