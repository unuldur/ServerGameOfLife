//
// Created by PAYS on 24/10/2016.
//

#include "RequestManager.h"
#include "../GamingModeGameOfLife/src/FactoryMode.h"
#include <string>
#include <algorithm>
#include <unistd.h>

RequestManager::RequestManager(string request): request(request) {
}

void RequestManager::manageArgument(string const &argument)
{
    bool apres = false;
    string argValue = "";
    string argName = "";

    for (int j = 0; j < argument.size(); ++j) {
        if(argument[j] == '=')
        {
            apres = true;
            continue;
        }
        if(apres)
        {
            argValue += argument[j];
        }
        else
        {
            argName += argument[j];
        }
    }
    arguments[argName] = argValue;
}


/**
 * Arguments valable :
 *  - idP = idplayer
 *  - idG = idGame
 *  - nameP = nomPlayer
 *  - mode = nom du mode
 *  - x, y = localisation du point.
 */
void RequestManager::manageRequest() {
    string argument = "";
    bool firstWord = true;
    for (int i = 0; i < request.size(); ++i) {
        if(request[i] == ' ')
        {
            if(firstWord)
            {
                type = argument;
                firstWord = false;
            } else{
                manageArgument(argument);
            }
            argument = "";
            continue;
        }
        argument += request[i];
    }
}


void RequestManager::execute(vector<Mode*> * modes,vector<Player*> * players , SOCKET socket) {
    if(type == "INIT")
    {
        return init(modes,players,socket);
    }
    if(type == "ADD")
    {
        return add(modes, players,socket);
    }
    if(type == "SET")
    {
        return set(modes, socket);
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
    if(type == "GETGAME")
    {
        return getGames(modes,socket);
    }
    if(type == "ADDPLAYER")
    {
        return addPlayer(players,socket);
    }
    if(type == "GETPLAYERS")
    {
        return getPlayersGame(modes,socket);
    }
    if(type == "GETZONE"){
        return getZone(modes,socket);
    }

}

void RequestManager::init(vector<Mode*> * modes,vector<Player*> * players, SOCKET socket) {
    stringstream buffer;
    Mode* mode = FactoryMode::createInstance(arguments["mode"]);
    auto it = find_if(players->begin(),players->end(),[&socket](const Player* play) {return play->getSocket() == socket;});
    if(it == players->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    buffer << "GAMEMODE ";
    mode->addJoueur(*it);
    modes->push_back(mode);
    buffer << "id=" << mode->getId();
    send(socket,buffer.str().c_str(),buffer.str().length(),0);
}

void RequestManager::set(vector<Mode*> * modes, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    int idPlayer = atoi((arguments["idP"]).c_str());
    string x =  arguments["x"];
    string y = arguments["y"];
    string buffer = "SWITCH x="+x+" y="+y+" player="+arguments["idP"];
    vector<Player*> playerInGame;
    auto it = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode){ return mode->getId() == idGame;});
    if(it == modes->end())
    {
        buffer == "ERROR";
        send(socket,buffer.c_str(),buffer.length(),0);
        return;
    }
    (*it)->setByte(atoi(x.c_str()),atoi(y.c_str()),idPlayer);
    playerInGame = (*it)->getJoueurs();
    for (int i = 0; i < playerInGame.size(); ++i) {
        send(playerInGame[i]->getSocket(),buffer.c_str(), buffer.length(),0);
    }

}

void RequestManager::add(vector<Mode*> * modes,vector<Player*> * players, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    int idPlayer = atoi((arguments["idP"]).c_str());
    stringstream buffer;
    stringstream bufferOtherPlayer;
    auto itGame = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode) {
        return mode->getId() == idGame;});
    if(itGame == modes->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }

    auto itPlayer = find_if(players->begin(),players->end(),[&idPlayer](const Player* play) {
        return play->getId() == idPlayer;});
    if(itPlayer == players->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }

    int status = (*itGame)->addJoueur(*itPlayer);
    if(status == 1){
        buffer << "ADDPLAYER complete=true";
    }
    else{
        buffer << "ADDPLAYER complete=false";
    }

    send(socket,buffer.str().c_str(),buffer.str().length(),0);
    if(status == 1){
        (*itGame)->initializeZone();
        bufferOtherPlayer << "NEWPLAYER complete=true ";
    }
    else{
        bufferOtherPlayer << "NEWPLAYER complete=false ";
    }
    bufferOtherPlayer << (*itPlayer)->getInfo();
    auto playerInGame = (*itGame)->getJoueurs();
    for(auto it = playerInGame.begin(); it != playerInGame.end() ; ++it){
        if((*it)->getId() == idPlayer) continue;
        send((*it)->getSocket(),bufferOtherPlayer.str().c_str(),bufferOtherPlayer.str().length(),0);
    }


}

void RequestManager::start(vector<Mode*> * modes, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    stringstream buffer;
    auto itGame = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode) {
        return mode->getId() == idGame;});
    if(itGame == modes->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    int status = (*itGame)->startRunning();
    if(status == 1)
    {
        buffer << "STARTGEN";
        auto playerInGame = (*itGame)->getJoueurs();
        for(auto it = playerInGame.begin(); it != playerInGame.end() ; ++it){
            send((*it)->getSocket(),buffer.str().c_str(),buffer.str().length(),0);
        }
    }
}

void RequestManager::get(vector<Mode*> * modes, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    int gen = atoi((arguments["gen"]).c_str());
    stringstream buffer;
    auto itGame = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode) {
        return mode->getId() == idGame;});
    if(itGame == modes->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    string result = (*itGame)->getDifferenceGeneration(gen);
    if(result == "ERROR"){
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    buffer << "DIFFGEN "<< result;
    send(socket,buffer.str().c_str(),buffer.str().length(),0);
}

void RequestManager::winner(vector<Mode*> * modes, SOCKET socket) {
    /*
    int idGame = atoi((arguments["idG"]).c_str());
    string buffer;
    buffer = (*modes)[idGame]->getWinner();
    send(socket,buffer.c_str(),buffer.length(),0);
     */
}

void RequestManager::getGames(vector<Mode*> * modes, SOCKET socket) {
    string buffer;
    buffer += "GAMES ";
    for (auto it = modes->begin();it != modes->end();it++)
    {
        if(!(*it)->complete()) buffer += (*it)->infos() + " ";
    }
    if(buffer == "") buffer = "ERROR";
    send(socket,buffer.c_str(),buffer.length(),0);
}

void RequestManager::addPlayer(vector<Player *> * players , SOCKET socket) {
    stringstream buffer;
    string name = arguments["nameP"];
    auto it = find_if(players->begin(),players->end(),[&name](const Player* play) {
        return play->getName() == name;});
    if(it != players->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    buffer << "PLAYER id=";
    Player* play = new Player(name,socket);
    players->push_back(play);
    buffer << play->getId();
    send(socket,buffer.str().c_str(),buffer.str().length(),0);
}

void RequestManager::getPlayersGame(vector<Mode *> *modes, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    stringstream buffer;
    auto itGame = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode) {
        return mode->getId() == idGame;});
    if(itGame == modes->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    buffer << "PLAYERSGAME ";
    auto playerInGame = (*itGame)->getJoueurs();
    for(auto it = playerInGame.begin(); it != playerInGame.end() ; ++it){
        buffer << (*it)->getInfo() <<" ";
    }
    send(socket,buffer.str().c_str(),buffer.str().length(),0);
}

void RequestManager::getZone(vector<Mode *> *modes, SOCKET socket) {
    int idGame = atoi((arguments["idG"]).c_str());
    stringstream buffer;
    auto itGame = find_if(modes->begin(),modes->end(),[&idGame](const Mode* mode) {
        return mode->getId() == idGame;});
    if(itGame == modes->end())
    {
        buffer << "ERROR";
        send(socket,buffer.str().c_str(),buffer.str().length(),0);
        return;
    }
    buffer << "GETZONE ";
    buffer << (*itGame)->getZone();
    send(socket,buffer.str().c_str(),buffer.str().length(),0);
}
