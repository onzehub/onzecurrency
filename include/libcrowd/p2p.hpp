#pragma once

#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>

#include <boost/array.hpp>

using namespace std; 
using namespace boost::asio; 
using namespace boost::asio::ip;

namespace Crowd
{
    class P2p
    {
    public:
        bool IsUpnp();
    };
    
    class Udp
    {
    public:
        int udp_server();
        int udp_client(std::string srv_ip, std::string message); // TODO: add a reference & to these strings
    protected:
        struct client
        {
            boost::asio::ip::address host;
            unsigned short port;
        };
    private:
        int die() // kill client/server if there's an error
        {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return errno;
        }
    };

    class Upnp
    {
    public:
        int Upnp_main();
    private:
        const char * protofix(const char * proto);
        static int SetRedirectAndTest(struct UPNPUrls * urls,
			       struct IGDdatas * data,
			       const char * iaddr,
			       const char * iport,
			       const char * eport,
			       const char * proto,
			       const char * leaseDuration,
			       const char * remoteHost,
			       const char * description,
			       int addAny);
    };

    class Protocol
    {
    public:
        int hello_and_setup(std::string& my_user_login_hash);
        std::string latest_block();
        std::map<std::string, uint32_t> layer_management(std::string total_amount_of_peers);
    private:
        int verify_latest_block(std::string latest_block_peer);
        int communicate_to_all(boost::array<char, 128> msg);
    };
}

extern "C"
{
    int ParseIPv4OrIPv6 ( const char** ppszText, unsigned char* abyAddr, int* pnPort, int* pbIsIPv6 );
}