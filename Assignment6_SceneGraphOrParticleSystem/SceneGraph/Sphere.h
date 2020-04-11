#pragma once
/** @file Sphere.h
 *  @brief Draw a simple sphere primitive.
 *
 *  Draws a simple sphere primitive, that is derived
 *  from the Object class.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#include <cmath>
#include <QtOpenGL>
#include <QtGui>
#include <QtCore>
#include <iostream>


class Sphere {
public:

    // Constructor for the Sphere
    Sphere();
    // The intialization routine for this object.
    void init();

    // Getters for our data.
    QVector<QVector3D> positions() const {return positions_;}
    QVector<QVector3D> normals() const {return normals_;}
    QVector<QVector2D> texCoords() const {return textureCoords_;}
    QVector<unsigned int> indexes() const {return index_;}

private:
    QVector<QVector3D> positions_;
    QVector<QVector3D> normals_;
    QVector<QVector2D> textureCoords_;
    QVector<unsigned int> index_;
};