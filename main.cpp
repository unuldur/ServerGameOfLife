#include <iostream>
#include "src/Serveur.h"
#include "src/RequestManager.h"

using namespace std;

int main() {
    /*
    Serveur* serveur = new Serveur(12345);
    if( serveur->init() != 0){
        cout << "rater" << endl;
    }
    if( serveur->start() != 0){
        cout << "rater 2" << endl;
    }*/
    RequestManager* request = new RequestManager("GET idG=0 idP=0 x=1 y=3");
    request->manageRequest();

    return 0;
}