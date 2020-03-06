#pragma once

#include <SSNet/EndPoint.h>
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

bool test()
{
    test_EndPoint();

    return true;
}

} // namespace TestNet