#include <iostream>
#include "TennisServ.h"

using namespace std;

int main()
{
    printf("\n ----- Booting ---- \n");

    TennisServ myServer(4000);

    if(myServer.StartServer() == FALSE)
    {
        cout << "\n\n ERROR: Failed to Start Server \n" << endl;
        return -1;
    }

    // Server Loop
    while(myServer.IsRunning() == TRUE)
    {
        // Update Server
        myServer.Update();
    }

    if(myServer.ShutdownServer() == FALSE)
    {
        cout << "\n\n ERROR: Failed to Shutdown Server \n" << endl;
        return -1;
    }
    return 0;
}
