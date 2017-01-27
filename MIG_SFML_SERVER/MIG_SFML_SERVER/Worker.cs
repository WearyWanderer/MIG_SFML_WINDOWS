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
                                client.tcp.Send(Encoding.ASCII.GetBytes("disconnected;" + task.task.originPlayerID + ";"));

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
                                    clientLoading.tcp.Send(Encoding.ASCII.GetBytes("addplayer;" + c.Value.uniquePlayerID + ";" + c.Value.playerCurrentPos.First + ";"));
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
                                    c.Value.tcp.Send(Encoding.ASCII.GetBytes("addplayer;" + clientLoading.uniquePlayerID + ";" + clientLoading.playerCurrentPos.First + ";"));
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

                            foreach (KeyValuePair<int, Client> c in task.clients)
                            {
                                if (c.Value.uniquePlayerID != clientLoading.uniquePlayerID)
                                    c.Value.tcp.Send(Encoding.ASCII.GetBytes("moveplayer;" + clientLoading.uniquePlayerID + ";" + clientLoading.playerCurrentPos.First + ";" + clientLoading.playerCurrentPos.Second + ";"));
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
