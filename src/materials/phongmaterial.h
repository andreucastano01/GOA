#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "material.h"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    PhongMaterial(Vector3D diffuse_, Vector3D specular_, int shininess_);

    virtual Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
        const Vector3D& wi) const;

    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
    double getIndexOfRefraction() const;
    Vector3D getDiffuseCoefficient() const;

private:
    Vector3D diffuse_;
    Vector3D specular_;
    int shininess_;

};

#endif // PHONGMATERIAL_H