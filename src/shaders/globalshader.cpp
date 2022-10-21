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
        else if (its.shape->getMaterial().hasSpecular()) {
            Vector3D wo = -r.d;
            Vector3D wr = its.normal * 2 * dot(its.normal, wo) - wo;
            Ray ray_r(its.itsPoint, wr, r.depth + 1, Epsilon);

            return computeColor(ray_r, objList, lsList);
        }
        else if (its.shape->getMaterial().hasTransmission()) {
            Vector3D wo = -r.d;
            double win = dot(its.normal, wo);
            double sin2alpha = 1 - pow(win, 2);
            double refrac = its.shape->getMaterial().getIndexOfRefraction();
            double rad = 1 - pow(refrac, 2) * sin2alpha;
            if (rad < 0) {
                Vector3D wr = its.normal * 2 * dot(its.normal, wo) - wo;
                Ray ray_r(its.itsPoint, wr, r.depth + 1, Epsilon);

                return computeColor(ray_r, objList, lsList);
            }
            else { //Peta aqui
                double ntl = dot(refrac, wo);
                Vector3D t = dot((-sqrt(rad) + refrac * win), its.normal); //Me falta restar el ntl
                Ray ray_refrac(its.itsPoint, t, r.depth + 1);

                return computeColor(ray_refrac, objList, lsList);
            }
        }
        // Once all light sources have been taken into account, return the final result
        Vector3D Li = ambient * its.shape->getMaterial().getDiffuseCoefficient();
        return Lo + Li;
    }
    else
        return bgColor;

}