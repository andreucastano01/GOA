#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"

#define PI 3.1415926535

GlobalShader::GlobalShader() :
    ambient(Vector3D(0.1))
{ }

GlobalShader::GlobalShader(Vector3D ambient) :
    ambient(ambient)
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
            
            //Indirect Illumination (2-bounces)
            /*Vector3D Li(0.0);
            if (r.depth == 0) {
                int max_samples = 4;
                Vector3D sumatorio(0.0);
                for (int i = 0; i < max_samples; i++) {
                    HemisphericalSampler sampler;
                    Vector3D sample = sampler.getSample(its.normal);
                    Ray indirect_ray(its.itsPoint, -sample, r.depth + 1, Epsilon);
                    sumatorio += computeColor(indirect_ray, objList, lsList);
                }
                double factor = (1 / (2 * PI * max_samples));
                Li = Vector3D(factor * sumatorio.x, factor * sumatorio.y, factor * sumatorio.z);
            }
            else if (r.depth > 0) {
                Li = ambient * its.shape->getMaterial().getDiffuseCoefficient();
            }
            return Lo + Li;*/
            
            
            //Indirect Illumination (n-bounces)
            Vector3D Li(0.0);
            int maxdepth = 4;
            if (r.depth == 0) {
                int max_samples = 4;
                Vector3D sumatorio(0.0);
                for (int i = 0; i < max_samples; i++) {
                    HemisphericalSampler sampler;
                    Vector3D sample = sampler.getSample(its.normal);
                    Ray indirect_ray(its.itsPoint, -sample, r.depth + 1, Epsilon);
                    sumatorio += computeColor(indirect_ray, objList, lsList);
                }
                double factor = (1 / (2 * PI * max_samples));
                Li = Vector3D(factor * sumatorio.x, factor * sumatorio.y, factor * sumatorio.z);
            }
            else if (r.depth == maxdepth) {
                Li = ambient * its.shape->getMaterial().getDiffuseCoefficient();
            }
            else if(r.depth > 0 && r.depth < maxdepth){
                Vector3D wo = -r.d;
                //Sample direction: normal at p
                Ray normal_ray(its.itsPoint, its.normal, r.depth + 1, Epsilon);
                //Sample direction: perfect reflection direction
                Vector3D wr = its.normal * 2 * dot(its.normal, wo) - wo;
                Ray perf_ray(its.itsPoint, wr, r.depth + 1, Epsilon);

                Vector3D suma1 = computeColor(normal_ray, objList, lsList);
                Vector3D suma2 = computeColor(perf_ray, objList, lsList);

                Vector3D suma = suma1 + suma2;
                
                double factor = (1 / (4 * PI));
                Li = Vector3D(factor * suma.x, factor * suma.y, factor * suma.z);
            }

            return Lo + Li;
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
                Ray ray_r(its.itsPoint, wr, r.depth + 1, Epsilon);

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
