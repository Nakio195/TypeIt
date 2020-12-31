#include <iostream>
#include <string>
#include <fstream>

#include <QString>
#include <QFile>
#include <QJsonDocument>

#include "Book.h"
#include "Scene.h"
using namespace std;

void goVisite();
void answerTo(QString Input);

int main()
{

    std::wofstream ErrorFile("failedDoors.txt", std::fstream::out|std::fstream::app);
    //std::cerr.rdbuf(ErrorFile.rdbuf());
    std::wcerr.rdbuf(ErrorFile.rdbuf());

    bool Continue = true;

    Scene Hall("Hall");
    Scene Toilettes("Toilettes");
    Scene Visite("Hall");
    Scene Visite_PlateauTechnique("Salle des machines");
    Scene Visite_I3D("Imprimante 3D");
    Scene Visite_Laser("Decoupeuse graveuse laser");
    Scene Visite_Fablab("Poster FabLab");
    Scene Shopbot("Salle Shopbot");

    Shopbot.setText("On est dans la rue devant la salle de la Shopbot, de jolies instructions sont affichées sur la porte vitree.");

    Hall.setText("Bienvenue au 8 Fablab ! \n\r Vous voila face a l'interface dernier cri de visite virtuelle ! \n\r Son fonctionnement est simple, vous pouvez vous laisser guider ou alors entrer un mot clef ! On y va ?");
    Hall.addEntryDoor("entrer");

    Visite.addEntryDoor("oui");
    Visite.addEntryDoor("ok");
    Visite.addEntryDoor("yup");
    Visite.setText("C'est parti ! On est dans le hall d'entree. Derriere vous la porte donne sur le plateau technique avec toutes les machines.");

    Visite_PlateauTechnique.addEntryDoor("plateau");
    Visite_PlateauTechnique.addEntryDoor("derriere");
    Visite_PlateauTechnique.addEntryDoor("machine");
    Visite_PlateauTechnique.setText("Nous voilà dans la salle des machines. On y trouve des gens sympas, des machines qui travaillent dans pleins de dimensions. Par quoi on commence ?");

    Visite_I3D.setText("Les imprimantes 3D c'est cool, c'est lent et ca fais plein de plastique !");
    Visite_I3D.addEntryDoor("impression 3d");
    Visite_I3D.addEntryDoor("imprimante 3d");
    Visite_I3D.addEntryDoor("i3d");

    Visite_Fablab.setText("Un FabLab c'est une endroit ou on apprend a faire par soit meme !");
    Visite_Fablab.addEntryDoor("fablab");
    Visite_Fablab.addEntryDoor("fab lab");

    Visite_Laser.setText("Ici c'est la decoupeuse graveuse laser, attention aux yeux !");
    Visite_Laser.addEntryDoor("laser");
    Visite_Laser.addEntryDoor("gravure");
    Visite_Laser.addEntryDoor("gravure laser");

    Toilettes.addEntryDoor("wc");
    Toilettes.addEntryDoor("toilette");
    Toilettes.setText("Merci de laisser cet endroit dans l'etat ou vous l'avez trouve :)");
    Shopbot.attach(&Toilettes, true);
    Hall.attach(&Visite);
    Hall.attach(&Toilettes);
    Hall.attach(&Shopbot);
    Visite.attach(&Visite_PlateauTechnique);
    Visite_PlateauTechnique.attach(&Visite_I3D);
    Visite_PlateauTechnique.attach(&Visite_Laser);
    Visite_PlateauTechnique.attach(&Visite_Fablab);

    Book mBook;
    //mBook.load();
    mBook.addScene(&Hall);
    mBook.addScene(&Toilettes);
    mBook.addScene(&Visite);
    mBook.addScene(&Visite_PlateauTechnique);
    mBook.addScene(&Visite_I3D);
    mBook.addScene(&Visite_Laser);
    mBook.addScene(&Visite_Fablab);
    mBook.addScene(&Shopbot);
    mBook.addScene(Shopbot.childs());
    mBook.save();

    Scene* CurrentScene = mBook.startScene();

    while(Continue)
    {
        CurrentScene = CurrentScene->Play();

        if(CurrentScene == 0)
            Continue = false;
    }

    return 0;
}
