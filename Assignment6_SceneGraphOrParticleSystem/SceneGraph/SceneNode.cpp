#pragma once
#include "SceneNode.h"

// Empty constructor, no renderable, no nothing.
SceneNode::SceneNode()
{
	this->object = nullptr;
	this->parent = nullptr;
	this->children = std::vector<SceneNode*>();

	// TODO: Update what the transforms are.
	this->localTransform.setToIdentity();
	this->worldTransform.setToIdentity();
}

// Constructor for a scene node. Uses the given texture file as the texture for this node.
// We create a sphere in this constructor in order to get the position, normals, etc. and use the given
// texture file to create our Renderable. 
SceneNode::SceneNode(QString textureFile)
{
	Sphere s = Sphere();

	Renderable* obj = new Renderable();
	obj->init(s.positions(), s.normals(), s.texCoords(), s.indexes(), textureFile);

	this->object = obj;
	this->parent = nullptr;
	this->children = std::vector<SceneNode*>();

	// TODO: Update what the transforms are.
	this->localTransform.setToIdentity();
	this->worldTransform.setToIdentity();
}

// Delete all of this Node's children, and this node's renderable object.
SceneNode::~SceneNode()
{
	// Delete our children
	for (int i = 0; i < this->children.size(); i++)
	{
		delete this->children[i];
	}

	// Delete our renderable object.
	delete this->object;
}

// Add given child to this SceneNode's children, updating the given child's parent to be this SceneNode in the process.
void SceneNode::add_child(SceneNode* child)
{
	this->children.push_back(child);
	child->parent = this;
}


void SceneNode::draw(const QMatrix4x4& world, const QMatrix4x4& view, const QMatrix4x4& projection)
{
	if (this->object) this->object->draw(world, view, projection);
}

void SceneNode::update(qint64 ms_since_last_frame)
{
	if (parent)
	{
		this->worldTransform = this->parent->worldTransform * this->localTransform;
	}
	else
	{
		this->worldTransform = this->localTransform;
	}

	for (auto i = this->children.begin(); i != this->children.end(); i++)
	{
		(*i)->update(ms_since_last_frame);
	}
}

void SceneNode::setLocalTransform(QMatrix4x4 transform)
{
	this->localTransform = transform;
}

std::vector<SceneNode*>::const_iterator SceneNode::getChildIteratorStart()
{
	return this->children.begin();
}

std::vector<SceneNode*>::const_iterator SceneNode::getChildIteratorEnd()
{
	return this->children.end();
}