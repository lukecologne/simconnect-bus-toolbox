#pragma once

class SimConnectInterface {
public:
    SimConnectInterface() = default;

    ~SimConnectInterface() = default;

    bool connect();

    void disconnect();
};
