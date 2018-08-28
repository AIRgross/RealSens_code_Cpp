#include "realsense.h"

// Constructor
RealSense::RealSense()
{
    // Initialize
    initialize();
}

// Destructor
RealSense::~RealSense()
{
    // Finalize
    finalize();
}

// Processing
void RealSense::run()
{
    // Main Loop
    while( true ){
        // Update Data
        update();

        // Draw Data
        draw();

        // Show Data
        show();

        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
	
	
    }
}

// Initialize
void RealSense::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Sensor
    initializeSensor();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_COLOR, color_width, color_height, rs2_format::RS2_FORMAT_BGR8, color_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_DEPTH, depth_width, depth_height, rs2_format::RS2_FORMAT_Z16, depth_fps );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Finalize
void RealSense::finalize()
{
    // Close Windows
    cv::destroyAllWindows();

    // Stop Pipline
    pipeline.stop();
}

// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Color
    updateColor();

    // Update Depth
    updateDepth();
 
    // Update DepthColor()
    updateDepthColor();
 
 


}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    rs2::frameset frameset = pipeline.wait_for_frames();
    dec.set_option(RS2_OPTION_FILTER_MAGNITUDE,2);

    // Retrieve Aligned Frame
    rs2::align align( rs2_stream::RS2_STREAM_COLOR );
    aligned_frameset = align.process( frameset );


    if( !aligned_frameset.size() ){
        return;
    }

    spat.set_option(RS2_OPTION_HOLES_FILL,4);

}

// Update Color
inline void RealSense::updateColor()
{
    // Retrieve Color Frame
    color_frame = aligned_frameset.get_color_frame();
 
    // Retrive Frame Information
    color_width  = color_frame.as<rs2::video_frame>().get_width();
    color_height = color_frame.as<rs2::video_frame>().get_height();
}

// Update Depth
inline void RealSense::updateDepth()
{
    // Retrieve Depth Frame
 
    depth_frame = aligned_frameset.get_depth_frame();
 

    depth_frame = dec.process(depth_frame);

    depth_frame = spat.process(depth_frame);

    // Retrive Frame Information
    depth_width  = depth_frame.as<rs2::video_frame>().get_width();
    depth_height = depth_frame.as<rs2::video_frame>().get_height();

}

 // Update Depth
inline void RealSense::updateDepthColor()
{
    // Retrieve Depth color Frame
    depth_color_frame = rs2::colorizer().colorize(depth_frame);

}


// Draw Data
void RealSense::draw()
{
    // Draw Color
    drawColor();

    // Draw Depth
    drawDepth();

    // Draw color Depth
    drawDepthColor();

    // Draw color Depth
    drawDepthColor();

    drawXYZ();

}

// Draw Color
inline void RealSense::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) );

}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16UC1, const_cast<void*>( depth_frame.get_data() ) );
}

// Draw Depth
inline void RealSense::drawDepthColor()
{
    // Create cv::Mat form Depth Frame    
    depth_color_mat = cv::Mat( depth_height, depth_width, CV_8UC3, const_cast<void*>( depth_color_frame.get_data() ) );

    
}

inline void RealSense::drawXYZ()
{
    // Create cv::Mat form XYZ Frame  
    xyz_mat  = cv::Mat( depth_mat.rows, depth_mat.cols, CV_32FC3 );
    
}


// Show Data
void RealSense::show()
{
    // Show Color
    showColor();

    // Show Depth
    showDepth();

    // Show Depth Color
    showDepthColor();

    // Show XYZ Color
    showXYZ();
}

// Show Color
inline void RealSense::showColor()
{
    if( color_mat.empty() ){
        return;
    }

    // Show Color Image
    cv::imshow( "Color", color_mat );
}

// Show Depth
inline void RealSense::showDepth()
{
    if( depth_mat.empty() ){
        return;
    }

    // Scaling

    depth_mat.convertTo( scale_mat, CV_8U, 1.0/10, 0. ); // 0-10000 -> 255(white)-0(black)  alpha=-255/10000 beta->255
    resize(scale_mat  , scale_mat, color_mat.size());

    // Show Depth Image
    cv::imshow( "Depth", scale_mat );
}

// Show Depth Color
inline void RealSense::showDepthColor()
{
    if( depth_color_mat.empty() ){
        return;
    }
    resize(depth_color_mat  , depth_color_mat, color_mat.size());
    // Show Depth Image
    cv::imshow( "Depth Color", depth_color_mat );
}



 
// Modification of the function dist_3d from librealsens
void dist_3d_mod(const rs2::depth_frame& frame, float*upixel, float* upoint)
{
  
    auto udist = frame.get_distance(upixel[0], upixel[1]);

    // Deproject from pixel to point in 3D
    rs2_intrinsics intr = frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
    rs2_deproject_pixel_to_point(upoint, &intr, upixel, udist);
           
}


// This will slow down the all program.
// It is not supposed to be used on the all frame.
// only for verification
inline void RealSense::showXYZ()
{
    if( depth_mat.empty() ){
        return;
    }



    rs2_intrinsics intr = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data

    float upixel[2]; // From pixel
    float upoint[3]; // From point (in 3D)


    int i=0;
    int j=0;
 
 

    for (i=0;i<depth_mat.rows ;i++) {
	for(j=0;j<depth_mat.cols ;j++) {
 
		upixel[0]=j;
		upixel[1]=i;
		dist_3d_mod(depth_frame,upixel,upoint);

  		xyz_mat.at<cv::Vec3f>(i,j)[0]=upoint[0];
    		xyz_mat.at<cv::Vec3f>(i,j)[1]=upoint[1];
   		xyz_mat.at<cv::Vec3f>(i,j)[2]=upoint[2];

	}
    }

    // Resize to the RGB size and show XYZ Image 
    //(distance in each direction with respect to the camera center
    resize(xyz_mat  , xyz_mat, color_mat.size());
    cv::imshow( "Depth Color xyz", xyz_mat );
	
    xyz_mat.release();
}
 



