#ifdef TYPE_IT_EDITOR
#include "../TypeIt/TypeIt/Book.h"
#else
#include "Book.h"
#endif

#include <QFile>
#include <iostream>
#include <QJsonDocument>
#include <QJsonArray>

Book::Book()
{
    
}

void Book::addScene(Scene* scene)
{
    mScenes.insert(scene->getId(), scene);
}

void Book::addScene(QList<Scene*> scenes)
{
    for(Scene* scene : scenes)
        mScenes.insert(scene->getId(), scene);
}

Scene* Book::startScene()
{
    for(Scene* scene : mScenes)
    {
        if(scene->isRoot())
            return scene;
    }

    return mScenes[0];
}

bool Book::load(QString path)
{
    QFile saveFile(path);

    if(!saveFile.open(QIODevice::ReadOnly))
    {
        std::cerr << "Impossible d'ouvrir le fichier de sauvegarde..." << endl;
        return false;
    }

    QByteArray data = saveFile.readAll();

    QJsonDocument saveDoc = QJsonDocument::fromJson(data);
    QJsonObject main = saveDoc.object();

    if(main.contains("scenes") && main["scenes"].isArray())
    {
        QJsonArray scenes = main["scenes"].toArray();

        for(int i = 0; i < scenes.size(); ++i)
        {
            QJsonObject o = scenes[i].toObject();
            Scene* scene = Scene::fromJson(o);
            addScene(scene);
        }
    }

    for(Scene* scene : mScenes)
    {
        for(Scene::Id id : scene->childsId())
        {
            scene->attach(mScenes[id], mScenes[id]->isAlias());
            scene->setParent(mScenes[scene->parentId()]);
        }
    }
}

bool Book::save()
{
    QFile saveFile("save.json");

    if(!saveFile.open(QIODevice::ReadWrite))
    {
        std::cerr << "Impossible d'ouvrir le fichier de sauvegarde..." << endl;
        return false;
    }

    QJsonObject save;
    QJsonArray scenes;

    for(Scene* scene : mScenes)
    {
        QJsonObject json;
        scene->write(json);
        scenes.append(json);
    }

    save["scenes"] = scenes;
    saveFile.write(QJsonDocument(save).toJson());

    return true;
}
