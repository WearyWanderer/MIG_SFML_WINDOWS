using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SERVER
{
    class WorkerTask
    {
        public Event task;
        public Dictionary<int, Client> clients;

        public WorkerTask(Event request, ref Dictionary<int, Client> connectedClients)
        {
            task = request;
            clients = connectedClients;
        }
    }

    class WorkerExecution
    {
        static public void DoWork(WorkerTask task)
        {
            //Where we do our actual work
            switch(task.task.type)
            {
                case EventType.DISCONNECT:
                    {
                        Client clientDisconnecting;
                        try
                        {
                            task.clients.TryGetValue(task.task.originPlayerID, out clientDisconnecting);
                            clientDisconnecting.tcp.Shutdown(System.Net.Sockets.SocketShutdown.Both);
                            clientDisconnecting.tcp.Close();
                            clientDisconnecting.udp.Close();
                            task.clients.Remove(task.task.originPlayerID);
                            foreach (KeyValuePair<int, Client> c in task.clients)
                            {
                                Client client = c.Value;
                                client.tcp.Send(Encoding.ASCII.GetBytes("dp;" + task.task.originPlayerID + ";ep;"));

                            }
                        }
                        catch (ArgumentNullException e)
                        {
                            Console.Write("Error dispatching task: ", e.Message);
                        }

                        
                        break;
                    }
                case EventType.REQUEST_LOAD_PLAYERS:
                    {
                        Client clientLoading;
                        try
                        {
                            task.clients.TryGetValue(task.task.originPlayerID, out clientLoading);

                            foreach (KeyValuePair<int, Client> c in task.clients)
                            {
                                if (c.Value.uniquePlayerID != clientLoading.uniquePlayerID)
                                    clientLoading.tcp.Send(Encoding.ASCII.GetBytes("ap;" + c.Value.uniquePlayerID + ";" + c.Value.playerCurrentPos.First + ";ep;"));
                            }
                        }
                        catch(ArgumentNullException e)
                        {
                            Console.Write("Error dispatching task: ", e.Message);
                        }
                        
                        break;
                    }
                case EventType.NEW_CONNECT:
                    {
                        Client clientLoading;
                        try
                        {
                            task.clients.TryGetValue(task.task.originPlayerID, out clientLoading);

                            foreach (KeyValuePair<int, Client> c in task.clients)
                            {
                                if(c.Value.uniquePlayerID != clientLoading.uniquePlayerID)
                                    c.Value.tcp.Send(Encoding.ASCII.GetBytes("ap;" + clientLoading.uniquePlayerID + ";" + clientLoading.playerCurrentPos.First + ";ep;"));
                            }
                        }
                        catch (ArgumentNullException e)
                        {
                            Console.Write("Error dispatching task: ", e.Message);
                        }
                        break;
                    }
                case EventType.POSITION_UPDATE:
                    {
                        Client clientLoading;
                        try
                        {
                            task.clients.TryGetValue(task.task.originPlayerID, out clientLoading);
                            Console.WriteLine("PositionUpdate");
                            foreach (KeyValuePair<int, Client> c in task.clients)
                            {
                                if (c.Value.uniquePlayerID != clientLoading.uniquePlayerID)
                                {
                                    byte[] dgram = Encoding.ASCII.GetBytes("mp;"
                                                                            + clientLoading.uniquePlayerID + ";"
                                                                            + (int)clientLoading.state + ";ep;");
                                    c.Value.udp.Send(dgram, dgram.Length, c.Value.udpEP);
                                }
                            }
                        }
                        catch (ArgumentNullException e)
                        {
                            Console.Write("Error dispatching task: ", e.Message);
                        }
                        break;
                    }
                case EventType.KICK_LATENCY:
                    {

                        break;
                    }
                case EventType.KICK_TIMEOUT:
                    {

                        break;
                    }
            }

           
        }
    }
}
