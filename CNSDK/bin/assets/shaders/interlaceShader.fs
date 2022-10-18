#if UV_TOP_LEFT == 1
    #define TEXCOORD_ORIGIN_TOP_LEFT
#else
    #define TEXCOORD_ORIGIN_BOTTOM_LEFT
#endif

#ifdef USE_VIDEO_TEXTURE_FOR_VIEWS
uniform samplerExternalOES viewTexture;
#else
uniform sampler2D viewTexture[12];
#endif

precision highp float;

out vec4 FragColor;

in vec2 TexCoord;
in vec2 ScreenTexCoord;

uniform float hardwareViewsX;
uniform float hardwareViewsY;
uniform float viewResX;
uniform float viewResY;

uniform float viewportX;
uniform float viewportY;
uniform float viewportWidth;
uniform float viewportHeight;

uniform float minView;
uniform float maxView;
uniform float n;
uniform float d_over_n;

uniform float p_over_du;
uniform float p_over_dv;
uniform float colorSlant;
uniform float colorInversion;

uniform float faceX;
uniform float faceY;
uniform float faceZ;
uniform float pixelPitch;

uniform float du;
uniform float dv;
uniform float s;
uniform float cos_theta;

uniform float sin_theta;
uniform float peelOffset;
uniform float No;
uniform float displayResX;

uniform float displayResY;
uniform float blendViews;
uniform int   interlaceMode;
uniform int   debugMode;

uniform int   perPixelCorrection;
uniform int   viewTextureType;
uniform float reconvergenceAmount;
uniform float softwareViews;

uniform float absReconvergenceAmount;
uniform mat4 textureTransform;
uniform float customTextureScaleX;
uniform float customTextureScaleY;
uniform float reconvergenceZoomX;
uniform float reconvergenceZoomY;



#define DEBUG_WHITE_VIEW 0

vec4 sampleTexture(int viewIndex, vec2 uv)
{
#ifdef USE_VIDEO_TEXTURE_FOR_VIEWS

    return texture(viewTexture, uv);
#else

    return texture(viewTexture[viewIndex], uv);
#endif
}

vec4 debugSampleAllTextures(vec2 uv, int rows, int cols)
{
    int col = int(uv.x * float(cols));
    int row = int(uv.y * float(rows));
    float u = (uv.x - float(col / cols)) * float(cols);
    float v = (uv.y - float(row / rows)) * float(rows);
    int viewIndex = row*cols+col;
    return sampleTexture(viewIndex, vec2(u,v));
}

vec3 periodic_mod(vec3 a, vec3 b) 
{
    return a - b * floor(a / b);
}

float N(float x, float y, float z, float x0, float y0)
{
    float dx     = s*x0 + (cos_theta-1.0)*x0 - sin_theta*y0;
    float dy     = s*y0 + (cos_theta-1.0)*y0 + sin_theta*x0;
    float denom  = sqrt(z*z + (1.0 - 1.0/(n*n))*((x-x0)*(x-x0) + (y-y0)*(y-y0)));
    float u      = dx + d_over_n*(x-x0)/denom;
    float v      = dy + d_over_n*(y-y0)/denom;
    float result = u/du + v/dv;
    return No + result;
}

vec4 sampleDebugStereoTexture(vec2 uv, int viewIndex)
{
    float u = ((float(viewIndex) < (hardwareViewsX/2.0f)) ? 0.0f : 0.5f) + (uv.x * 0.5f);
    float v = uv.y;
    return sampleTexture(0, vec2(u,v));
}

vec4 sampleDebugThumbnailTexture(vec2 uv, int viewIndex)
{
    //leaving for reference
    //uv += vec2(tilex[viewIndex], tiley[viewIndex]);
    return sampleTexture(0, uv);
}

vec4 sampleDebugCalibrationTexture(vec2 uv, int viewIndex)
{
         if (viewIndex == 0)  return sampleTexture(0, vec2(0.0,   0.75) + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 1)  return sampleTexture(0, vec2(0.333, 0.75) + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 2)  return sampleTexture(0, vec2(0.666, 0.75) + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 3)  return sampleTexture(0, vec2(0.0,   0.5)  + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 4)  return sampleTexture(0, vec2(0.333, 0.5)  + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 5)  return sampleTexture(0, vec2(0.666, 0.5)  + vec2(0.333, 0.25) * uv.xy);  
    else if (viewIndex == 6)  return sampleTexture(0, vec2(0.0,   0.25) + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 7)  return sampleTexture(0, vec2(0.333, 0.25) + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 8)  return sampleTexture(0, vec2(0.666, 0.25) + vec2(0.333, 0.25) * uv.xy);  
    else if (viewIndex == 9)  return sampleTexture(0, vec2(0.0,   0.0)  + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 10) return sampleTexture(0, vec2(0.333, 0.0)  + vec2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 11) return sampleTexture(0, vec2(0.666, 0.0)  + vec2(0.333, 0.25) * uv.xy);    
    return vec4(0,0,0,1); 
}

vec4 sample_view(vec2 uv, int viewIndex) 
{
//if (viewIndex < minView || viewIndex > maxView)
//  return vec4(0.0,0.0,0.0,1.0);

#if defined(TEXCOORD_ORIGIN_BOTTOM_LEFT)
    // This is the default
#elif defined(TEXCOORD_ORIGIN_TOP_LEFT)
    uv.y = 1.0 - uv.y;
#endif

#if DEBUG_WHITE_VIEW //debug first view white, rest black
    if(viewIndex == 0)
    {
        return vec4(1.0,1.0,1.0,1.0);
    }
    else
    {
        return vec4(0.0,0.0,0.0,1.0);
    }
#endif 

    float actualSoftwareView = floor(float(viewIndex) / hardwareViewsX * softwareViews);

#if ENABLE_RECONVERGENCE == 1

    // Idea: Input reconvergenceAmount value expressed in fraction of image width
    //       - translate left image by 0.5*reconv | right image by -0.5*reconv (noZoom)
    //       - OPTIONAL: apply x-scaling to avoid de-occlusions on sides (zoomX)
    //       - OPTIONAL: apply additional y-scaling to conserve image aspect ratio (zoomXY)
    
    float isLeft = actualSoftwareView; // Careful we want more L than R when numView is odd
    
    uv.x = (1.0-reconvergenceZoomX*absReconvergenceAmount)*(uv.x-0.5) + 0.5 + 0.5*(2.0*isLeft-1.0)*reconvergenceAmount; // zoom in x only
    
    uv.y = (1.0-reconvergenceZoomY*absReconvergenceAmount)*(uv.y-0.5) + 0.5; // uncomment to add zoom in y direction to conserve image aspect ratio
#endif

#ifdef SHOW_CALIBRATION_IMAGE

	// Show calibration image.
    return sampleDebugCalibrationTexture(uv, viewIndex);

#elif defined(SHOW_STEREO_IMAGE)

	// Show stereo image.
    return sampleDebugStereoTexture(uv, viewIndex);

#elif defined(SHOW_THUMBNAILS)

	// Show thumbnail
    return sampleDebugThumbnailTexture(uv, viewIndex);

#elif defined(STEREO_INDIVIDUAL_VIEWS)

    // A pair of individual left/right views.
    return sampleTexture(float(viewIndex) < (hardwareViewsX/2.0f) ? 0 : 1, uv);

#elif defined(STEREO_ATLAS_VIEWS)

    // Texture atlas of side-by-side left/right views.
    float u = ((float(viewIndex) < (hardwareViewsX/2.0f)) ? 0.0f : 0.5f) + (uv.x * 0.5f);
    float v = uv.y;
    return sampleTexture(0, vec2(u,v));

#elif defined(INDIVIDUAL_VIEWS)

    return sampleTexture(viewIndex, uv);

#elif defined(ATLAS_VIEWS)
    float xTextureScale = textureTransform[0][0];
    float yTextureScale = textureTransform[1][1];

    #if SINGLE_VIEW_MODE == 1
        actualSoftwareView = 0.0;
    #endif
    
    //Quick support for nx1 & 1xn view sources
    #if HORIZONTAL_SOURCE_VIEWS == 1
        float viewScale = (1.0 / softwareViews) * customTextureScaleX;
        uv.x = (uv.x * viewScale) + (actualSoftwareView * viewScale);
        uv.y *= customTextureScaleY;

    #else
        float viewScale = (1.0 / softwareViews) * customTextureScaleY;
        uv.y = (uv.y * viewScale) + (actualSoftwareView * viewScale);
        uv.x *= customTextureScaleX;

    #endif

    return sampleTexture(0, uv);

#endif

    return vec4(0,0,0,0);
}

vec4 sample_view(vec2 uv, float view)
{
    //if (blendViews == 0.0)
        //return sample_view(uv, int(view));

    float black_point    = 0.0;
    float white_point    = 1.0;
    float view1          = mod(floor(view), hardwareViewsX);
    float view2          = mod(floor(view1 + 1.1), hardwareViewsX);
    float linear_mix     = (view - view1);
    float non_linear_mix = smoothstep(black_point, white_point, linear_mix);
    vec4 a               = sample_view(uv, int(view1));
    vec4 b               = sample_view(uv, int(view2));

    return mix(a, b, non_linear_mix * blendViews);
}

vec3 calculateViewVector(vec2 uv)
{
    vec3 views;

    float baseView = (p_over_du * displayResX * uv.x) + (p_over_dv * displayResY * uv.y);
    views.r = baseView;
    views.g = baseView;
    views.b = baseView;

    views.r += 2.0 * colorInversion;
    views.g += 1.0;
    views.b += 2.0 * (1.0 - colorInversion);

    return views;
}

vec3 calculateViewIndices(vec2 normalized_display_coord)
{
   vec3 viewVector = calculateViewVector(normalized_display_coord);

    float viewCount = hardwareViewsX * hardwareViewsY;
	// correct some float precision issues with this offset
    float correction = - 1.0 / max(2.0, viewCount);

    float float_precision_offset = 0.0;
    
    float user_offset = 0.0;

#if DEBUG_WHITE_VIEW
     // we want todo no PPC!
#else

    #if PIXELCORRECTION == 1 // View peeling
            float x0 = (normalized_display_coord.x - 0.5) * displayResX * pixelPitch;
         //there is a strong likelyhood this is all we need and the #define was being used multiple times incorrectly, lee & cyrus to verify
            float y0 = (normalized_display_coord.y - 0.5) * displayResY * pixelPitch;

                float dN = N(faceX, faceY, faceZ, 0.0, 0.0) - N(faceX, faceY, faceZ, x0, y0);
                user_offset += (viewCount - 1.0) * 0.5 - No;
                user_offset -= peelOffset;
                user_offset += dN;
    #elif PIXELCORRECTION == 2 // stereo sliding
         float x0 = (normalized_display_coord.x - 0.5) * displayResX * pixelPitch;
        
         //there is a strong likelyhood this is all we need and the #define was being used multiple times incorrectly, lee & cyrus to verify
         float y0 = (normalized_display_coord.y - 0.5) * displayResY * pixelPitch;

         user_offset += (viewCount - mod(viewCount + 1.0, 2.0)) * 0.5 - N(faceX, faceY, faceZ, x0, y0);
    #endif

#endif

    user_offset -= p_over_dv*(displayResY-1.0);

    // last row / row "3" of viewMatrix is ith view index,
    vec3 views = periodic_mod(viewVector + user_offset + float_precision_offset, vec3(viewCount));

   return views;
}

void main()
{
    vec2 texture_offset = vec2(0.5) / vec2(displayResX, displayResY);
    vec2  uv = ScreenTexCoord;
    float u0 = float(viewportX)/float(displayResX);
    float v0 = float(viewportY)/float(displayResY);
    float u1 = float(viewportX+viewportWidth)/float(displayResX);
    float v1 = float(viewportY+viewportHeight)/float(displayResY);
    uv = mix(vec2(u0, v0), vec2(u1, v1), uv);
    vec2 normalized_display_coord = uv - texture_offset;

    vec3 viewIndices = calculateViewIndices(normalized_display_coord);

    vec4 interlaced_fragment;

    normalized_display_coord = TexCoord - texture_offset;

#if SINGLE_VIEW_MODE == 1
    interlaced_fragment = sample_view(normalized_display_coord, 0);
#else

    interlaced_fragment[0] = sample_view(normalized_display_coord, viewIndices[0]).r;
    interlaced_fragment[1] = sample_view(normalized_display_coord, viewIndices[1]).g;
    interlaced_fragment[2] = sample_view(normalized_display_coord, viewIndices[2]).b;
    interlaced_fragment[3] = 1.0;
#endif

    FragColor  = interlaced_fragment; 

#if defined(SHOW_TEXTURE_COORDINATES)
    FragColor = vec4(ScreenTexCoord.x, TexCoord.y, 0.0f, 1.0f); // Show texture coordinates
#elif defined(SHOW_ALL_VIEWS)
    FragColor = debugSampleAllTextures(TexCoord, 3, 4);   // Show all textures
#elif defined(SHOW_FIRST_VIEW)
    FragColor = sampleTexture(0, TexCoord);        // Show first texture
#endif
}