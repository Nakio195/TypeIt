#ifdef TYPE_IT_EDITOR
#include "../TypeIt/TypeIt/Scene.h"
#else
#include "Scene.h"
#endif

#include <QJsonArray>

Scene::Id Scene::id = 0;

Scene::Scene(QString Name, Scene* Parent)
{
    mParent = Parent;
    mName = Name;

    mId = id;
    id++;

    mIsRoot = false;
    mAliasId = ID::Unvalid;
    mIsAlias = false;
    mAlias = nullptr;
}

Scene::Scene(const char* Name, Scene *Parent)
{
    mParent = Parent;
    mName = QString(Name);

    mId = id;
    id++;

    mIsRoot = false;
    mAliasId = ID::Unvalid;
    mIsAlias = false;
    mAlias = nullptr;
}

Scene::Scene()
{
    mParent = nullptr;

    mIsRoot = false;
    mAliasId = ID::Unvalid;
    mIsAlias = false;
    mAlias = nullptr;
}

Scene::Scene(Scene& copy)
{
    mId = id;
    id++;

    mIsRoot = copy.mIsRoot;

    mParent = copy.mParent;
    mName = copy.mName;
    mSceneText = copy.mSceneText;
    mEnterDoors = copy.mEnterDoors;

    mParentId = copy.mParentId;

    mChilds = copy.mChilds;
    mChildsId = copy.mChildsId;

    mIsAlias = copy.mIsAlias;
    mAliasId = copy.mAliasId;
    mAlias = copy.mAlias;
}

Scene::Id Scene::getId()
{
    return mId;
}

/**
 * @brief Scene::Play, Joue une scène et gère les interactions avec le joueur tant qu'auune condition de sortie n'est activée
 * @return Retourne la prochaine scène à jouer
 */

Scene* Scene::Play()
{
    bool Continue = true;
    Scene *NextScene = this;

    cout << endl << "_________    " << mName.toStdString() << "    _________" << endl << endl;
    cout << mSceneText.toStdString();

    cout << endl << endl;

    while(Continue)
    {
        std::wstring Input;
        getline(std::wcin, Input);

        NextScene = TestInput(QString::fromStdWString(Input));

        if(NextScene == 0)
            NextScene = this;

        if(NextScene != this)
            Continue = false;
    }

    return NextScene;
}

void Scene::read(QJsonObject &json)
{
    if (json.contains("id") && json["id"].isDouble())
        mId = static_cast<int>(json["id"].toDouble());
    if (json.contains("name") && json["name"].isString())
        mName = json["name"].toString();
    if (json.contains("text") && json["text"].isString())
        mSceneText = json["text"].toString();
    if (json.contains("root") && json["root"].isDouble())
        mIsRoot = true;
    if (json.contains("aliasId") && json["aliasId"].isDouble())
    {
        mAliasId = json["aliasId"].toDouble();
        mIsAlias = true;
    }

    if(json.contains("doors") && json["doors"].isArray())
    {
        QJsonArray doors = json["doors"].toArray();

        for(int i = 0; i < doors.size(); ++i)
        {
            addEntryDoor(doors[i].toString());
        }
    }

    if(json.contains("parent") && json["parent"].isDouble())
        mParentId = static_cast<Scene::Id>(json["parent"].toDouble());

    if(json.contains("childs") && json["childs"].isArray())
    {
        QJsonArray childs = json["childs"].toArray();

        for(int i = 0; i < childs.size(); ++i)
        {
            mChildsId.append(static_cast<Scene::Id>(childs[i].toDouble()));
        }
    }
}

void Scene::write(QJsonObject &json)
{
    json["id"] = static_cast<double>(mId);
    json["name"] = mName;
    json["text"] = mSceneText;

    if(mIsAlias)
        json["aliasId"] = static_cast<double>(mAliasId);

    QJsonArray doors;
    doors = QJsonArray::fromStringList(Scene::doors());
    json["doors"] = doors;

    json["parent"] = static_cast<double>(mParentId);

    QJsonArray childs;
    for(int i = 0; i < mChildsId.size(); ++i)
        childs.append(QJsonValue(static_cast<double>(mChildsId[i])));
    json["childs"] = childs;
}

Scene* Scene::fromJson(QJsonObject &json)
{
    Scene* scene = new Scene();

    scene->read(json);

    return scene;
}

QList<Scene::Id> Scene::childsId()
{
    return mChildsId;
}

Scene::Id Scene::parentId()
{
    return mParentId;
}

Scene* Scene::child(int row)
{
    if(row < 0 || row > mChilds.size())
        return nullptr;
    return mChilds[row];
}

QList<Scene*> Scene::childs()
{
    return mChilds;
}

Scene* Scene::parent()
{
    return mParent;
}

unsigned int Scene::childCount()
{
    return mChilds.count();
}

int Scene::row()
{
    if (mParent != nullptr)
        return mParent->mChilds.indexOf(const_cast<Scene*>(this));

    return 0;
}

int Scene::columnCount()
{
    return 3;
}

QString Scene::data(int col)
{
    if(col == Data::ID)
        return QString::number(mId);
    else if(col == Data::Name)
        return mName;
    else if(col == Data::InText)
        return mSceneText;

    else
        return QString();
}

void Scene::setText(const char* Text)
{
    mSceneText = QString(Text);
}

void Scene::setText(QString Text)
{
    mSceneText = Text;
}

QList<QString> Scene::doors()
{
    return mEnterDoors.toList();
}


Scene* Scene::TestInput(QString Input)
{
    Input = Input.toLower();

    foreach(Scene* scene, mChilds)
    {
        QSet<QString>::iterator it = scene->mEnterDoors.find(Input);

        if(it != scene->mEnterDoors.end())
            return scene;

        if(scene->mName.toLower() == Input)
            return scene;
    }

    if(mParent != 0)
    {
        if(Input == QString("sortie") || Input == QString("sortir") || Input == QString("retour") || Input == mParent->mName.toLower())
            return mParent;
    }

    wcerr << endl << mId << " - " << this->mName.toStdWString() << " : " << Input.toStdWString();

    return 0;
}

void Scene::addEntryDoor(QString Door)
{
    mEnterDoors.insert(Door);
}

void Scene::addEntryDoor(const char* Door)
{
    mEnterDoors.insert(QString(Door));
}

void Scene::setParent(Scene* Parent)
{
    if(Parent != nullptr)
    {
        mParent = Parent;
        mParentId = Parent->mId;
    }

    return;
}

void Scene::attach(Scene *Child, bool asAlias)
{
    if(Child == nullptr)
        return;

    if(asAlias)
    {
        Scene* alias = new Scene(*Child);
        alias->setAlias(Child);
        alias->setParent(this);
        mChildsId.append(alias->mId);
        mChilds.append(alias);
    }

    else
    {
        Child->setParent(this);
        mChildsId.append(Child->mId);
        mChilds.append(Child);
    }
}

void Scene::setAlias(Scene* alias)
{
    if(alias != nullptr)
    {
        mIsAlias = true;
        mAlias = alias;
        mAliasId = alias->getId();
    }
}

bool Scene::isRoot()
{
    return mIsRoot;
}

bool Scene::isAlias()
{
    return mIsAlias;
}

Scene* Scene::alias()
{
    return mAlias;
}

Scene::Id Scene::aliasId()
{
    return mAliasId;
}
