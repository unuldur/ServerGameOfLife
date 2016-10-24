//
// Created by PAYS on 24/10/2016.
//

#include "RequestManager.h"
#include <string>
RequestManager::RequestManager(string request): request(request) {
}

void RequestManager::manageRequest() {
    string argument = "";
    bool firstWord = true;
    string argName = "";
    string argValue = "";
    bool apres = false;
    for (int i = 0; i < request.size(); ++i) {
        if(request[i] == ' ')
        {
            if(firstWord)
            {
                type = argument;
                firstWord = false;
            } else{
                for (int j = 0; j < argument.size(); ++j) {
                    if(argument[i] == '=')
                    {
                        apres = true;
                        continue;
                    }
                    if(apres)
                    {
                        argValue += argument[i];
                    }
                    else
                    {
                        argName += argument[i];
                    }
                }
                int value = atoi(argValue.c_str());
                arguments[argName] = value;
                argName = "";
                argValue = "";
                apres = false;
            }
            argument = "";
            continue;
        }
        argument += request[i];
    }
}

void RequestManager::execute(map<int, Mode> modes, SOCKET socket) {
    if(type == "INIT")
    {
        return init(modes,socket);
    }
    if(type == "ADD")
    {
        return add(modes,socket);
    }
    if(type == "SET")
    {
        return set(modes,socket);
    }
    if(type == "START")
    {
        return start(modes,socket);
    }
    if(type == "GET")
    {
        return get(modes,socket);
    }
    if(type == "WINNER")
    {
        return winner(modes,socket);
    }
}

void RequestManager::init(map<int, Mode> modes, SOCKET socket) {

}

void RequestManager::set(map<int, Mode> modes, SOCKET socket) {

}

void RequestManager::add(map<int, Mode> modes, SOCKET socket) {

}

void RequestManager::start(map<int, Mode> modes, SOCKET socket) {

}

void RequestManager::get(map<int, Mode> modes, SOCKET socket) {

}

void RequestManager::winner(map<int, Mode> modes, SOCKET socket) {

}
