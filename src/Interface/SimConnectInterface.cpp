#include "SimConnectInterface.h"

bool SimConnectInterface::connect(const std::string &name) {
    this->connectionName = name;

    HRESULT result = SimConnect_Open(&hSimConnect, name.c_str(), nullptr, 0, nullptr, 0);

    if (result == S_OK) {
        isConnected = true;

        return true;
    }

    return false;
}

void SimConnectInterface::disconnect() {
    if (isConnected) {
        SimConnect_Close(hSimConnect);
        isConnected = false;
        hSimConnect = nullptr;
    }
}

bool SimConnectInterface::createClientData(int size) {
    if (!isConnected) {
        return false;
    }

    HRESULT result = true;

    result &= SimConnect_MapClientDataNameToID(hSimConnect, connectionName.c_str(), 0);

    result &= SimConnect_CreateClientData(hSimConnect, 0, size, SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT);

    result &= SimConnect_AddToClientDataDefinition(hSimConnect, 0, 0, size);

    if (result == S_OK) {
        return true;
    }

    return false;
}

bool SimConnectInterface::setClientData(int size, void *data) {
    HRESULT result = SimConnect_SetClientData(hSimConnect, 0, 0, SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, size, data);

    if (result == S_OK) {
        return true;
    }

    return false;
}

void* SimConnectInterface::getClientData() {
    processDispatch();

    return data;
}

bool SimConnectInterface::requestClientData() {
    HRESULT result = SimConnect_RequestClientData(hSimConnect, 0, 0, 0, SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET);

    if (result == S_OK) {
        return true;
    }

    return false;
}

bool SimConnectInterface::getIsConnected() {
    return isConnected;
}

void SimConnectInterface::processDispatch() {
    DWORD cbData;
    SIMCONNECT_RECV *pData;
    while (SUCCEEDED(SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData))) {
        dispatchProcedure(pData, &cbData);
    }
}

void SimConnectInterface::dispatchProcedure(SIMCONNECT_RECV *pData, DWORD *cbData) {
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_CLIENT_DATA: {
            auto *event = (SIMCONNECT_RECV_CLIENT_DATA*)pData;
            switch (event->dwRequestID) {
                case 0:
                    data = &event->dwData;
                    break;
            }
            break;
        }
        case SIMCONNECT_RECV_ID_QUIT: {
            disconnect();
            break;
        }
        case SIMCONNECT_RECV_ID_EXCEPTION: {
            // TODO Handle exception/throw error
            break;
        }

    }
}
