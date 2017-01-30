using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace SERVER
{
    class UdpBroadcaster
    {
        public void broadcastIP()
        {
            bool done = false;

            UdpClient listener = new UdpClient(8080);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, 8080);

            try
            {
                while (!done)
                {
                    Console.WriteLine("Waiting for broadcast");

                    // blocking but fine for broadcasting
                    byte[] buff = listener.Receive(ref groupEP);

                    Console.WriteLine("Received broadcast from {0}", groupEP.ToString());

                    Ping ping = new Ping();
                    PingReply reply = ping.Send(groupEP.Address);
                    
                    Byte[] msg = Encoding.ASCII.GetBytes(Server.Instance.LobbyName + ",      " + Server.Instance.numPlayersActive + "players,       " + reply.RoundtripTime.ToString() + "ms");

                    // The client will get our IP Address here
                    listener.Send(msg, msg.Length, groupEP);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception {0}", e.Message);
            }
            finally
            {
                listener.Close();
            }
        }
    }
}
