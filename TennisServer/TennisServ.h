#ifndef TENNISSERV_INCLUDED
#define TENNISSERV_INCLUDED

/*-----------------------------------------
    Includes
-----------------------------------------*/
#include "TennisPlayer.h" // Includes various socket and network headers along with Tennis Player

/*-----------------------------------------
    Defines
-----------------------------------------*/
#define MAXPENDING 5
#define MAXCLIENTS 2

/*-----------------------------------------
    Tennis Server in C++
-----------------------------------------*/
class TennisServ
{

public:
    //------------------------------------------- Public Interface
    TennisServ(unsigned short servPort);
    ~TennisServ();

    int StartServer();
    int ShutdownServer();

    void Update();
    void GotoLobby();
    void NewBall();

    //------------------------------------------- Public Accessors
    int IsRunning() const { return ((m_eState > SS_DEAD) && (m_eState < SS_SHUTDOWN)); }
    int IsReady() const { return ((m_eState > SS_AWAITINGPLAYERS) && (m_eState < SS_SHUTDOWN)); }
    int GetPlayX(int playID) const { return m_players[playID].m_posX; }
    int GetPlayY(int playID) const { return m_players[playID].m_posY; }
    int GetBallX() const { return m_ballPos[X]; }
    int GetBallY() const { return m_ballPos[Y]; }

private:
    //------------------------------------------- Private Enums and Structs
    enum servState
    {
        SS_DEAD,
        SS_AWAITINGPLAYERS,
        SS_PLAYING_GAME,
        SS_GAME_ENDED,
        SS_SHUTDOWN,
        NOOF_SERVER_STATES
    };

    //------------------------------------------- Private Functions
    int     CreateTCPServerSocket();
    void    UpdateWaiting();
    void    UpdateGame();

    //------------------------------------------- Private Varibles
    servState       m_eState;

    timeval         m_oldTime;
    timeval         m_newTime;

    int             m_bBallInPlay;
    float           m_ballPos[2];
    float           m_ballDir[2];

    unsigned short  m_serverPort;
    int             m_servSock;
    sockaddr_in     m_servAddress;

    TennisPlayer    m_players[MAXCLIENTS];
};

#endif // TENNISSERV_INCLUDED
