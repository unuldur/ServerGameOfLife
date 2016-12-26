//
// Created by PAYS on 18/10/2016.
//



#include <algorithm>
#include "Serveur.h"
#include "RequestManager.h"

#define _MAX_HOST_LENGTH_ 100

using namespace std;


Serveur::Serveur(int p){
    port	= p;
    running	= false;
}

int Serveur::init(){
    struct in_addr  MyAddress;
    struct hostent  *host;
    char HostName[_MAX_HOST_LENGTH_];
    WSADATA	      wsaData;

    if(WSAStartup(MAKEWORD(2,2), &wsaData ) != 0 ){
        cerr <<"WSAStartup a échoué "<< endl;
        return 1;
    }

    if( gethostname( HostName, _MAX_HOST_LENGTH_ ) == SOCKET_ERROR ){
        cerr<< "gethostname() a rencontré l'erreur "<< WSAGetLastError()  << endl;
        return 1;
    }

    if( (host = gethostbyname( HostName ) ) == NULL){
        cerr <<"gethostbyname() a rencontré l'erreur "<< WSAGetLastError()<< endl;
        return 1;
    }

    memcpy( &MyAddress.s_addr, host->h_addr_list[0], sizeof( MyAddress.s_addr ) );

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons( port );
    ServerAddr.sin_addr.s_addr = inet_addr( inet_ntoa( MyAddress ) );
    cout <<"server correctement initialisé" << endl;
    return 0;
}

int Serveur::start (){
    SOCKADDR_IN                 ClientAddr;
    int                         ClientAddrLen;
    HANDLE                      hProcessThread;
    SOCKET                      NewConnection;
    struct thread_param         p;

    if( ( ListeningSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET ){
        cerr << "ne peut créer la socket. Erreur n° " << WSAGetLastError()<< endl;
        WSACleanup();
        return 1;
    }

    if( bind( ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof( ServerAddr ) ) == SOCKET_ERROR ){
        cerr << "bind a échoué avec l'erreur " << WSAGetLastError() << endl;
        cerr << "Le port est peut-être déjà utilisé par un autre processus " << endl;
        closesocket( ListeningSocket );
        WSACleanup();
        return 1;
    }

    if( listen( ListeningSocket, 5 ) == SOCKET_ERROR ){
        cerr << "listen a échoué avec l'erreur " << WSAGetLastError() << endl;
        closesocket( ListeningSocket );
        WSACleanup();
        return 1;
    }

    cout << "serveur demarre : à l'ecoute du port " << port << endl;
    running = true;
    ClientAddrLen = sizeof( ClientAddr );

    while(running){

        if((NewConnection = accept( ListeningSocket, (SOCKADDR *) &ClientAddr, &ClientAddrLen)) == INVALID_SOCKET){
            cerr  << "accept a échoué avec l'erreur " << WSAGetLastError() << endl;;
            closesocket( ListeningSocket );
            WSACleanup();
            return 1;
        }

        p.ser = this;
        p.soc = NewConnection;

        cout << "client connecté ::  IP : " <<inet_ntoa( ClientAddr.sin_addr )<< " ,port = " <<ntohs( ClientAddr.sin_port ) << endl;

        hProcessThread = CreateThread(NULL, 0,&Serveur::ThreadLauncher, &p,0,NULL);
        if ( hProcessThread == NULL ){
            cerr << "CreateThread a échoué avec l'erreur " <<GetLastError()<< endl;
        }
    }

    return 0;
}

int Serveur::pause (){
    running = false;
    cout << "Serveur en pause" << endl;
    closesocket( ListeningSocket );
    return 0;
}

/* ======================================================================== */
/* ========================== thread proc ================================= */
/* ======================================================================== */


DWORD Serveur::ClientThread(SOCKET soc){
    cout << "thread client démarré" << endl;
    char buffer[1024];
    RequestManager* request;
    while (recv(soc, buffer, sizeof(buffer), 0) > 0){
        request = new RequestManager(buffer);
        cout << "recupertaion de " << buffer <<endl;
        request->manageRequest();
        request->execute(&games,&players, soc);
    };
    auto it = find_if(players.begin(),players.end(),[&soc](const Player* play) {return play->getSocket() == soc;});
    if(it != players.end())
    {
        for (auto itgames = games.begin(); itgames != games.end(); ++itgames) {
            int ret = (*itgames)->deleteJoueur((*it)->getId());
            if(ret != -1)
            {
                auto joueurs = (*itgames)->getJoueurs();
                stringstream msg;
                if(ret == 0){
                    msg << "ERRORGAME";
                    for (int i = 0; i < joueurs.size(); ++i) {
                        send(joueurs[i]->getSocket(),msg.str().c_str(),msg.str().size(),0);
                    }
                    games.erase(itgames);
                    break;
                }
                if(ret == 1)
                {
                    msg << "DELPLAYER id=" << (*it)->getId();
                    for (int i = 0; i < joueurs.size(); ++i) {
                        send(joueurs[i]->getSocket(),msg.str().c_str(),msg.str().size(),0);
                    }
                    break;
                }
            }

        }
        players.erase(it);
    }
    delete request;
    cout << "fin thread client" << endl;
    return 0;
}