#pragma once

#include "Renderable.h"
#include <QtGui/QVector3D>
#include <QVector>

// Define our light(s)
struct PointLight
{
    QVector3D color;
    QVector3D position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;

    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;

    QMatrix4x4 rot;
    // i think this should determine what axis to go across
    bool rot_x;
    bool rot_y;
    bool rot_z;
};


class UnitQuad : public Renderable
{
protected:
	QVector3D lightPos_;
    QVector<PointLight> lights_;
	float sign_;
public:
	UnitQuad();
	virtual ~UnitQuad();

	// Our init method is much easier now.  We only need a texture!
	virtual void init(const QString& textureFile);
	virtual void update(const qint64 msSinceLastFrame) override;

private:

};