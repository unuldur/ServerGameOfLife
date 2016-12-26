//
// Created by PAYS on 24/10/2016.
//

#ifndef SERVERGAMEOFLIFE_FACTORYMANAGER_H
#define SERVERGAMEOFLIFE_FACTORYMANAGER_H

#include <iostream>
#include <map>
#include <winsock.h>
#include "../GameOfLifeMode/src/Mode.h"
#include "../GameOfLifeMode/src/Player.h"

using namespace std;
class RequestManager {
public:
    RequestManager(string request);
    void manageRequest();
    void execute(vector<Mode*> * modes,vector<Player*> *,SOCKET socket);

private:
    map<string,string> arguments;
    string request;
    string type;
    void manageArgument(string const &argument);
    void init(vector<Mode*> * modes,vector<Player*> *,SOCKET socket);
    void set(vector<Mode*> * modes, SOCKET socket);
    void add(vector<Mode*> * modes,vector<Player*> *,SOCKET socket);
    void start(vector<Mode*> * modes,SOCKET socket);
    void get(vector<Mode*> * modes,SOCKET socket);
    void winner(vector<Mode*> * modes,SOCKET socket);
    void getGames(vector<Mode*> * modes,SOCKET socket);
    void addPlayer(vector<Player *> * players,SOCKET socket);
    void getPlayersGame(vector<Mode*> * modes,SOCKET socket);

};


#endif //SERVERGAMEOFLIFE_FACTORYMANAGER_H
