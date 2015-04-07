#ifndef TENNISPLAYER_INCLUDED
#define TENNISPLAYER_INCLUDED

/*-----------------------------------------
    Includes
-----------------------------------------*/
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netdb.h>      /* for gethostbyaddr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>
#include <unistd.h>     /* for close() */
#include <pthread.h>    /* for POSIX threads */

/*-----------------------------------------
    Defines
-----------------------------------------*/
#define TRUE    1
#define FALSE   0
#define X       0
#define Y       1

static const int COURT_WIDTH = 300;
static const int COURT_HEIGHT = 500;
static const int RAQ_OFFSET = 10;
static const int RAQ_SIZE = 30;

// Pre-declare Server Class
class TennisServ;

/*-----------------------------------------
    Tennis Player in C++
-----------------------------------------*/
class TennisPlayer
{
public:
    //------------------------------------------- Public Interface
    TennisPlayer();
    ~TennisPlayer();

    int     AwaitingClientConnection(int ServerSocket, TennisServ* pServ, int playID);
    void    Kill();

    int     Pulse();
    int     SendData();
    int     RecieveData();

    int     Running() const { return m_bConnected; }

    TennisServ* GetServer() { return m_pServer; }

    //------------------------------------------- Public Accessors

private:
    //------------------------------------------- Private Enums and Structs
    //------------------------------------------- Private Functions
    //------------------------------------------- Private Varibles
    int     m_bConnected;
    int     m_posX;
    int     m_posY;
    int     m_playerID;

    int         m_clientSocket;
    sockaddr_in m_clientAddress;
    pthread_t   m_thread;
    hostent*    m_pHostDetails;

    TennisServ* m_pServer;

    friend class TennisServ;
};

#endif // TENNISPLAYER_INCLUDED
