
All: TestComponents TestVmPipelines TestVmControlUnit


TestComponents: TestVmComponents.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmComponents TestVmComponents.cpp

TestVmPipelines: TestVmPipelines.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmPipelines TestVmPipelines.cpp

TestVmControlUnit: TestVmControlUnit.cpp
	g++ -std=gnu++11 -fpack-struct=1 -o ./debug/TestVmControlUnit TestVmControlUnit.cpp

clean:
	-rm *.o ./debug/TestVmComponents ./debug/TestVmPipelines ./debug/TestVmControlUnit
