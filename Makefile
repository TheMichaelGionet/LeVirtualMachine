
All: Setup TestComponents TestVmPipelines TestVmControlUnit TestVmStages TestVmBusPeripherals TestVirtualMachine LeVirtualMachine

Setup:
	-mkdir debug
	-mkdir bin

TestComponents: TestVmComponents.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmComponents TestVmComponents.cpp

TestVmPipelines: TestVmPipelines.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmPipelines TestVmPipelines.cpp

TestVmControlUnit: TestVmControlUnit.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmControlUnit TestVmControlUnit.cpp

TestVmStages: TestVmStages.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmStages TestVmStages.cpp

TestVmBusPeripherals: TestVmBusPeripherals.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmBusPeripherals TestVmBusPeripherals.cpp

TestVirtualMachine: 
	g++ -std=gnu++11 -g -fpack-struct=1 -o ./debug/TestVirtualMachine TestVirtualMachine.cpp

LeVirtualMachine:
	g++ -std=gnu++11 -O2 -fpack-struct=1 -o ./bin/LeVirtualMachine LeVirtualMachine.cpp

clean:
	-rm *.o
	-rm -r ./debug/
	-rm -r ./bin/

