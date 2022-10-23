#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader():
    ambient(Vector3D(0.1))
{ }

DirectShader::DirectShader(Vector3D bgColor_, Vector3D ambient) :
    Shader(bgColor_), ambient(ambient)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
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
            else { //We have problems here
                Vector3D ntl = Vector3D(wo.x * refrac, wo.y * refrac, wo.z * refrac);
                Vector3D t = cross((-sqrt(rad) + refrac * win), its.normal) - ntl;
                if (dot(its.normal, t) > 0.0) {
                    Vector3D new_n = -its.normal;
                    refrac = 1 / refrac;

                    win = dot(new_n, wo);
                    sin2alpha = 1 - pow(win, 2);
                    rad = 1 - pow(refrac, 2) * sin2alpha;
                    ntl = Vector3D(wo.x * refrac, wo.y * refrac, wo.z * refrac);
                    t = cross((-sqrt(rad) + refrac * win), new_n) - ntl;
                }
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