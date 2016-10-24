//
// Created by PAYS on 24/10/2016.
//

#ifndef SERVERGAMEOFLIFE_FACTORYMANAGER_H
#define SERVERGAMEOFLIFE_FACTORYMANAGER_H

#include <iostream>
#include <map>
#include <winsock.h>
#include "../GameOfLifeMode/src/Mode.h"

using namespace std;
class RequestManager {
public:
    RequestManager(string request);
    void manageRequest();
    void execute(map<int,Mode> modes,SOCKET socket);

private:
    map<string,int> arguments;
    string request;
    string type;
    void init(map<int,Mode> modes,SOCKET socket);
    void set(map<int,Mode> modes,SOCKET socket);
    void add(map<int,Mode> modes,SOCKET socket);
    void start(map<int,Mode> modes,SOCKET socket);
    void get(map<int,Mode> modes,SOCKET socket);
    void winner(map<int,Mode> modes,SOCKET socket);

};


#endif //SERVERGAMEOFLIFE_FACTORYMANAGER_H
