#if UV_TOP_LEFT == 1
    #define TEXCOORD_ORIGIN_TOP_LEFT
#else
    #define TEXCOORD_ORIGIN_BOTTOM_LEFT
#endif

#define DEBUG_WHITE_VIEW 0

struct VSInput
{
    float3 Pos : POSITION;
    float3 Col : COLOR;
    float2 Tex : TEXCOORD;
};

struct PSInput
{
    float4               Pos            : SV_POSITION;
    noperspective float2 TexCoord       : TEXCOORD;
    noperspective float2 ScreenTexCoord : TEXCOORD2;
};

#if defined(INDIVIDUAL_VIEWS) || defined(STEREO_INDIVIDUAL_VIEWS)
    Texture2DArray viewTextureArray : register(t0);
#else
    Texture2D viewTexture : register(t0);
#endif

SamplerState viewSampler : register(s0);

cbuffer LeiaInterlaceShaderConstantBufferData : register(b0)
{
    float  hardwareViewsX;
    float  hardwareViewsY;
    float  viewResX;
    float  viewResY;

    float  viewportX;
    float  viewportY;
    float  viewportWidth;
    float  viewportHeight;

    float  minView;
    float  maxView;
    float  n;
    float  d_over_n;

    float  p_over_du;
    float  p_over_dv;
    float  colorSlant;
    float  colorInversion;

    float  faceX;
    float  faceY;
    float  faceZ;
    float  pixelPitch;

    float  du;
    float  dv;
    float  s;
    float  cos_theta;

    float  sin_theta;
    float  peelOffset;
    float  No;
    float  displayResX;

    float  displayResY;
    float  blendViews;
    int    interlaceMode;
    int    debugMode;

    int    perPixelCorrection;
    int    viewTextureType;
    float  reconvergenceAmount;
    float  softwareViews;

    float  absReconvergenceAmount;
    float  padding0;
    float  padding1;
    float  padding2;

    float4x4 textureTransform;

    float4x4 rectTransform;

    float  customTextureScaleX;
    float  customTextureScaleY;
    float  reconvergenceZoomX;
    float  reconvergenceZoomY;
};

float4 sampleTexture(int viewIndex, float2 uv)
{
#if defined(INDIVIDUAL_VIEWS) || defined(STEREO_INDIVIDUAL_VIEWS)
    float4 color = viewTextureArray.Sample(viewSampler, float3(uv, viewIndex));
#else
    float4 color = viewTexture.Sample(viewSampler, uv);
#endif

    // Linear->Gamma
    //if (gammaCorrect != 0)
    //    color.rgb = pow(color.rgb, float3(1.0f/2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return color;
}

float4 debugSampleAllTextures(float2 uv, int rows, int cols)
{
    int col = int(uv.x * float(cols));
    int row = int(uv.y * float(rows));
    float u = (uv.x - float(col / cols)) * float(cols);
    float v = (uv.y - float(row / rows)) * float(rows);
    int viewIndex = row*cols+col;
    return sampleTexture(viewIndex, float2(u,v));
}

float3 periodic_mod(float3 a, float3 b) 
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

float4 sampleDebugStereoTexture(float2 uv, int viewIndex)
{
    float u = ((float(viewIndex) < (hardwareViewsX/2.0f)) ? 0.0f : 0.5f) + (uv.x * 0.5f);
    float v = uv.y;
    return sampleTexture(0, float2(u,v));
}

float4 sampleDebugThumbnailTexture(float2 uv, int viewIndex)
{
    //leaving for reference
    //uv += float2(tilex[viewIndex].x, tiley[viewIndex].x);
    return sampleTexture(0, uv);
}

float4 sampleDebugCalibrationTexture(float2 uv, int viewIndex)
{
         if (viewIndex == 0)  return sampleTexture(0, float2(0.0,   0.75) + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 1)  return sampleTexture(0, float2(0.333, 0.75) + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 2)  return sampleTexture(0, float2(0.666, 0.75) + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 3)  return sampleTexture(0, float2(0.0,   0.5)  + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 4)  return sampleTexture(0, float2(0.333, 0.5)  + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 5)  return sampleTexture(0, float2(0.666, 0.5)  + float2(0.333, 0.25) * uv.xy);  
    else if (viewIndex == 6)  return sampleTexture(0, float2(0.0,   0.25) + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 7)  return sampleTexture(0, float2(0.333, 0.25) + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 8)  return sampleTexture(0, float2(0.666, 0.25) + float2(0.333, 0.25) * uv.xy);  
    else if (viewIndex == 9)  return sampleTexture(0, float2(0.0,   0.0)  + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 10) return sampleTexture(0, float2(0.333, 0.0)  + float2(0.333, 0.25) * uv.xy);
    else if (viewIndex == 11) return sampleTexture(0, float2(0.666, 0.0)  + float2(0.333, 0.25) * uv.xy);    
    return float4(0,0,0,1); 
}

float4 sample_view(float2 uv, int viewIndex) 
{
//if (viewIndex < minView || viewIndex > maxView)
//  return float4(0.0,0.0,0.0,1.0);

#if DEBUG_WHITE_VIEW //debug first view white, rest black
    if(viewIndex == 0)
    {
        return float4(1.0,1.0,1.0,1.0);
    }
    else
    {
        return float4(0.0,0.0,0.0,1.0);
    }
#endif 

    float actualSoftwareView = floor(float(viewIndex) / hardwareViewsX * softwareViews);

#if ENABLE_RECONVERGENCE == 1
    // Idea: Input reconvergenceAmount value expressed in fraction of image width
    //       - translate left image by 0.5*reconv | right image by -0.5*reconv (noZoom)
    //       - OPTIONAL: apply x-scaling to avoid de-occlusions on sides (zoomX)
    //       - OPTIONAL: apply additional y-scaling to conserve image aspect ratio (zoomXY)
    
    float isLeft = actualSoftwareView; // Careful we want more L than R when numView is odd
    
    uv.x = (1-reconvergenceZoomX*absReconvergenceAmount)*(uv.x-0.5) + 0.5 + 0.5*(2*isLeft-1.0)*reconvergenceAmount; // zoom in x only
    
    uv.y = (1-reconvergenceZoomY*absReconvergenceAmount)*(uv.y-0.5) + 0.5; // uncomment to add zoom in y direction to conserve image aspect ratio
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
    return sampleTexture(0, float2(u,v));

#elif defined(INDIVIDUAL_VIEWS)

    // Individual views.
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

    return float4(0,0,0,0);
}

float4 sample_view(float2 uv, float view)
{
    //if (blendViews == 0.0)
        //return sample_view(uv, int(view));

    float black_point    = 0.0;
    float white_point    = 1.0;
    float view1          = fmod(floor(view), hardwareViewsX);
    float view2          = fmod(floor(view1 + 1.1), hardwareViewsX);
    float linear_mix     = (view - view1);
    float non_linear_mix = smoothstep(black_point, white_point, linear_mix);
    float4 a             = sample_view(uv, int(view1));
    float4 b             = sample_view(uv, int(view2));

    return lerp(a, b, non_linear_mix * blendViews);
}

float3 calculateViewVector(float2 uv)
{
    float3 views;

    float baseView = (p_over_du * displayResX * uv.x) + (p_over_dv * displayResY * uv.y) + fmod(colorSlant * displayResY * uv.y, p_over_du);
    views.r = baseView;
    views.g = baseView;
    views.b = baseView;

    views.r += 2.0 * colorInversion;
    views.g += 1.0;
    views.b += 2.0 * (1.0 - colorInversion);

    return views;
}

float3 calculateViewIndices(float2 normalized_display_coord)
{
   float3 viewVector = calculateViewVector(normalized_display_coord);

    float viewCount = hardwareViewsX * hardwareViewsY;
	// correct some float precision issues with this offset
    float correction = - 1.0 / max(2.0, viewCount);

    float float_precision_offset = 0.0;
    
    //#if DirectX set offset to 0
    float user_offset =  1.0 - p_over_dv * displayResY;

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
         user_offset += (viewCount - fmod(viewCount + 1.0, 2.0)) * 0.5 - N(faceX, faceY, faceZ, x0, y0);
    #endif

#endif

    // last row / row "3" of viewMatrix is ith view index,
    float3 views = periodic_mod(viewVector + user_offset + float_precision_offset, float3(viewCount, viewCount, viewCount));

   return views;
}

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput)0;

    // Scale and offset texture coordinates.
    output.TexCoord  = mul(textureTransform, float4(input.Tex, 0, 1)).xy;

    // Scale of offset position of rectangle being rendered.
    output.ScreenTexCoord = mul(rectTransform, float4(input.Tex, 0, 1)).xy;

    // Scale and offset position.
    output.Pos = float4((output.ScreenTexCoord  * 2.0) - 1.0, 0, 1);

#ifdef TEXCOORD_ORIGIN_TOP_LEFT
    output.TexCoord.y = 1.0f - output.TexCoord.y;
#endif

    return output;
}

float4 PSMain(PSInput input) : SV_Target0
{
    float2 texture_offset = float2(0.5, 0.5) / float2(displayResX, displayResY);
    float2  uv = input.ScreenTexCoord;
    float u0 = float(viewportX)/float(displayResX);
    float v0 = float(viewportY)/float(displayResY);
    float u1 = float(viewportX+viewportWidth)/float(displayResX);
    float v1 = float(viewportY+viewportHeight)/float(displayResY);
    uv = lerp(float2(u0, v0), float2(u1, v1), uv);
    float2 normalized_display_coord = uv - texture_offset;

    float3 viewIndices = calculateViewIndices(normalized_display_coord);

    float4 interlaced_fragment;

    normalized_display_coord = input.TexCoord - texture_offset;

    interlaced_fragment[0] = sample_view(normalized_display_coord, viewIndices[0]).r;
    interlaced_fragment[1] = sample_view(normalized_display_coord, viewIndices[1]).g;
    interlaced_fragment[2] = sample_view(normalized_display_coord, viewIndices[2]).b;
    interlaced_fragment[3] = 1.0;

    float4 FragColor  = interlaced_fragment; 

#if defined(SHOW_TEXTURE_COORDINATES)
    FragColor = float4(input.TexCoord.x, input.TexCoord.y, 0.0f, 1.0f); // Show texture coordinates
#elif defined(SHOW_ALL_VIEWS)
    FragColor = debugSampleAllTextures(input.TexCoord, 3, 4);   // Show all textures
#elif defined(SHOW_FIRST_VIEW)
    FragColor = sampleTexture(0, input.TexCoord);        // Show first texture
#endif

    return FragColor;
}