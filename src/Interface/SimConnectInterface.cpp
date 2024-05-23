#include "SimConnectInterface.h"

#include "simstruc.h"

SimConnectInterface::SimConnectInterface(size_t dataSize) {
    this->dataSize = dataSize;
    data = malloc(dataSize);
}

SimConnectInterface::~SimConnectInterface() {
    if (data != nullptr) {
        free(data);
        data = nullptr;
    }
}

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

    if (this->dataSize != size) {
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

bool SimConnectInterface::setClientData(int size, void *pDataSet) {
    if (this->dataSize != size) {
        return false;
    }

    if (pDataSet == nullptr) {
        return false;
    }

    std::memcpy(data, pDataSet, this->dataSize);

    HRESULT result = SimConnect_SetClientData(hSimConnect, 0, 0, SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT, 0, this->dataSize, data);

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
                    // We memcpy the resulting data, so that we can use it even potentially in the next iteration
                    // if the sim has not yet written another Client data frame (In this case the pointer could be invalid).
                    if (data == nullptr) {
                        return;
                    }
                    std::memcpy(data, &event->dwData, this->dataSize);
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
