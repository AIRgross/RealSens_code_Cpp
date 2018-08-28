RealSense SDK 2 Sample Program
==============================
I modified part of the Align sample code made by Tsukasa SUGIURA.
The goal was to output RGB, depths, color depths, and the XYZ 
matrix in order to use the camera on a rover and detect obstacles 
or objects of interest. The issue with the raw output was that 
the depths frame is not aligned with the RGB frame. Also, once 
both aligned, depths frame would show holes inside. I used a 
smoothing function from the realsens library to fill these holes up.

The XYZ matrix link a pixel from the RGB frame to a position in space.
I made this matrix using the dist_3d function from the intel real
sens library. I only constructed this matrix for validation. 
It is not supposed to be used in real time as it may reduce highly the 
speed.

The XYZ matrix corresponds to a 3D RGB matrix with:

- R = x distance in the x-direction with respect to the camera center.
- G = y ...
- B = z ...

To not show the XYZ matrix:
- comment the drawXYW function
- comment the showXYZ function


Reference
---------
* IntelRealSense/librealsense | GitHub  
  <https://github.com/IntelRealSense/librealsense>

* UnaNancyOwen/librealsense_apireference | GitHub Pages  
  <https://unanancyowen.github.io/librealsense2_apireference/>

* AIRgross/Intel-RealSens | GitHub 
  <https://github.com/AIRgross/Intel-RealSens>
