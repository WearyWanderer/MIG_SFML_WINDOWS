using System; // For Console, Int32, ArgumentException, Environment
using System.Net; // For IPAddress
using System.Net.Sockets; // For TcpListener, TcpClient
using System.Text;

namespace SERVER
{
    class ReactorListenerServer
    {
        const int BUFSIZE = 32;

        public void ListenForClients()
        {
            TcpListener listener = null;

            try
            {
                // Create a TCPListener to accept client connections
                listener = new TcpListener(IPAddress.Any, 8080);
                listener.Start();
            }
            catch (SocketException se)
            {
                Console.WriteLine(se.ErrorCode + ": " + se.Message);
                Environment.Exit(se.ErrorCode);
            }

            byte[] rcvBuffer = new byte[BUFSIZE]; // Receive buffer
            int bytesRcvd; // Received byte count


            for (;;)
            {   // Run forever, accepting and servicing connections
                Console.WriteLine(IPAddress.Any);
                Socket client = null;
                
                try
                {
                    client = listener.AcceptSocket(); //Get client connection
                    client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.KeepAlive, true);
                    Console.Write("Handling client - ");

                    client.Receive(rcvBuffer);
                    
                    Console.WriteLine("Recieved - {0}", new String(Encoding.ASCII.GetChars(rcvBuffer)));
                    // Close the stream and socket. We are done with this client!
                    client.Close();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }


            }
        }
    }
}



