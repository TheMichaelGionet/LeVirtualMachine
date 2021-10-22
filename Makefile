
All: Setup TestComponents TestVmPipelines TestVmControlUnit TestVmStages

Setup:
	-mkdir debug

TestComponents: TestVmComponents.cpp
	g++ -std=gnu++11 -o ./debug/TestVmComponents TestVmComponents.cpp

TestVmPipelines: TestVmPipelines.cpp
	g++ -std=gnu++11 -o ./debug/TestVmPipelines TestVmPipelines.cpp

TestVmControlUnit: TestVmControlUnit.cpp
	g++ -std=gnu++11 -o ./debug/TestVmControlUnit TestVmControlUnit.cpp

TestVmStages: TestVmStages.cpp
	g++ -std=gnu++11 -o ./debug/TestVmStages TestVmStages.cpp

clean:
	-rm *.o ./debug/TestVmComponents ./debug/TestVmPipelines ./debug/TestVmControlUnit ./debug/TestVmStages
	-rmdir debug
