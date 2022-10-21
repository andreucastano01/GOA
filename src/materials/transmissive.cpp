#include "transmissive.h"
#include <iostream>

Transmissive::Transmissive(double refractive_index, Vector3D diffuse_):
    refractive_index(refractive_index), diffuse_(diffuse_)
{}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    return Vector3D(1.0);
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