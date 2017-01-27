using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace SERVER
{
    enum PlayerState
    {
        PLAYER_IDLE,
        PLAYER_WALKING_LEFT,
        PLAYER_WALKING_RIGHT,
        PLAYER_CROUCHING,
        PLAYER_JUMPING
    };

    class Client
    {
        public int uniquePlayerID; //this won't just simply increment eventually due to disconnects and reconnects but will distribute a number that is not currently in the dictionary
        public Pair<float, float> playerCurrentPos;
        public PlayerState state = PlayerState.PLAYER_IDLE; //This will be used to run the simulation

        //network variables
        public IPEndPoint udpEP;
        public Socket tcp;
        public UdpClient udp;
        public int udpPort;

        public byte[] buffer = new byte[PortDefinitions.MAXBUFFSIZE];

        public Client(Socket tcpSock, UdpClient udpSock, int port, IPEndPoint ep, int id, float x, float y)
        {
            tcp = tcpSock;
            udp = udpSock;
            udpEP = ep;
            udpPort = port;
            uniquePlayerID = id;
            playerCurrentPos = new Pair<float, float>(x, y);

            Console.WriteLine("Client Added: TCP {0}, UDP port {1}", tcp.RemoteEndPoint, udpPort);
        }
    }
}
