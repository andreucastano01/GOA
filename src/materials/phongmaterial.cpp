#include "phongmaterial.h"
#include <iostream>

PhongMaterial::PhongMaterial(): 
    diffuse_(Vector3D(0.2, 0.7, 0.3)), specular_(Vector3D(0.2, 0.6, 0.2)), shininess_(50)
{ }

PhongMaterial::PhongMaterial(Vector3D diffuse_, Vector3D specular_, int shininess_):
    diffuse_(diffuse_), specular_(specular_), shininess_(shininess_)
{}

Vector3D PhongMaterial::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const
{
    Vector3D wr = (Vector3D(2.0)*(n * wi)) * n - wi;
    Vector3D wowr = wo * wr;

    for (int i = 0; i < shininess_; i++) {
        wowr = wowr * wowr;
    }

    Vector3D r = diffuse_ * (wi * n) + specular_ * wowr;

    return r;
}

bool PhongMaterial::hasSpecular() const {
    return false;
}

bool PhongMaterial::hasTransmission() const {
    return false;
}

bool PhongMaterial::hasDiffuseOrGlossy() const {
    return false;
}

double PhongMaterial::getIndexOfRefraction() const {
    return 0.0;
}