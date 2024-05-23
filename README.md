# SimConnect Bus Toolbox

## Overview

SimConnect Bus Toolbox is a MATLAB/Simulink Toolbox for allowing easy transfer of Simulink buses into Microsoft Flight Simulator.
The Toolbox contains the Block SimConnectBusInterface, which can act both as Sink and Source.

## Building

### Prerequisites 

* MATLAB/Simulink (tested with R2023b)
* Microsoft Flight Simulator SDK
* CMake
* A C++ Compiler (tested with Visual Studio 2022)

### How to Build

```lang bash
git clone https://github.com/lukecologne/simconnect-bus-toolbox
cd simconnect-bus-toolbox
cmake -S . -B build
cmake --build build --config Release
```

### Adding the block to Simulink

Add the `<simconnect-bus-toolbox>/matlab` directory to you MATLAB path, then add the Block to your model from the Simulink Block Library.

## Using the block

To use the block, first add it to your model. It can then be configured via the mask:

![mask](https://github.com/lukecologne/simconnect-bus-toolbox/assets/39596827/7d4782d1-87d6-449e-9c7e-f27d97950003)

* The `Connection Name` Entry specifies the name of the client data area registered to SimConnect.
* The `Is Sink` option specifies wether the block should act as source (request data from SimConnect) or sink (send data to SimConnect).
* The `Bus Data Type` option specifies the data type of the in-/output port. It must be a valid Simulink Bus type.

Now, when running the model with MSFS open, the block will connect to SimConnect, register a client data area with the size of the specified
Bus Type, and then either write or read data to/from the data area. The data is taken/sent to directly from/to Simulink via a raw buffer, which
is structured like a normal C++ struct with the fields of the bus type.

When receiving/sending the data in another SimConnect Client, you should provide a header file with the corresponding fields of the Bus to cast to.
It is recommended to use Simulink Coder to generate the header files for maximum ease of use.
