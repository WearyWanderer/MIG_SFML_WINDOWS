using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SERVER
{
    enum EventType
    {
        POSITION_UPDATE,
        REQUEST_LOAD_PLAYERS,
        NEW_CONNECT,
        DISCONNECT,
        KICK_TIMEOUT,
        KICK_LATENCY,
        KICK_SERVER_SHUTDOWN,
        KICK_PASSWRONG
    }
    class Event
    {
        public EventType type;
        public int originPlayerID;

        public Event(EventType eventToBeHandled, int playerSendingEvent)
        {
            type = eventToBeHandled;
            originPlayerID = playerSendingEvent;
        }
    }
}
