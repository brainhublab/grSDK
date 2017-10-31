Welcome to GR SDK Documentation! {#mainpage}
=========
The GR SDK`s function is to be a library layer (.so .dll), which layer provides easy communication with GR palm-bracelets. The SDK will also implement the basic features that can be achieved in device interaction.

---

// HERE WE CAN MAKE ONLY LINKS TO PAGES with decomposition, licenses, etc.

## Decomposition

The main ones (there are also auxiliary classes and libraries but you can see them in the description of the code) modules of which are shown in the diagram below

// TODO: make a diagram
![](https://cloud.brainhub.co/index.php/s/hHS99USLSVajsWE/download)

#### Modules:
__GRConnection__  - provides methods for connection and receiving data from one or more GR devices

__GRAlgorithm__ - provides low-level algorithm for aggregating and preconditioning of received data 

* __GRMadgwick__ - provides calculation of rotation from IMU data

* __GRImuPrecondition__ - provides methods for low-level aggregation of imu data

__GRUtils__ - provides higher level of algorithm for aggregation of data
* __GRTrajectory__ - provides extraction of trajectory from precondition IMU data 
    *  __GRImuTrajectory__ - provides calculation of trajectory only with IMU data
    * __GRBinOcularTrajectory__ - provides calculation of trajectory with IMU and tracking of IR led`s

__samples__ - provides examples of usage GR SDK
* __GRVisualization__ - provides OpenGL visualization of rotations and trajectory using GR SDK


---

## Licensing
We`re using **NAME** license. [Click here](@ref licensepage) for more information

