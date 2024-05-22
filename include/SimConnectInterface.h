#pragma once

#include <string>

#include "Windows.h"
#include "SimConnect.h"

class SimConnectInterface {
public:
    SimConnectInterface() = default;

    ~SimConnectInterface() = default;

    bool connect(const std::string &name);

    void disconnect();

    bool createClientData(int size);

    bool setClientData(int size, void *data);

    void* getClientData();

    bool requestClientData();

    bool getIsConnected();

private:
    bool isConnected = false;
    HANDLE hSimConnect = nullptr;
    std::string connectionName;

    void *data;

    void processDispatch();

    void dispatchProcedure(SIMCONNECT_RECV *pData, DWORD *cbData);
};
