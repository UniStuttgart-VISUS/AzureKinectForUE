# Azure Kinect for Unreal Engine
This for of [Ayumu Nagamtsu' plugin für UE4](https://github.com/nama-gatsuo/AzureKinectForUE) enables using the Azure Kinect sensor in Unreal Engine 5.

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
