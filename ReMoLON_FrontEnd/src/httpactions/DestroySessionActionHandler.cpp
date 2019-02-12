#include "httpactions/DestroySessionActionHandler.h"

#include <fstream>

#include <Poco/Net/HTMLForm.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/IPAddress.h>

#include "UserManager.h"
#include "SessionTracker.h"

namespace remolonFrontend
{
  namespace httpactions
  {
    std::string DestroySessionActionHandler::getAction ( )
    {
      return "DestroySession";
    }

    void DestroySessionActionHandler::handleAction ( Poco::Net::HTTPServerRequest & request_, 
														                         Poco::Net::HTTPServerResponse & response_ )
    {
      const Poco::Net::SocketAddress & sAddr = request_.clientAddress ( );
			Poco::Net::IPAddress ipAddr = sAddr.host ( );
			std::string ipAddrStr = ipAddr.toString ( );

			response_.setStatus ( Poco::Net::HTTPResponse::HTTP_OK );

			if( !UserManager::getInstance ( ).isUserLoggedIn ( ipAddrStr ) )
			{
				std::ifstream file ( "client/auth.html", std::ios::binary );
				response_.setContentType ( "text/html" );
				Poco::StreamCopier::copyStream ( file, response_.send ( ) );
			}
			else
			{
        User * usr = UserManager::getInstance ( ).getUser ( ipAddrStr );

				Poco::Net::HTMLForm form (request_, request_.stream ( ) );
				std::string sessionName = form.get ( "id", "" );

        SessionTracker::getInstance ( ).tryFinishSession ( *usr, sessionName );
      }
    }
  }
}