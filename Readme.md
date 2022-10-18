# Leia CNSDK Getting Started Documentation

A repository containing a minimal Windows OpenGL application to showcase the use of Leia CNSDK.

## How to build

 * Open CNSDKGettingStarted.sln in Visual Studio 2019.
 * Open Build and run Debug or Release configuration.

## CNSDK usage

For the best experience on Leia displays, use the "Stereo Sliding" interlace mode.

Typical application:

  Create window

  Initialize OpenGL

  Initialize CNSDK
    Call CreateLeiaSDK() to create main SDK class
    Call ILeiaSDK::InitializePlatform() for platform-specific initialization
    Call ILeiaSDK::Initialize() for general initialization
    Call ILeiaSDK::CreateNewThreadedInterlacer to create interlacer class
    Call ThreadedInterlacer::InitializeOpenGL() to initialize the interlacer for OpenGL use
    Optionally call ThreadedInterlacer::InitializeGUI() if you wish to use the Leia interactive GUI
    Call ThreadedInterlacer::InitOnDesiredThread()

  Initialize offscreen frame buffer
    Create a double-wide frame buffer to render into.
    Your application will render the left view to the left half and the right view to the right half.

  Load scene to render
     Perform app-specific preparation to render (loading textures, buffers, necessary game data, etc.)

  Call ILeiaSDK::SetBacklight(true) to enable backlight

  Main render loop

     For left and right views
        Call ThreadedInterlacer::GetViewOffset(view) to get the offset for the current view

        For a correct projection, we perform both an offset and shear. 
            The shear is calculates as follows:
              shearX = -viewOffset.x / sdk->GetConvergenceDistance()
              shearY = -viewOffset.z / sdk->GetConvergenceDistance()
            The shear is applied to the projection matrix as follows:
              projectionMatrix[2][0] = projectionMatrix[0][0] * shearX
              projectionMatrix[2][1] = projectionMatrix[1][1] * shearY
        
        Use glViewport() or any other mechanism to render into the left or right half of the offscreen frame buffer

     Perform interlacing
        You should now have an offscreen frame buffer containing the left and right views     
        Call ThreadedInterlacer::SetInterlaceViewTextureAtlas() to set the offscreen frame buffer you just rendered to
        Call ThreadedInterlacer::SetSourceViewsSize() to let the interlacer know the size and layout of the offscreen frame buffer
        Call ThreadedInterlacer::DoPostProcess() with the render target (or frame buffer) and it's size to perform the interlacing
        You should now have an interlaced image in the render target

     Present the backfuffer
        Call glFlush();
        Call SwapBuffers(hDC);

  Call ILeiaSDK::SetBacklight(false) to disable backlight

  Call ILeiaSDK::Destroy() to cleanup the Leia SDK