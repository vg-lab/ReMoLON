#ifndef __REMOLON_SESSIONMANAGER_H__
#define __REMOLON_SESSIONMANAGER_H__

#include <unordered_map>
#include <mutex>

#include "StreamingSession.h"

#define SESSION_LAUNCH_OK 0
#define SESSION_LAUNCH_DUPLICATE_NAME 1
#define SESSION_LAUNCH_PORT_UNAVAILABLE 2
#define SESSION_LAUNCH_CRASHED 3

namespace remolon
{
  typedef std::unordered_map < std::string, StreamingSession > sessionList;

  class SessionManager
  {
    public:
      static SessionManager & getInstance ( );

      void setPublicAddress ( const std::string & address_ );
      void addAvailablePort ( uint16_t port_ );
      void addAvailableSocketPort ( uint16_t port_ );
      void addAvailableRTCPort ( uint16_t port_ );

      void startNewSession ( const std::string & ownerName_,
                             const std::string & ownerAddress_,
                             const std::string & sessionName_,
                             uint16_t port_ );

      void finishSession ( const std::string & ownerName_,
                           const std::string & sessionName_ );

      StreamingSession * getStreamSession ( const std::string & owernName_,
                                            const std::string & sessionName_ );

      const std::string & getPublicAddress ( );
      const std::unordered_map < uint16_t, bool > & getPortList ( );
      const std::unordered_map < uint16_t, bool > & getSocketPortList ( );
      const std::unordered_map < uint16_t, bool > & getRTCPortList ( );

    private:
      uint16_t findAvailablePort ( std::unordered_map < uint16_t, bool> & source_ );

      static SessionManager _INSTANCE;

      std::string _publicAddress;
      std::mutex _mtx;
      std::unordered_map < uint16_t, bool > _usedPorts;
      std::unordered_map < uint16_t, bool > _usedSockPorts;
      std::unordered_map < uint16_t, bool > _usedRTCPorts;
      std::unordered_map < std::string, sessionList > _streamSessions;  
  };
}

#endif