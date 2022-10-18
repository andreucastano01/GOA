#include "transmissive.h"
#include <iostream>

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    return Vector3D(0, 0, 0);
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
    return 0.0;
}

Vector3D Transmissive::getDiffuseCoefficient() const {
    return Vector3D(0.0);
}