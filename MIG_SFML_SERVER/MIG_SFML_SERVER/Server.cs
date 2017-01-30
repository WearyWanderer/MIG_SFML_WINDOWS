using SERVER;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace SERVER
{
    public class Server
    {
        private string m_lobbyName;
        private string m_lobbyPassword;

        public string LobbyName
        {
            get { return m_lobbyName; }
            set { m_lobbyName = value; }
        }

        public string LobbyPassword
        {
            get { return m_lobbyPassword; }
            set { m_lobbyPassword = value; }
        }

        private static Server m_instance;

        private Server() { }

        public static Server Instance
        {
            get
            {
                if (m_instance == null)
                    m_instance = new Server();

                return m_instance;
            }
        }

        public int numPlayersActive = 0;

        public void InitServer(string lobbyName, string password)
        {
            Console.WriteLine("Starting Server...");

            LobbyName = lobbyName; LobbyPassword = password;
            UdpBroadcaster broadcaster = new UdpBroadcaster();
            ReactorListenerServer server = new ReactorListenerServer();

            // Start broadcasting thread
            Thread bcastThread = new Thread(new ThreadStart(broadcaster.broadcastIP));
            bcastThread.Name = "BroadcastThread";
            bcastThread.Start();

            Thread connectionThread = new Thread(new ThreadStart(server.ListenForClients));
            connectionThread.Name = "ListenerThread";
            connectionThread.Start();

            Thread dispatcherThread = new Thread(new ThreadStart(server.StartDispatcherThread));
            dispatcherThread.Name = "DispatcherThread";
            dispatcherThread.Start();

            Thread syncThread = new Thread(new ThreadStart(server.SyncMaintenance));
            syncThread.Name = "SyncThread";
            syncThread.Start();
        }
    }
}
