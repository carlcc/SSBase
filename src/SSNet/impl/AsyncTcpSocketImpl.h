//
// Copyright (c) 2020 Carl Chen. All rights reserved.
//

#pragma once

#include "../AsyncTcpSocket.h"
#include <uv.h>

namespace ss {

class Loop;

class AsyncTcpSocketImpl : public AsyncTcpSocket {
    SS_OBJECT(AsyncTcpSocketImpl, AsyncTcpSocket);

public:
    // NOTE: &Data must equals to &Data::handle_
    struct Data {
        Data()
        {
            bufferSize_ = 0x10000;
            buffer_ = new uint8_t[bufferSize_];
        }
        ~Data()
        {
            SetUserData(nullptr, nullptr);
            delete[] buffer_;
        }

        void SetUserData(void* data, OnReleaseData&& f) {
            if (onReleaseUserData_ != nullptr) {
                onReleaseUserData_(userData_);
            }
            userData_ = data;
            onReleaseUserData_ = std::forward<OnReleaseData>(f);
        }

        uv_tcp_t handle_ {};
        OnDataCb onDataCb_ { nullptr };
        OnCloseCb onCloseCb_ { nullptr };
        OnConnectionCb onConnectionCb { nullptr };
        AsyncTcpSocketImpl* self_ { nullptr };
        void* userData_ { nullptr };
        OnReleaseData onReleaseUserData_ { nullptr };
        uint8_t* buffer_ { nullptr };
        uint32_t bufferSize_ { 0 };
        bool bufferUsed_ { false };
    };

    // This object will take the ownship of handle
    explicit AsyncTcpSocketImpl(Data* handle);
    AsyncTcpSocketImpl(const AsyncTcpSocketImpl&) = delete;
    AsyncTcpSocketImpl(AsyncTcpSocketImpl&&) = delete;
    ~AsyncTcpSocketImpl() override;

    AsyncTcpSocketImpl& operator=(const AsyncTcpSocketImpl&) = delete;
    AsyncTcpSocketImpl& operator=(AsyncTcpSocketImpl&&) = delete;

    int Connect(const EndPoint& ep, OnConnectCb&& cb) override;

    int Bind(const EndPoint& ep) override;

    int Listen(int backlog, OnConnectionCb&& cb) override;

    EndPoint GetPeer() const override;

    SharedPtr<AsyncTcpSocket> Accept() override;

    int Send(const void* data, uint32_t length, OnSendCb&& cb) override;

    int StartReceive(OnDataCb&& cb) override;

    void StopReceive() override;

    void Close(OnCloseCb&& cb) override;

    int ShutDown(OnShutDownCb&& cb) override;

    void* GetUserData() const override
    {
        return data_->userData_;
    }
    void SetUserData(void* d, OnReleaseData&& onRelease) const override
    {
        data_->SetUserData(d, std::forward<OnReleaseData>(onRelease));
        data_->userData_ = d;
    }

private:
    Data* data_;
};

} // namespace ss