#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <librealsense2/h/rs_sensor.h>
#include <opencv2/opencv.hpp>


#define _USE_MATH_DEFINES
#include <math.h>
#include <queue>
#include <unordered_set>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>

class RealSense
{

private:
    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset aligned_frameset;

    // Color Buffer
    rs2::frame color_frame;
    cv::Mat color_mat;
    cv::Mat scale_mat;
    uint32_t color_width = 640;
    uint32_t color_height = 480;
    uint32_t color_fps = 30;

    // Depth Buffer
    rs2::frame depth_frame;
    rs2::frame depth_color_frame;
 
    rs2::colorizer color_map;
    rs2::spatial_filter spat;
    rs2::temporal_filter temp;
    rs2::decimation_filter dec;
 

    cv::Mat depth_mat;
    cv::Mat depth_color_mat;
    cv::Mat xyz_mat;

    uint32_t depth_width = 640;
    uint32_t depth_height = 480;
    uint32_t depth_fps = 30;

public:
    // Constructor
    RealSense();

    // Destructor
    ~RealSense();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Color
    inline void updateColor();

    // Update Depth
    inline void updateDepth();

    inline void updateDepthColor();

    // Draw Data
    void draw();

    // Draw Color
    inline void drawColor();

    // Draw Depth
    inline void drawDepth();

    // Draw Depth Color
    inline void drawDepthColor(); 

    inline void drawXYZ();

    // Show Data
    void show();

    // Show Color
    inline void showColor();

    // Show Depth
    inline void showDepth();

    // Show DepthColor
    inline void showDepthColor();

    // Show XYZColor   
    inline void showXYZ();

//    input a pixel, output a point (x,y,z).
//    x = distance to center to the camera in the x direction
//    y ...
//    z ...
//    void dist_3d_mod(const rs2::depth_frame& frame, float* upixel, float* upoint);

};

#endif // __REALSENSE__
