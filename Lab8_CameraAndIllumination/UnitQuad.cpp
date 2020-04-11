#include "UnitQuad.h"
#include <iostream>


UnitQuad::UnitQuad() : lightPos_(0.5f, 0.5f, -2.0f), sign_(1.0f)
{
    PointLight light1;
    PointLight light2;
    PointLight light3;

    light1.color = QVector3D(1.0, 1.0, 1.0);
    light1.position = QVector3D(0.5f, 0.5f, -2.0f);
    light1.ambientIntensity = 0.5f;
    light1.specularIntensity = 0.5f;
    light1.constant = 1.0f;
    light1.linear = 0.09f;
    light1.quadratic = 0.032f;
    light1.rot.setToIdentity();
    light1.rot_x = false;
    light1.rot_y = true;
    light1.rot_z = false;

    this->lights_.push_back(light1);

    light2.color = QVector3D(0.0, 1.0, 1.0);
    light2.position = QVector3D(0.5f, 0.5f, -2.0f);
    light2.ambientIntensity = 0.5f;
    light2.specularIntensity = 0.5f;
    light2.constant = 1.0f;
    light2.linear = 0.09f;
    light2.quadratic = 0.032f;
    light2.rot.setToIdentity();
    light2.rot_x = true;
    light2.rot_y = false;
    light2.rot_z = false;

    //this->lights_.push_back(light2);

    light3.color = QVector3D(1.0, 0.0, 0.0);
    light3.position = QVector3D(0.5f, 0.5f, -2.0f);
    light3.ambientIntensity = 0.5f;
    light3.specularIntensity = 0.5f;
    light3.constant = 1.0f;
    light3.linear = 0.09f;
    light3.quadratic = 0.032f;
    light3.rot.setToIdentity();
    light3.rot_x = false;
    light3.rot_y = false;
    light3.rot_z = false;

    //this->lights_.push_back(light3);
}

UnitQuad::~UnitQuad()
{
}

void UnitQuad::init(const QString& textureFile)
{
    // The unit quad goes from 0.0 to 1.0 in each dimension.
    QVector<QVector3D> pos;
    QVector<QVector3D> norm;
    QVector<QVector2D> texCoord;
    QVector<unsigned int> idx;
    // unit positions.
    pos << QVector3D(0.0, 0.0, 0.0);
    pos << QVector3D(1.0, 0.0, 0.0);
    pos << QVector3D(0.0, 1.0, 0.0);
    pos << QVector3D(1.0, 1.0, 0.0);
    // We use normals for shading and lighting
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    // Add in the texcoords
    texCoord << QVector2D(0.0, 0.0);
    texCoord << QVector2D(1.0, 0.0);
    texCoord << QVector2D(0.0, 1.0);
    texCoord << QVector2D(1.0, 1.0);
    idx << 0 << 1 << 2 << 2 << 1 << 3;
    Renderable::init(pos, norm, texCoord, idx, textureFile);
}

void UnitQuad::update(const qint64 msSinceLastFrame)
{
    // This is where we want to maintain our light.
    float secs = (float)msSinceLastFrame / 1000.0f;
    float angle = secs * 180.0f;

    // TODO: FIgure out why this doesn't work with multiple lights...
    // Rotate our light around the scene
    for (int i = 0; i < this->lights_.size(); i++)
    {
        PointLight curr = this->lights_[i];

        this->lights_[i].rot.setToIdentity();

        // Rotate on the axis if enabled for this light.
        if (curr.rot_x)
        {
            this->lights_[i].rot.rotate(angle, 1.0, 0.0, 0.0);
        }
        if (curr.rot_y)
        {
            this->lights_[i].rot.rotate(angle, 0.0, 1.0, 0.0);
        }
        if (curr.rot_z)
        {
            this->lights_[i].rot.rotate(angle, 0.0, 0.0, 1.0);
        }

        //this->lights_[i].rot.rotate(angle, 0.0, 1.0, 0.0);
        QVector3D newPos = curr.rot * curr.position;
        this->lights_[i].position = newPos;

        newPos.setX(0.5);

        std::string curr_light_string = "pointLights[" + std::to_string(i) + ']';

        shader_.bind();
        shader_.setUniformValue((curr_light_string + ".color").c_str(), curr.color.x(), curr.color.y(), curr.color.z());
        shader_.setUniformValue((curr_light_string + ".position").c_str(), newPos);

        shader_.setUniformValue((curr_light_string + ".ambientIntensity").c_str(), curr.ambientIntensity);
        shader_.setUniformValue((curr_light_string + ".specularStrength").c_str(), curr.specularIntensity);
        shader_.setUniformValue((curr_light_string + ".constant").c_str(), curr.constant);
        shader_.setUniformValue((curr_light_string + ".linear").c_str(), curr.linear);
        shader_.setUniformValue((curr_light_string + ".quadratic").c_str(), curr.quadratic);

        shader_.release();
    }

    // Because we aren't doing any occlusion, the lighting on the walls looks
    // super wonky.  Instead, just move the light on the z axis.
    // newPos.setX(0.5);
    // TODO:  Understand how the light gets initialized/setup.

}