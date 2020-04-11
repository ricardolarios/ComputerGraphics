#pragma once
#include "SolarSystem.h"
#include <QtGui>
#include <QtOpenGL>

SolarSystem::SolarSystem()
{
	QMatrix4x4 root_t;
	root_t.setToIdentity();
	this->setLocalTransform(root_t);

	SceneNode* sun = new SceneNode("../../sun.ppm");
	QMatrix4x4 sun_t;
	sun_t.setToIdentity();
	sun_t.translate(QVector3D(.5f, .5f, 0.f));
	sun->setLocalTransform(sun_t);
	this->add_child(sun);

	
	//SceneNode* mercury = new SceneNode("../../mercury.ppm");
	//QMatrix4x4 merc_t;
	//merc_t.setToIdentity();
	//merc_t.translate(QVector3D(10, 0, 0));
	//mercury->setLocalTransform(merc_t);
	//sun->add_child(mercury);

	//SceneNode* earth = new SceneNode("../../earth.ppm");
	//QMatrix4x4 earth_t;
	//earth_t.setToIdentity();
	//earth_t.translate(QVector3D(20, 0, 0));
	//earth->setLocalTransform(earth_t);
	//sun->add_child(earth);

	//SceneNode* moon1 = new SceneNode("../../rock.ppm");
	//QMatrix4x4 moon1_t;
	//moon1_t.setToIdentity();
	//moon1_t.translate(QVector3D(20, 5, 0));
	//earth->setLocalTransform(earth_t);
	//earth->add_child(moon1);

	//SceneNode* moon2 = new SceneNode("../../rock.ppm");
	//QMatrix4x4 moon2_t;
	//moon2_t.setToIdentity();
	//moon2_t.translate(QVector3D(10, 5, 0));
	//moon2->setLocalTransform(moon2_t);
	//mercury->add_child(moon2);

	//SceneNode* moon3 = new SceneNode("../../rock.ppm");
	//QMatrix4x4 moon3_t;
	//moon3_t.setToIdentity();
	//moon3_t.translate(QVector3D(10, -5, 0));
	//earth->setLocalTransform(moon3_t);
	//mercury->add_child(moon3);
	
}