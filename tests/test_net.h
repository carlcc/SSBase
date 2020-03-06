#pragma once

#include <SSNet/EndPoint.h>
#include <SSNet/Loop.h>
#include <SSNet/TcpSocket.h>
#include <thread>
#include <uv.h>

namespace TestNet
{

using namespace ss;

void test_EndPoint()
{
    {
        EndPoint ep("1.2.3.4:13");
        SSASSERT(ep.IP() == "1.2.3.4");
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("1.2.3.4::13"); // invalid ip
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("1.2.3.4:0"); // invalid port
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("1.3.4::13"); // invalid ip
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("1.3.256.4::13"); // invalid ip
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }

    {
        EndPoint ep("1.2.3.4", 13);
        SSASSERT(ep.IP() == "1.2.3.4");
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());

        EndPoint ep2(std::move(ep)); // test move constructor
        SSASSERT(!ep.IsIPV6());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsValid());
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep2.IP() == "1.2.3.4");
        SSASSERT(ep2.Port() == 13);
        SSASSERT(ep2.IsValid());
        SSASSERT(ep2.IsIPV4());
        SSASSERT(!ep2.IsIPV6());

        EndPoint ep3(ep2); // test copy constructor
        SSASSERT(ep2.IP() == "1.2.3.4");
        SSASSERT(ep2.Port() == 13);
        SSASSERT(ep2.IsValid());
        SSASSERT(ep2.IsIPV4());
        SSASSERT(!ep2.IsIPV6());
        SSASSERT(ep3.IP() == "1.2.3.4");
        SSASSERT(ep3.Port() == 13);
        SSASSERT(ep3.IsValid());
        SSASSERT(ep3.IsIPV4());
        SSASSERT(!ep3.IsIPV6());
    }

    {
        EndPoint ep("1.2.3.4", 13);
        SSASSERT(ep.IP() == "1.2.3.4");
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());

        EndPoint ep2;
        ep2 = std::move(ep); // test move =
        SSASSERT(!ep.IsIPV6());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsValid());
        SSASSERT(ep.IP().Empty());
        SSASSERT(ep2.IP() == "1.2.3.4");
        SSASSERT(ep2.Port() == 13);
        SSASSERT(ep2.IsValid());
        SSASSERT(ep2.IsIPV4());
        SSASSERT(!ep2.IsIPV6());

        EndPoint ep3;
        ep3 = ep2; // test copy =
        SSASSERT(ep2.IP() == "1.2.3.4");
        SSASSERT(ep2.Port() == 13);
        SSASSERT(ep2.IsValid());
        SSASSERT(ep2.IsIPV4());
        SSASSERT(!ep2.IsIPV6());
        SSASSERT(ep3.IP() == "1.2.3.4");
        SSASSERT(ep3.Port() == 13);
        SSASSERT(ep3.IsValid());
        SSASSERT(ep3.IsIPV4());
        SSASSERT(!ep3.IsIPV6());
    }

    {
        EndPoint ep("www.github.com", 13); // invalid ip
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("1.2.3.4.5", 13); // invalid ip
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("::12::1", 13); // invalid ip
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep(":::12:1", 13); // invalid ip
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("[::1]:13");
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(ep.IsIPV6());
        SSASSERT(ep.IP() == "::1");
    }
    {
        EndPoint ep("::1]:13");
        SSASSERT(ep.Port() == 0);
        SSASSERT(!ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(!ep.IsIPV6());
    }
    {
        EndPoint ep("::1", 13);
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(ep.IsIPV6());
        SSASSERT(ep.IP() == "::1");
    }
    {
        EndPoint ep("2001:DB8:2de:000:000:000:000:e13", 13);
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(ep.IsIPV6());
        SSASSERT(ep.IP() == "2001:db8:2de::e13");
    }
    {
        EndPoint ep("2001:000:000:123f:000:000:000:e13", 13);
        SSASSERT(ep.Port() == 13);
        SSASSERT(ep.IsValid());
        SSASSERT(!ep.IsIPV4());
        SSASSERT(ep.IsIPV6());
        SSASSERT(ep.IP() == "2001:0:0:123f::e13");
    }
}

void test_TcpSocket_client()
{
    Loop loop;
    auto client = loop.CreateTcpSocket();
    client->Connect("127.0.0.1", 1234, [client](int status) {
        SSASSERT(status == 0);
        client->StartReceive([client](ssize_t nread, const char *buf) {
            if (nread < 0)
            {
                std::cout << "Client Get: error" << std::endl;
                return;
            }

            std::string msg(buf, nread);
            std::cout << "Client Get: " << msg << std::endl;

            client->Send("bye", 3, [](int status) { SSASSERT(status == 0); });
        });
        client->Send("Hello, world!", 13, [](int status) { SSASSERT(status == 0); });
    });

    loop.Run();

    std::cout << "Client exit" << std::endl;
}

void test_TcpSocket()
{
    std::thread clientThread([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        test_TcpSocket_client();
    });

    // Server thread
    Loop loop;

    auto serverSocket = loop.CreateTcpSocket();
    SSASSERT(serverSocket != nullptr);
    SSASSERT(serverSocket->Bind("0.0.0.0:1234"));
    serverSocket->Listen(100, [](TcpSocket *server, int status) {
        SSASSERT(status == 0);
        auto client = server->Accept();
        SSASSERT(client != nullptr);

        client->StartReceive([client, server](ssize_t nread, const char *buf) {
            if (nread < 0)
            {
                std::cout << "Server Get: error" << std::endl;
                return;
            }
            std::string msg = std::string(buf, nread);

            if (msg == "bye")
            {
                client->Close(nullptr);
                server->Close(nullptr);
                return;
            }

            char *buffer = new char[1024];
            memcpy(buffer, buf, nread);
            memcpy(buffer + nread, buf, nread);
            std::cout << "Server Get: " << msg << std::endl;
            client->Send(buffer, 2 * nread, [buffer](int status) { delete[] buffer; });
        });
    });

    loop.Run();

    std::cout << "Server exit" << std::endl;
    clientThread.join();
}

bool test()
{
    test_EndPoint();

    test_TcpSocket();

    return true;
}

} // namespace TestNet