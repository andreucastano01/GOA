#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader():
    Color(Vector3D(0.1))
{ }

DirectShader::DirectShader(Vector3D bgColor_) :
    Shader(bgColor_)
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
            return Lo;
        }
        //Check if Mirror Material
        else if (its.shape->getMaterial().hasSpecular()) {
            Vector3D wo = -r.d;
            Vector3D wr = its.normal * 2 * dot(its.normal, wo) - wo;
            Ray ray_r(its.itsPoint, wr, r.depth, Epsilon);

            return computeColor(ray_r, objList, lsList);
        }
        //Check if Transmissive Material
        else if (its.shape->getMaterial().hasTransmission()) {
            double refrac = its.shape->getMaterial().getIndexOfRefraction();
            Vector3D n = its.normal;
            if (dot(its.normal, r.d) > 0.0) {
                n = -its.normal;
                refrac = 1 / refrac;
            }
            Vector3D wo = -r.d;
            double win = dot(n, wo);
            double sin2alpha = 1 - pow(win, 2);
            double rad = 1 - pow(refrac, 2) * sin2alpha;
            if (rad < 0) {
                Vector3D wr = n * 2 * dot(n, wo) - wo;
                Ray ray_r(its.itsPoint, wr, r.depth, Epsilon);

                return computeColor(ray_r, objList, lsList);
            }
            else {
                Vector3D ntl = Vector3D(wo.x * refrac, wo.y * refrac, wo.z * refrac);
                double square = (-sqrt(rad) + refrac * win);
                Vector3D t = Vector3D(square * n.x, square * n.y, square * n.z) - ntl;
                Ray ray_refrac(its.itsPoint, t, r.depth, Epsilon);

                return computeColor(ray_refrac, objList, lsList);
            }
        }
    }
    else
        return bgColor;

}