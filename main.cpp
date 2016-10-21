#include <iostream>
#include "src/Serveur.h"

using namespace std;

int main() {
    Serveur* serveur = new Serveur(12345);
    if( serveur->init() != 0){
        cout << "rater" << endl;
    }
    if( serveur->start() != 0){
        cout << "rater 2" << endl;
    }
    return 0;
}