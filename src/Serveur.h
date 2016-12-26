//
// Created by PAYS on 18/10/2016.
//

#ifndef SERVERGAMEOFLIFE_SERVEUR_H
#define SERVERGAMEOFLIFE_SERVEUR_H

#include <iostream>
#include <winsock2.h>
#include <map>
#include "../GamingModeGameOfLife/src/Mode.h"
#include "../GamingModeGameOfLife/src/Player.h"

class Serveur{
    struct thread_param{
        Serveur* ser;
        SOCKET soc;
    };

private:
    int           port;
    SOCKET	      ListeningSocket;
    bool          running;
    SOCKADDR_IN   ServerAddr;
    DWORD         ClientThread(SOCKET);
    vector<Player*> players;
    vector<Mode*> games;
public:
    Serveur(int);
    int           init();
    int           start ();
    int           pause ();
    static DWORD WINAPI ThreadLauncher(void *p){
        struct thread_param *Obj = reinterpret_cast<struct thread_param*>(p);
        Serveur *s = Obj->ser;
        return s->ClientThread(Obj->soc);
    }
};

#endif //SERVERGAMEOFLIFE_SERVEUR_H
