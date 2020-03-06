//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include <SSBase/Object.h>
#include <SSBase/Ptr.h>

namespace TestSignal
{

std::stringstream gOutput;

class Switcher : public ss::Object
{
    SS_OBJECT(Switcher, ss::Object);

public:
};

class Light : public ss::Object
{
    SS_OBJECT(Light, ss::Object);

public:
    explicit Light(int n) : n_(n)
    {
        static int counter = 1;
        id_ = counter++;
    }

public:
    void handleSwitch(ss::VariantMap &params)
    {
        count_++;
        if (count_ == n_)
        {
            isOn_ = true;
            count_ = 0;
        }
        else
        {
            isOn_ = false;
        }
        gOutput << "Toggle light " << id_ << " to " << (isOn_ ? "on" : "off") << std::endl;
    }

    int n_;
    int id_;
    int count_{0};
    bool isOn_{true};
};

class Fan : public ss::Object
{
    SS_OBJECT(Light, ss::Object);

public:
    void handleSwitch(ss::VariantMap &params)
    {
        isOn_ = !isOn_;
        gOutput << "Toggle fan " << (isOn_ ? "on" : "off") << ", And greetings: " << params["msg"].GetString()
                  << std::endl;
    }

    bool isOn_{false};
};

bool test()
{
    {
        ss::SharedPtr<Switcher> switcher = ss::MakeShared<Switcher>();
        ss::SharedPtr<Light> light1 = ss::MakeShared<Light>(2);
        ss::SharedPtr<Light> light2 = ss::MakeShared<Light>(3);
        ss::SharedPtr<Fan> fan = ss::MakeShared<Fan>();
        switcher->SignalConnect("toggle", light1, &Light::handleSwitch);
        switcher->SignalConnect("toggle", light2, &Light::handleSwitch);
        switcher->SignalConnect("toggle", fan, &Fan::handleSwitch);

        ss::VariantMap params;
        params["msg"] = "'Greeting msg!'";
        for (int i = 0; i < 10; ++i)
        {
            gOutput << "======" << i << std::endl;
            if (i == 5)
            {
                switcher->SignalDisconnect("toggle", fan);
            }
            if (i == 6)
            {
                light2 = nullptr;
            }
            switcher->SignalEmit("toggle", params);
        }
    }

    std::string kResult = R"(======0
Toggle light 1 to off
Toggle light 2 to off
Toggle fan on, And greetings: 'Greeting msg!'
======1
Toggle light 1 to on
Toggle light 2 to off
Toggle fan off, And greetings: 'Greeting msg!'
======2
Toggle light 1 to off
Toggle light 2 to on
Toggle fan on, And greetings: 'Greeting msg!'
======3
Toggle light 1 to on
Toggle light 2 to off
Toggle fan off, And greetings: 'Greeting msg!'
======4
Toggle light 1 to off
Toggle light 2 to off
Toggle fan on, And greetings: 'Greeting msg!'
======5
Toggle light 1 to on
Toggle light 2 to on
======6
Toggle light 1 to off
======7
Toggle light 1 to on
======8
Toggle light 1 to off
======9
Toggle light 1 to on
)";
    return true;
}

} // namespace TestSignal