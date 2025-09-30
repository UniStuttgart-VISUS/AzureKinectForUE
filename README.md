# Azure Kinect for Unreal Engine
This for of [Ayumu Nagamtsu' plugin für UE4](https://github.com/nama-gatsuo/AzureKinectForUE) enables using the Azure Kinect sensor in Unreal Engine 5.

## Prerequisites
* Platform: Win64
* Dependencies:
    * `Azure Kinect SDK v1.4.1` is installed
        * Download from [here](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/blob/develop/docs/usage.md)
        * The plugin expects the default installation directory. If you are using a different one, provide its absolute path in the `AZUREKINECT_SDK` environment variable.
    * `Azure Kinect Body Tracking SDK v1.1.2` is installed
        * Download from [here](https://docs.microsoft.com/en-us/azure/Kinect-dk/body-sdk-download)
        * The plugin expects the default installation directory. If you are using a different one, provide its absolute path in the `AZUREKINECT_BODY_SDK` environment variable.
* Unreal Engine 5.6+
    * Only tested with 5.6. May or may not work with previous versions.

## Usage