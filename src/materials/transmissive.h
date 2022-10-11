#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "material.h"

class Transmissive : public Material
{
public:

    virtual Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
        const Vector3D& wi) const;

    bool hasSpecular() const;
    bool hasTransmission() const;
    bool hasDiffuseOrGlossy() const;
    double getIndexOfRefraction() const;
};

#endif // TRANSMISSIVE_H