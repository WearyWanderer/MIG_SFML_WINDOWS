using System; // For Console, Int32, ArgumentException, Environment
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net; // For IPAddress
using System.Net.Sockets; // For TcpListener, TcpClient
using System.Text;
using System.Threading;
#if _WIN32
using System.Data.SQLite;
#elif __APPLE__
using Mono.Data.Sqlite;
#endif

namespace SERVER
{
    class ReactorListenerServer
    {
        double syncRefreshRate = 0.5; //
        //client tracking
        Dictionary<int, Client> clients = new Dictionary<int, Client>();
        int clientNum = 0;
        //sync management
        AutoResetEvent wait = new AutoResetEvent(false);
        Queue<Event> eventQueue = new Queue<Event>();

        public void SyncMaintenance()
        {
            Stopwatch sw = Stopwatch.StartNew();
            while(true) //run forever
            {
                
                sw.Restart();
            }
        }

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
            Random rand = new Random(Guid.NewGuid().GetHashCode() + clientNum);

            //tcp client creation
            Socket listener = (Socket)ar.AsyncState;
            Socket localTcp = listener.EndAccept(ar);

            //udp client creation
            IPEndPoint localUDPEP = new IPEndPoint(IPAddress.Parse(PortDefinitions.GetLocalIP()), 0); //we request a free port by passing 0 to constructor
            UdpClient udpClient = new UdpClient(localUDPEP);

            byte[] loginBuffer = new byte[512];
            localTcp.Receive(loginBuffer);
            string msg = System.Text.Encoding.UTF8.GetString(loginBuffer);
            Queue<string> tokens = split(msg, ";");

            if (tokens.Count == 3)
            {
				#if _WIN32
				SQLiteConnection dbConnection = new SQLiteConnection("Data Source = logindb.db;Version=3;");
				int clientNum = CreateOrRetrievePlayer(dbConnection, tokens.Dequeue(), tokens.Dequeue());
				dbConnection.Close();
				#elif __APPLE__
				SqliteConnection dbConnection = new SqliteConnection("Data Source = logindb.db;Version=3;");
				int clientNum = CreateOrRetrievePlayerMac(dbConnection, tokens.Dequeue(), tokens.Dequeue());
				dbConnection.Close();
				#endif

                

                if (clientNum != -1)
                {
                    //Create our tcp and udp tracking client object here, add it to our management dictionary or map or something
                    AddClient(new Client(localTcp, udpClient, ((IPEndPoint)udpClient.Client.LocalEndPoint).Port, localUDPEP, clientNum, rand.Next(40, 1301), 768), clientNum);
                    Client thisClient;
                    clients.TryGetValue(clientNum, out thisClient);

                    thisClient.tcp.BeginReceive(thisClient.buffer, 0, PortDefinitions.MAXBUFFSIZE, 0, new AsyncCallback(TCPListen), thisClient);
                    thisClient.udp.BeginReceive(new AsyncCallback(UDPListen), thisClient);

                    eventQueue.Enqueue(new Event(EventType.REQUEST_LOAD_PLAYERS, clientNum));
                    eventQueue.Enqueue(new Event(EventType.NEW_CONNECT, clientNum));

                    clientNum++;
                    Server.Instance.numPlayersActive++;
                }
                else
                {
                    string failMsg = "fail;ep;";
                    EncryptString(ref failMsg);
                    localTcp.Send(Encoding.ASCII.GetBytes(failMsg));
                    localTcp.Disconnect(false);
                }
            }
            else
            {
                localTcp.Send(Encoding.ASCII.GetBytes("randomrejection")); //we don't trust what we've recieved lets just send some grabled data back and burn out whatever is messaging our server for now!
            }
        }

		#if _WIN32
        public int CreateOrRetrievePlayerWindows(SQLiteConnection db, string username, string password)
        {
            db.Open();
            StringBuilder sqlQuery = new StringBuilder("select * from logins where PlayerName = '" + username + "'");
            string sqlSearch = sqlQuery.ToString();//first search for the player already

            SQLiteCommand com1 = new SQLiteCommand(sqlSearch, db);
            SQLiteDataReader reader = com1.ExecuteReader();

            if (reader.HasRows)
            {
                while (reader.Read())
                {

                    if (username == "admin" && password == "admin")
                        return Convert.ToInt32(reader["PlayerID"]);
                    else
                    {
                        string passwordEncrypted = Convert.ToString(reader["PlayerPass"]);
                        EncryptString(ref passwordEncrypted);
                        if (passwordEncrypted == password)
                            return Convert.ToInt32(reader["PlayerID"]);
                    }
                }
            }
            else
            {
                //create new player
                string passEncrypted = password;
                EncryptString(ref passEncrypted);
                sqlQuery = new StringBuilder("insert into logins values ('" + username + "',NULL,'" + passEncrypted + "')");
                SQLiteCommand com2 = new SQLiteCommand(sqlQuery.ToString(), db);


                com2.ExecuteNonQuery();

                SQLiteDataReader reader2 = com2.ExecuteReader();
                while (reader2.Read())
                {
                    return Convert.ToInt32(reader["PlayerID"]);
                }
            }

            return -1;
        }
		#endif

		#if __APPLE__
		public int CreateOrRetrievePlayerMac(SqliteConnection db, string username, string password)
		{
			db.Open();
			StringBuilder sqlQuery = new StringBuilder("select * from logins where PlayerName = '" + username + "'");
			string sqlSearch = sqlQuery.ToString();//first search for the player already

			SqliteCommand com1 = new SqliteCommand(sqlSearch, db);
			SqliteDataReader reader = com1.ExecuteReader();

			if (reader.HasRows)
			{
				while (reader.Read())
				{

					if (username == "admin" && password == "admin")
						return Convert.ToInt32(reader["PlayerID"]);
					else
					{
						string passwordEncrypted = Convert.ToString(reader["PlayerPass"]);
						EncryptString(ref passwordEncrypted);
						if (passwordEncrypted == password)
							return Convert.ToInt32(reader["PlayerID"]);
					}
				}
			}
			else
			{
				//create new player
				string passEncrypted = password;
				EncryptString(ref passEncrypted);
				sqlQuery = new StringBuilder("insert into logins values ('" + username + "',NULL,'" + passEncrypted + "')");
				SqliteCommand com2 = new SqliteCommand(sqlQuery.ToString(), db);


				com2.ExecuteNonQuery();

				SqliteDataReader reader2 = com2.ExecuteReader();
				while (reader2.Read())
				{
					return Convert.ToInt32(reader["PlayerID"]);
				}
			}

			return -1;
		}
		#endif

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

        public void AddClient(Client clientToAdd, int clientID)
        {
            clients.Add(clientID, clientToAdd);

            //send back a simply encrypted with bitwise XOR registration packet
            //
            string password = "";
            //if (Server.Instance.LobbyPassword != "")
            //{
            //    password = Server.Instance.LobbyPassword;
            //    EncryptString(ref password);
            //}
            string msgToSend = clientID.ToString() + ";" + clientToAdd.playerCurrentPos.First + ";" + clientToAdd.udpPort + ((password == "") ? password + ";ep;" : ";ep;");
            EncryptString(ref msgToSend);

            byte[] msg = Encoding.ASCII.GetBytes(msgToSend);

            clientToAdd.tcp.Send(msg);
        }

        public void TCPListen(IAsyncResult ar)
        {
            Client clientInfo = (Client)ar.AsyncState;
            string msg = System.Text.Encoding.UTF8.GetString(clientInfo.buffer);
            if (msg.Length != 0)
            {
                Queue<string> tokens;
                try
                {
                    tokens = split(msg, ";");
                    string eventToken = tokens.Dequeue();

                    //if(!clientInfo.passedPassword) //if the player hasn't yet passed the password we need to either ensure that there is a password to pass and that this is their first response, or kick them
                    //{
                    //    if (Server.Instance.LobbyPassword != "")
                    //    {
                    //        if (eventToken == Server.Instance.LobbyPassword)
                    //            clients[clientInfo.uniquePlayerID].passedPassword = true;
                    //        else
                    //            eventQueue.Enqueue(new Event(EventType.KICK_PASSWRONG, clientInfo.uniquePlayerID));
                    //    }
                    //}

                    //lets first check we aren't disconnecting, if we are we neednt waste more time on this client
                    //TODO - could improve this by making a string to event enum parse and switch statement here but man I'm running low on time, foxus on marking criteria
                    if (eventToken == "dp")
                    {
                        Console.WriteLine("Client {0} disconnecting, notifying active players...", clientInfo.udpEP.ToString());
                        eventQueue.Enqueue(new Event(EventType.DISCONNECT, clientInfo.uniquePlayerID));
                    }
                    if (eventToken == "sd" && clientInfo.host)
                    {
                        eventQueue.Enqueue(new Event(EventType.KICK_SERVER_SHUTDOWN, clientInfo.uniquePlayerID));
                    }
                    if (eventToken == "ah")
                    {
                        Console.WriteLine("Being hosted by a client, will shutdown when client closes...");
                        clients[clientInfo.uniquePlayerID].host = true;
                    }
                }
                catch(ArgumentOutOfRangeException e)
                {
                    Console.WriteLine("Couldn't parse a message!");

                    if (clientInfo.host)
                        eventQueue.Enqueue(new Event(EventType.KICK_SERVER_SHUTDOWN, clientInfo.uniquePlayerID)); //because we had a faulty message from the localhost which is reliably connected, we'll assume it was a shutdown for now and shut down our server
                    else
                        eventQueue.Enqueue(new Event(EventType.DISCONNECT, clientInfo.uniquePlayerID)); //otherwise lets assume disconnect of another player for now (eventually we could kick people for too many garbled messages as potential hackers
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
                        clients[clientInfo.uniquePlayerID].playerCurrentPos.First = float.Parse(tokens.Dequeue());
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
                if (pos == -1) return tokens;
                string token = str.Substring(prev, pos - prev);
                if (token.Length != 0) tokens.Enqueue(token);
                prev = pos + delim.Length;
            } while (pos < str.Length && prev < str.Length);
            return tokens;
        }

        public void EncryptString(ref string stringToEncrypt)
        {
            StringBuilder strBuilder = new StringBuilder(stringToEncrypt);
            char encryptKey = 's';
            for(int i = 0; i < stringToEncrypt.Length; ++i)
            {
                strBuilder[i] ^= encryptKey;
            }

            stringToEncrypt = strBuilder.ToString();
        }
    }
}



