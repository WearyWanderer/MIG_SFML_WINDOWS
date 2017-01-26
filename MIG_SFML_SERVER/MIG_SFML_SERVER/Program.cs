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
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 2) // Test for correct # of args
            {
                Server.Instance.InitServer(args[0], args[1]);
            }
            else
            {
                string name, pass;
                Console.WriteLine("Please enter a lobby name and press enter: ");
                name = Console.ReadLine();
                Console.WriteLine("Please enter a password and press enter (or just enter if no password desired): ");
                pass = Console.ReadLine();

                Server.Instance.InitServer(name, pass);
            }
        }
    }
}
