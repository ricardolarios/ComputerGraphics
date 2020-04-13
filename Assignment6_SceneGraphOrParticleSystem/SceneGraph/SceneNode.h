#pragma once
#include <QtGui>
#include <QtOpenGL>
#include "Renderable.h"
#include <vector>
#include "Sphere.h"

class SceneNode
{
    public:
        SceneNode();
        SceneNode(QString textureFile);
        ~SceneNode();

        void add_child(SceneNode* child);

        void draw(const QMatrix4x4& world, const QMatrix4x4& view, const QMatrix4x4& projection);
        void update(qint64 ms_since_last_frame);

        void setLocalTransform(QMatrix4x4 transform);

        std::vector<SceneNode*>::const_iterator getChildIteratorStart();
        std::vector<SceneNode*>::const_iterator getChildIteratorEnd();

    protected:
        // Parent
        SceneNode* parent;
    private:
        // Children holds all a pointer to all of the descendents
        // of a particular SceneNode. A pointer is used because
        // we do not want to hold or make actual copies.
        std::vector<SceneNode*> children;
        // The object stored in the scene graph
        Renderable* object;  // May be nullptr!
        // Each SceneNode nodes locals transform.
        QMatrix4x4 localTransform;
        // We additionally can store the world transform
        QMatrix4x4 worldTransform;

        QString texpath;
};