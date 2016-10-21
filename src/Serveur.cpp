//
// Created by PAYS on 18/10/2016.
//



#include "Serveur.h"

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

    cout << "serveur démarré : à l'écoute du port " << port << endl;
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



    return 0;
}