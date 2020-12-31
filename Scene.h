#ifndef SCENE_H
#define SCENE_H

#include <QString>
#include <QMap>
#include <QSet>
#include <iostream>

#include <QJsonObject>

using namespace std;

class Scene
{
    public:
        typedef unsigned long Id;
        static Scene::Id id;
        QString mName;
        QString mSceneText;

        enum Data{ID, Name, InText};
        enum ID{Unvalid = (Scene::Id)(~0)};

    public:
        Scene(QString Name, Scene *Parent = 0);
        Scene(const char* Name, Scene *Parent = 0);

        Scene* Play();

        QList<QString> doors();

        void addEntryDoor(QString Door);
        void addEntryDoor(const char* Door);
        void attach(Scene* Child, bool asAlias = false);

        void setParent(Scene* Parent);

        void setText(const char* Text);
        void setText(QString Text);

        void read(QJsonObject &json);
        void write(QJsonObject &json);
        static Scene* fromJson(QJsonObject &json);

        Scene::Id getId();

        QList<Scene::Id> childsId();
        Scene::Id parentId();

        QList<Scene*> childs();
        Scene* child(int row);
        Scene* parent();


        int row();
        int columnCount();
        QString data(int col);
        unsigned int childCount();

        bool isRoot();

        bool isAlias();
        Scene* alias();
        Scene::Id aliasId();
        void setAlias(Scene* alias);

    private:
        Scene();
        Scene(Scene& copy);

        Scene::Id mId;
        QSet<QString> mEnterDoors;   // id de la scene cible et texte valable en entrée

        Scene *mParent;
        Scene::Id mParentId;

        QList<Scene*> mChilds;
        QList<Scene::Id> mChildsId;

        Scene* TestInput(QString Input);

        bool mIsRoot;

        bool mIsAlias;
        Scene::Id mAliasId;
        Scene* mAlias;

};

#endif // SCENE_H
