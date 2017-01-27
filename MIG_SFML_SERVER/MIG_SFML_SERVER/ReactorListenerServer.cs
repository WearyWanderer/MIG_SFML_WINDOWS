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

                while (true) //when we get a connection request from a client
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
            Random rand = new Random(clientNum);

            //tcp client creation
            Socket listener = (Socket)ar.AsyncState;
            Socket localTcp = listener.EndAccept(ar);

            //udp client creation
            IPEndPoint localUDPEP = new IPEndPoint(IPAddress.Parse(PortDefinitions.GetLocalIP()), 0); //we request a free port by passing 0 to constructor
            UdpClient udpClient = new UdpClient(localUDPEP);

            //Create our tcp and udp tracking client object here, add it to our management dictionary or map or something
            AddClient(new Client(localTcp, udpClient, ((IPEndPoint)udpClient.Client.LocalEndPoint).Port, localUDPEP, clientNum, rand.Next(40, 1301), 768));
            Client thisClient;
            clients.TryGetValue(clientNum, out thisClient);

            thisClient.tcp.BeginReceive(thisClient.buffer, 0, PortDefinitions.MAXBUFFSIZE, 0, new AsyncCallback(TCPListen), thisClient);
            thisClient.udp.BeginReceive(new AsyncCallback(UDPListen), thisClient);

            eventQueue.Enqueue(new Event(EventType.REQUEST_LOAD_PLAYERS, clientNum));
            eventQueue.Enqueue(new Event(EventType.NEW_CONNECT, clientNum));

            clientNum++;
        }

        public void StartDispatcherThread()
        {
            while (true) //endlessley run distributing our events to the thread pool workers
            {
                if (eventQueue.Count != 0)
                {
                    WorkerTask worker = new WorkerTask(eventQueue.Dequeue(), ref clients);
                    ThreadPool.QueueUserWorkItem(o => WorkerExecution.DoWork(worker), null);
                }
            }
        }

        public void AddClient(Client clientToAdd)
        {
            clients.Add(clientNum, clientToAdd);

            //send back a registration packet
            //TODO - make gen a random position on X, also send back the udp port we're listening on
            byte[] msg = Encoding.ASCII.GetBytes(clientNum.ToString() + ";" + clientToAdd.playerCurrentPos.First + ";" + clientToAdd.udpPort + ";ep;");

            clientToAdd.tcp.Send(msg);
        }

        public void TCPListen(IAsyncResult ar)
        {
            Client clientInfo = (Client)ar.AsyncState;
            string msg = System.Text.Encoding.UTF8.GetString(clientInfo.buffer);
            if (msg.Length != 0)
            {
                string typeMsg = "";
                try
                {
                    typeMsg = msg.Substring(0, msg.IndexOf(';'));
                }
                catch(ArgumentOutOfRangeException e)
                {
                    Console.WriteLine("Couldn't parse a message!");
                }

                //lets first check we aren't disconnecting, if we are we neednt waste more time on this client
                if (typeMsg == "dp")
                {
                    Console.WriteLine("Client {0} disconnecting, notifying active players...", clientInfo.udpEP.ToString());
                    eventQueue.Enqueue(new Event(EventType.DISCONNECT, clientInfo.uniquePlayerID));
                }
            }
        }

        public void UDPListen(IAsyncResult ar)
        {
            Client clientInfo = (Client)ar.AsyncState;

            UdpClient u = clientInfo.udp;
            IPEndPoint e = clientInfo.udpEP;
            try
            {
                Byte[] recieveBytes = u.EndReceive(ar, ref e);
                string msg = System.Text.Encoding.UTF8.GetString(recieveBytes);
                Queue<string> tokens = split(msg, ";");

                if (msg.Length != 0)
                {
                    string token = tokens.Dequeue();
                    if (token == "lp")
                    {
                        clients[clientInfo.uniquePlayerID].udpPort = Convert.ToInt32(tokens.Dequeue());
                        clients[clientInfo.uniquePlayerID].udpEP.Port = clients[clientInfo.uniquePlayerID].udpPort;
                    }
                    else if (token == "mp")
                    {
                        clients[clientInfo.uniquePlayerID].state = (PlayerState)Convert.ToInt32(tokens.Dequeue());
                        eventQueue.Enqueue(new Event(EventType.POSITION_UPDATE, clientInfo.uniquePlayerID));
                    }
                }

                clientInfo.udp.BeginReceive(new AsyncCallback(UDPListen), clientInfo);
            }
            catch (ObjectDisposedException ex)
            {
                //likely thrown because of a disconnect event in a worker thread, this is fine
            }
        }

        public Queue<string> split(string str, string delim)
        {
            Queue<string> tokens = new Queue<string>();
            int prev = 0, pos = 0;
            do
            {
                pos = str.IndexOf(delim, prev);
                if (pos == str.Length) pos = str.Length;
                string token = str.Substring(prev, pos - prev);
                if (token.Length != 0) tokens.Enqueue(token);
                prev = pos + delim.Length;
            } while (pos < str.Length && prev < str.Length);
            return tokens;
        }
    }
}



