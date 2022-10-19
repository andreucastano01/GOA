#include "transmissive.h"
#include <iostream>

Transmissive::Transmissive(double refractive_index, Vector3D diffuse_):
    refractive_index(refractive_index), diffuse_(diffuse_)
{}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    double win = dot(n, wi);
    double sin2alpha = 1 - pow(win, 2);
    double rad = 1 - pow(refractive_index, 2) * sin2alpha;
    if (rad < 0) { //Mirror
        Vector3D wr = n * 2 * win - wi;
        return wr;
    }
    else {
        double temp1 = -sqrt(rad) + refractive_index * win;
        double t = dot(temp1, n) - dot(refractive_index, wi);
        return t;
    }
}

bool Transmissive::hasSpecular() const {
    return false;
}

bool Transmissive::hasTransmission() const {
    return true;
}

bool Transmissive::hasDiffuseOrGlossy() const {
    return false;
}

double Transmissive::getIndexOfRefraction() const {
    return refractive_index;
}

Vector3D Transmissive::getDiffuseCoefficient() const {
    return Vector3D(0.0);
}