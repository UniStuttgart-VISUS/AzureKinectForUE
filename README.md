# Azure Kinect for Unreal Engine
This for of [Ayumu Nagamtsu's plugin für UE4](https://github.com/nama-gatsuo/AzureKinectForUE) enables using the Azure Kinect sensor in Unreal Engine 5.

## Prerequisites
* Platform: Win64
* Dependencies:
    * [`Azure Kinect SDK v1.4.1`](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/blob/develop/docs/usage.md) is expected in the default installation directory. If you are using a different one, provide its absolute path in the `AZUREKINECT_SDK` environment variable.
    * [`Azure Kinect Body Tracking SDK v1.1.2`](https://docs.microsoft.com/en-us/azure/Kinect-dk/body-sdk-download) is expected in the default installation directory. If you are using a different one, provide its absolute path in the `AZUREKINECT_BODY_SDK` environment variable.
* Unreal Engine 5.6+
    * Tested with Unreal Engine 5.6.1. May or may not work with previous versions.

## Usage
* Install the prerequisites.
* Clone the repository into the "Plugins" directory of your project.
* Restart Unreal Engine and check the plugin list for "Azure Kinect for Unreal Engine 5".
* The [`UAzureKinectDevice`](Source/UnrealAzureKinect/Public/AzureKinectDevice.h) is the core class implementing all the stuff.
* The `BP_AzureKinectActor` in the plugin's content shows how to put everything together. It uses a default instance of `UAzureKinectDevice` called `DefaultAzureKinectDevice`, also located in the plugin's content folder.
* You can double-click the `DefaultAzureKinectDevice` to test the setup:
   * Attach the Azure Kinect via USB.
   * Click the "Refresh" button at the very top of the device configuration. This should populate the "Device Selection" list below with the serial number of your device.
   * Select the device and configure its behaviour.
   * Click "Start" in the "Camera control" group at the very bottom.
   * The "Start" button should change to a "Stop" button, and you should start seeing updates in the previews of the textures configured above.
   * If nothing happens, that is if the "Start" button remains, the device was not found or could not be started. In this case, make sure that you have selected an existing device in the dropdown at the top and check the "Output Log" for messages.
  
## Debugging
Logging can be enabled for the Azure Kinect SDK by setting the [`K4A_ENABLE_TO_TO_A_FILE`](https://learn.microsoft.com/en-us/previous-versions/azure/kinect-dk/troubleshooting) environment variable and setting the [`K4A_LOG_LEVEL`](https://learn.microsoft.com/en-us/previous-versions/azure/kinect-dk/troubleshooting) variable to one of 'c', 'e', 'w', 'i' or 't'. There are similar, but [undocumented](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/issues/709) variables `K4ABT_ENABLE_LOG_TO_A_FILE`and `K4ABT_LOG_LEVEL` for the body tracking SDK.
