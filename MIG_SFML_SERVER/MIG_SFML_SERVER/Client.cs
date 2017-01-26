using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace SERVER
{
    class Client
    {
        public int uniquePlayerID; //this won't just simply increment eventually due to disconnects and reconnects but will distribute a number that is not currently in the dictionary

        //network variables
        public IPEndPoint udpEP;
        public Socket tcp;
        public UdpClient udp;
        public int udpPort;

        public Client(Socket tcpSock, UdpClient udpSock, int port, IPEndPoint ep, int id)
        {
            tcp = tcpSock;
            udp = udpSock;
            udpEP = ep;
            udpPort = port;
            uniquePlayerID = id;

            Console.WriteLine("Client Added: TCP {0}, UDP port {1}", tcp.RemoteEndPoint, udpPort);
        }
    }
}
