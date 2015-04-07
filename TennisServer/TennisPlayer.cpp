/*-----------------------------------------
    Tennis Player in C++
-----------------------------------------*/

#include "TennisPlayer.h"
#include "TennisServ.h"
#include <errno.h>

#define DONE        0x0D03
#define NEWBALL     0x0BA1
#define RAQUET      0x05AC

#if !defined(max)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

//========================================================================
//  Constructor Empty Player
TennisPlayer::TennisPlayer()
{
    m_bConnected = FALSE;
    m_posX = 0;
    m_posY = 0;

    m_clientSocket = NULL;
    m_pHostDetails = NULL;
    m_pServer = NULL;

    memset(&m_clientAddress, 0, sizeof(m_clientAddress));
    memset(&m_thread, 0, sizeof(m_thread));
}

//========================================================================
//  Kill Player
TennisPlayer::~TennisPlayer()
{
}

//========================================================================
//  Setup Player from Recieved Connection
int TennisPlayer::AwaitingClientConnection(int ServerSocket, TennisServ* pServ, int playID)
{
    unsigned int clientAddressLength = sizeof(m_clientAddress);       // Length of Client Address Struct
    m_pServer = pServ;
    m_playerID = playID;

    // Wait to accept connection :: BLOCKING CALL!!!
    if ((m_clientSocket = accept(ServerSocket, (sockaddr*)&m_clientAddress, &clientAddressLength)) < 0)
    {
        perror("Player Failed to Accept Connection");
        return FALSE;
    }

    // Get Host Name
    m_pHostDetails = gethostbyaddr((char*) &m_clientAddress.sin_addr, sizeof(m_clientAddress.sin_addr), AF_INET);

    // Annouce Player Join
    printf("Handling client %s\n", inet_ntoa(m_clientAddress.sin_addr));
    m_bConnected = TRUE;

    return TRUE;
}

//========================================================================
//  Kill Player Connection
void TennisPlayer::Kill()
{
    if(m_bConnected == TRUE)
    {
        printf("Killed Player [%i] \n", m_playerID);
        close(m_clientSocket);
        m_bConnected = FALSE;
    }
}

//========================================================================
//  Send Pulse to Player to Check Life signs
int TennisPlayer::Pulse()
{
    if(m_bConnected == FALSE)
    {
        return FALSE;
    }

    if(send(m_clientSocket, "PULSE!", 20, NULL) < 2)
    {
        Kill();
        return FALSE;
    }

    return TRUE;
}

//========================================================================
//  Send Data to Player
int TennisPlayer::SendData()
{
    if(m_bConnected == FALSE)
    {
        printf("Attempting to send to dead Player [%i] \n", m_playerID);
        return FALSE;
    }

    char buffer[255];

    // Paddle Position "RAC %d %d %d %d",
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "RAC %d %d %d %d ]",
                    m_pServer->GetPlayX(0),
                    m_pServer->GetPlayX(1),
                    m_pServer->GetPlayY(0),
                    m_pServer->GetPlayY(1));

    if(send(m_clientSocket, buffer, sizeof(buffer), NULL) < 2)
    {
        printf("Failed to Send Data to Player [%i] \n", m_playerID);
        Kill();
        return FALSE;
    }

    // Done Message
    return TRUE;
}

//========================================================================
//  Listen to Player
int TennisPlayer::RecieveData()
{
    if(m_bConnected == FALSE)
    {
        printf("Attempting to listen to dead Player [%i] \n", m_playerID);
        return FALSE;
    }

    int result[128];
    char buffer[255];
    int recvBytes = -1;

    memset(result, 0, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    recvBytes = recv(m_clientSocket, buffer, sizeof(buffer), NULL);

    if(recvBytes < 0)
    {
        printf("Error: recv returned Error [%s] \n", strerror(errno));
        return FALSE;
    }

    if((recvBytes % 2) == 1)
    {
        printf("Recieved Odd number of Bytes  \n");
        return FALSE;
    }

    int iCur = 0;
    while(iCur < recvBytes)
    {
        char holdA = buffer[iCur];
        char holdB = buffer[iCur+1];

        result[iCur/2] = (holdA & 0xFF);
        result[iCur/2] = result[iCur/2] << 8;
        result[iCur/2] |= (holdB & 0xFF);

        // printf("Recv [%X : %X] -> [%X] \n", holdA, holdB, result[iCur/2]);
        iCur += 2;
    }

    iCur = 0;
    while((iCur*2) < recvBytes)
    {
        if(result[iCur] == 0)
        {
        }
        else if(result[iCur] == DONE)
        {
            printf("Player [%i] requests DONE \n", m_playerID);
            Kill();
        }
        else if(result[iCur] == NEWBALL)
        {
            printf("Player [%i] requests NEW BALL \n", m_playerID);
            m_pServer->NewBall();

        }
        else if((result[iCur] & 0xF000) == 0x1000)
        {
            m_posX = (result[iCur] & 0x0FFF) - 0x7D0;
        }
        else if((result[iCur] & 0xF000) == 0x2000)
        {
            m_posY = (result[iCur] & 0x0FFF) - 0x7D0;

            if(m_playerID == 0)
            {
                m_posY = min((COURT_HEIGHT / 2), max(0, m_posY));
            }
            else
            {
                m_posY = min(COURT_HEIGHT, max((COURT_HEIGHT / 2), m_posY));
            }
        }

        iCur += 1;
    }

    return TRUE; /**/
}
