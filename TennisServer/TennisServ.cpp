/*-----------------------------------------
    Tennis Server in C++
-----------------------------------------*/

#include "TennisServ.h"
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

// Service Player Thread Service Function
void* service_Player( void* pVoidPlayer );

//========================================================================
//  Constructor which sets up port
TennisServ::TennisServ(unsigned short servPort)
{
    m_serverPort = servPort;
    m_eState = SS_DEAD;
}

//========================================================================
//  Deconstructor
TennisServ::~TennisServ()
{
    if(IsRunning() == TRUE)
    {
        ShutdownServer();
    }
}

//========================================================================
//  Start Server UP
int TennisServ::StartServer()
{
    if(CreateTCPServerSocket() != TRUE)
    {
        return FALSE;
    }

    printf("\n ----- Server Setup---- \n");
    printf("\n ----- Awaiting Players ---- \n");
    m_eState = SS_AWAITINGPLAYERS;

    return TRUE;
}

//========================================================================
//  Take Server DOWN
int TennisServ::ShutdownServer()
{
    printf("\n ----- Server Shutdown ---- \n");
    m_eState = SS_SHUTDOWN;

    return TRUE;
}

//========================================================================
//  Update Server
void TennisServ::Update()
{
    switch(m_eState)
    {
        case SS_AWAITINGPLAYERS:
            UpdateWaiting();
            break;

        case SS_PLAYING_GAME:
            UpdateGame();
            break;

        case SS_GAME_ENDED:
            break;

        default:
            // Do Nothing
            break;
    }

    // Function will re-enter via main
}

//========================================================================
//  Setup TCP Server Socket
int TennisServ::CreateTCPServerSocket()
{
    // Setup Local Address
    memset(&m_servAddress, 0, sizeof(m_servAddress));
    m_servAddress.sin_family  = AF_INET;                  /* Internet address family */
    m_servAddress.sin_addr.s_addr = htonl(INADDR_ANY);    /* Any incoming interface */
    m_servAddress.sin_port = htons(m_serverPort);         /* Local port */

    // Create socket for incoming connections
    if ((m_servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Creation of Server Socket Failed");
        return FALSE;
    }

    // Set Socket to be Reusable Address
    int bReusableSocket = TRUE;
    if (setsockopt(m_servSock, SOL_SOCKET, SO_REUSEADDR, &bReusableSocket, sizeof(int)) < 0)
    {
        perror("Failed to set socket option: Reusable Socket to TRUE");
        return FALSE;
    }

    // Bind to Server Address
    if(bind(m_servSock, (sockaddr*) &m_servAddress, sizeof(m_servAddress)) < 0)
    {
        perror("Unable to bind local socket");
        return FALSE;
    }

    // Tell the server socket to listen
    if (listen(m_servSock, MAXPENDING) < 0)
    {
        perror("Unable to set server socket to listen");
        return FALSE;
    }

    return TRUE;
}

//========================================================================
//  Update Awaiting Players State
void TennisServ::UpdateWaiting()
{
    int bFullyConnected = TRUE;

    // Cycle Players and Attempt to connect all
    for(int iPlayer = 0; iPlayer < MAXCLIENTS; ++iPlayer)
    {
        if(m_players[iPlayer].Running() == FALSE)
        {
            bFullyConnected = FALSE;
            printf("Waiting for Player [%i] to join \n", iPlayer);

            if(m_players[iPlayer].AwaitingClientConnection(m_servSock, this, iPlayer) == FALSE)
            {
                printf("Failed to Connect Player!!! \n");
            }
            else
            {
                // Setup Player Thread
                printf("Player [%i] joined. Service Thread Starting! \n     --------- \n", iPlayer);
                pthread_create(&(m_players[iPlayer].m_thread), NULL, service_Player, (void*)&(m_players[iPlayer]));
            }
        }
        else
        {
            if(m_players[iPlayer].Pulse() == FALSE)
            {
                printf("Connection to Player [%i] died \n", iPlayer);
                bFullyConnected = FALSE;
            }
            else
            {
                printf("Already Connected Player [%i] \n", iPlayer);
            }
        }
    }

    // If all players are fully connected then start game
    if(bFullyConnected == TRUE)
    {
        printf("\n ----- Starting Game ---- \n");
        m_players[0].m_posX = COURT_WIDTH / 2;
        m_players[1].m_posX = COURT_WIDTH / 2;

        m_players[0].m_posY = RAQ_OFFSET;
        m_players[1].m_posY = COURT_HEIGHT - RAQ_OFFSET;

        m_bBallInPlay = FALSE;

        gettimeofday(&m_oldTime, NULL);

        m_eState = SS_PLAYING_GAME;
    }
}

//========================================================================
//  Update Game State
void TennisServ::UpdateGame()
{
    gettimeofday(&m_newTime, NULL);

    if(m_bBallInPlay == TRUE)
    {
        // Update Ball Position
        m_ballPos[X] += m_ballDir[X] * 0.01f;
        m_ballPos[Y] += m_ballDir[Y] * 0.01f;

        // Check Sides
        if(m_ballDir[X] < 0)
        {
            if(m_ballPos[X] < 0)
            {
                m_ballPos[X] = 1.0f;
                m_ballDir[X] *= -1.0f;
            }
        }
        else
        {
            if(m_ballPos[X] > COURT_WIDTH)
            {
                m_ballPos[X] = COURT_WIDTH - 1.0f;
                m_ballDir[X] *= -1.0f;
            }
        }

        // Check Vertical
        if(m_ballDir[Y] < 0)
        {
            // Point Sampling Collision is a bit messy we really should use ray collision

            if(m_ballPos[Y] < 0.0f)
            {
                // Score for Player 1
                m_bBallInPlay = FALSE;
            }
            else if(fabsf(m_ballPos[Y] - GetPlayY(0)) < 8.0f)
            {
                // Check Player 0 Bat Collision
                float posBat = fabs(GetPlayX(0) - m_ballPos[X]) / (RAQ_SIZE * 1.0f);

                if(posBat < 1.0f)
                {
                    m_ballDir[X] *= (posBat + 1.0f);
                    m_ballDir[Y] *= -1.0f;
                }
            }
        }
        else
        {
            if(m_ballPos[Y] > COURT_HEIGHT)
            {
                // Score for Player 0
                m_bBallInPlay = FALSE;
            }
            else if(fabsf(m_ballPos[Y] - GetPlayY(1)) < 8.0f)
            {
                // Check Player 1 Bat Collision
                float posBat = fabs(GetPlayX(1) - m_ballPos[X]) / (RAQ_SIZE * 1.0f);

                if(posBat < 1.0f)
                {
                    m_ballDir[X] *= (posBat + 1.0f);
                    m_ballDir[Y] *= -1.0f;
                }
            }
        }
    }

    // Update Network Ball
    if(IsReady() == TRUE)
    {
        char buffer[255];

        // Ball Position "POS %d %d"
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "POS %d %d ]", GetBallX(), GetBallY());

        for(int iPlay = 0; iPlay < 2; ++iPlay)
        {
            if(m_players[iPlay].Running() == TRUE)
            {
                if(send(m_players[iPlay].m_clientSocket, buffer, sizeof(buffer), NULL) < 2)
                {
                    printf("Failed to Send Data to Player [%i] \n", iPlay);
                }
            }
        }
    }

    // Sleep for X time
    timeval result;
    char bWait = TRUE;

    while(bWait == TRUE)
    {
        gettimeofday(&m_newTime, NULL);
        timersub(&m_oldTime, &m_newTime, &result);

        if((result.tv_sec + result.tv_usec/1000000.0f) < (1.0f / 1.2f)) // 1000000 microseconds per second
        {
            bWait = FALSE;
        }
    }

    gettimeofday(&m_oldTime, NULL);
}

//========================================================================
//  Put New Ball into Play
void TennisServ::NewBall()
{
    if(m_bBallInPlay == TRUE)    { return; }

    m_ballPos[X] = (COURT_WIDTH / 3) + (rand() % (COURT_WIDTH / 3));
    m_ballPos[Y] = COURT_HEIGHT / 2;

    m_ballDir[X] = (rand() % 50) / 100.0f;
    m_ballDir[Y] = 0.5f + (1.0f - m_ballDir[X]);

    // Up or Down
    if((rand() % 2) == FALSE)
    {
        m_ballDir[Y] *= -1.0f;
    }

    m_bBallInPlay = TRUE;
}

//========================================================================
//  Jump back to the Lobby
void TennisServ::GotoLobby()
{
    printf("\n ----- Returning to Lobby---- \n");
    printf("\n ----- Awaiting Players ---- \n");
    m_eState = SS_AWAITINGPLAYERS;
}


//========================================================================
//  Service Player Thread
void* service_Player( void* pVoidPlayer )
{
    TennisPlayer* pPlayer = static_cast<TennisPlayer*>(pVoidPlayer);
    TennisServ* pServ = pPlayer->GetServer();

    while((pPlayer != NULL) && (pPlayer->Running() == TRUE))
    {
        if((pPlayer->Running() == TRUE) && (pServ->IsReady() == TRUE))
        {
            pPlayer->RecieveData();
        }

        if((pPlayer->Running() == TRUE) && (pServ->IsReady() == TRUE))
        {
            pPlayer->SendData();
        }
    }

    pServ->GotoLobby();
    return NULL;
}
