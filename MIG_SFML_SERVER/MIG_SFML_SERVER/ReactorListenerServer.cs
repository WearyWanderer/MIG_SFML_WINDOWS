using System; // For Console, Int32, ArgumentException, Environment
using System.Collections.Generic;
using System.Net; // For IPAddress
using System.Net.Sockets; // For TcpListener, TcpClient
using System.Text;
using System.Threading;

namespace SERVER
{
    class ReactorListenerServer
    {
        //client tracking
        Dictionary<int, Client> clients = new Dictionary<int, Client>();
        int clientNum = 0;
        //sync management
        AutoResetEvent wait = new AutoResetEvent(false);
        Queue<Event> eventQueue = new Queue<Event>();

        public void ListenForClients()
        {
            string localIP = PortDefinitions.GetLocalIP();

            //Set up our TCP listener for new connection reuqests
            IPEndPoint localEP = new IPEndPoint(IPAddress.Parse(localIP), PortDefinitions.TCP_CONNECT_PORT);
            Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                listener.Bind(localEP);
                listener.Listen(64); //max max backlog of connects is low but just set to this for testing

                while(true) //when we get a connection request from a client
                {
                    wait.Reset();
                    listener.BeginAccept(new AsyncCallback(AcceptClient), listener);
                    wait.WaitOne();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception occured in listening for clients: " + e.Message);
                Environment.Exit(e.HResult);
            }
        }

        public void AcceptClient(IAsyncResult ar)
        {
            wait.Set(); //set our wait handle to ensure sync and don't gobble up memory!

            //tcp client creation
            Socket listener = (Socket)ar.AsyncState; 
            Socket localTcp = listener.EndAccept(ar);

            //udp client creation
            IPEndPoint localUDPEP = new IPEndPoint(IPAddress.Parse(PortDefinitions.GetLocalIP()), 0); //we request a free port by passing 0 to constructor
            UdpClient udpClient = new UdpClient(localUDPEP);

            //Create our tcp and udp tracking client object here, add it to our management dictionary or map or something
            AddClient(new Client(localTcp, udpClient, ((IPEndPoint)udpClient.Client.LocalEndPoint).Port, localUDPEP, clientNum));

        }

        public void StartWorkerThreads()
        {

        }

        public void HandleRequests()
        {

        }

        public void AddClient(Client clientToAdd)
        {
            clients.Add(clientNum, clientToAdd);
            clientNum++;

            //send back a registration packet
            byte[] msg = Encoding.ASCII.GetBytes(clientNum.ToString() + " your spawn pos will be 0");

            clientToAdd.tcp.Send(msg);
        }
    }
}



