
All: Setup TestComponents TestVmPipelines TestVmControlUnit TestVmStages TestVmBusPeripherals

Setup:
	-mkdir debug

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

clean:
	-rm *.o
	-rm -r ./debug/
