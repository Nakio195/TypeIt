#ifndef BOOK_H
#define BOOK_H

#ifdef TYPE_IT_EDITOR
#include "../TypeIt/TypeIt/Scene.h"
#else
#include "Scene.h"
#endif

class Book
{
    public:
        Book();
        bool save();
        bool load(QString Path);
        void addScene(Scene* scene);
        void addScene(QList<Scene*> scenes);

        Scene* startScene();

    private:
        QMap<Scene::Id, Scene*> mScenes;
        Scene::Id mCurrent;
};

#endif // BOOK_H
