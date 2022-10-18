#pragma once

#include "leia/sdk/api.h"
#include "leia/common/log.h"
#include "leia/sdk/enums.hpp"
#include "leia/sdk/helpers.hpp"
#include "leia/sdk/shaders.hpp"
#include "leia/sdk/debugMenu.hpp"

// TODO: remove glm from the public API
#include <glm/glm.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <array>

#if defined(LEIA_USE_OPENGL)
#include "leia/common/opengl/utils.hpp"
#endif

#include <spdlog/stopwatch.h>

#if defined(LEIA_USE_NEW_RENDERER)
namespace LeiaRenderer
{
    class IRenderer;
    class IShader;
    class ITexture;
    class IRenderState;
}
#endif

#if defined(LEIA_USE_DIRECTX)
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
#endif


struct leia_sdk_image_desc;

namespace leia::sdk {

class ILeiaSDK;
class LeiaSDK;

struct ThreadedInterlacerInitArgs {
    bool isProtected = false;
	bool useMegaTextureForViews = false;
    InterlacerDelegate* delegate = nullptr;
};

class ThreadedInterlacer
{
	//Interlacers must be made through the SDK
	ThreadedInterlacer(const LeiaSDK& mReadOnlyLeiaSDK, ThreadedInterlacerInitArgs const&);
	~ThreadedInterlacer();

	friend class LeiaSDK;
private:

#ifdef LEIA_ENABLE_THUMBNAIL_EXAMPLE
	std::vector<Thumbnail>  mThumbnails;
#endif

#ifdef LEIA_USE_NEW_RENDERER
    LeiaRenderer::IRenderer*    mRenderer                  = nullptr;
    LeiaRenderer::IShader*      mInterlaceShader           = nullptr;
    LeiaRenderer::IShader*      mSharpenShader             = nullptr;
    LeiaRenderer::ITexture*     mViewTexture               = nullptr;
    LeiaRenderer::ITexture*     mInterlacedResultTexture   = nullptr;
    LeiaRenderer::IRenderState* mInterlaceRenderState      = nullptr;
    LeiaRenderer::IRenderState* mSharpenRenderState        = nullptr;
    LeiaRenderer::ITexture*     mOutputRenderTarget        = nullptr;
    LeiaRenderer::ITexture*     mUserInterlaceTextureAtlas = nullptr;
    LeiaRenderer::ITexture*     mActiveViewTexture         = nullptr;
#else
	opengl::Shader mInterlaceShader;
	opengl::Shader mSharpenShader;
	uint32_t mViewRenderTargets[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	uint32_t mViewTextures[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	uint32_t mViewDepthBuffers[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	uint32_t mInterlacedResultRenderTarget = 0;
	uint32_t mInterlacedResultTexture = 0;
	uint32_t mInterlacedResultDepthBuffer = 0;
	uint32_t mActiveViewTextureId = 0;
	uint32_t mDebugThumbnailTextureIds[32] = { 0 };
	uint32_t mFullscreenQuadVertexArray = 0;
	uint32_t mFullscreenQuadVertexBuffer = 0;
	uint32_t mFullscreenQuadIndexBuffer = 0;
	uint32_t mOutputRenderTarget = 0;
	uint32_t mUserInterlaceTextureAtlas = 0;
#endif
	eLeiaShaderDebugMode mShaderDebugMode = eLeiaShaderDebugMode::None;
	eLeiaTaskResponsibility mBuffersResponsibility = eLeiaTaskResponsibility::SDK;
	eLeiaTaskResponsibility mShadersResponsibility = eLeiaTaskResponsibility::SDK;
	eLeiaTaskResponsibility mRenderingResponsibility = eLeiaTaskResponsibility::SDK;
	eLeiaInterlaceMode mInterlaceMode = eLeiaInterlaceMode::StereoSliding;

	int mInterlacedResultWidth = 0;
	int mInterlacedResultHeight = 0;
	bool mRenderTargetsOK = true;
	int mViewWidth = 0;
	int mViewHeight = 0;
	int mViewCount = 0;
	int mSourceViewsWidth = 0;
	int mSourceViewsHeight = 0;
	eFitMode fitMode = eFitMode::Fill;
    bool flipInputUVVertical = false;
	bool mIsProtectedContext = false;
    InterlacerDelegate* delegate = nullptr;
	bool mSourceViewsHorizontal = true;
    float mAspectRatioOffset = 0.0f;
	bool mSingleViewMode = false;
	bool mUseMegaTextureForViews = false;
    bool mReconvergenceEnabled = false;
    float mReconvergenceAmount = 0.0f;
    bool mReconvergenceZoomX = false;
    bool mReconvergenceZoomXY = true;
	float mAlpha = 1.0f;
    int mViewportXPosition = 0;
    int mViewportYPosition = 0;
	bool mMediumShaderPrecision = false;
    spdlog::stopwatch mNoFaceTime;

	glm::vec3 mLastPrimaryFacePosition;
	glm::vec3 mLastNonPredictedPrimaryFacePosition;
    glm::mat4 customTextureTransform = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 customRectTransform = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
public:

	LEIASDK_API
	void InitOnDesiredThread();
	LEIASDK_API
	void Shutdown();
	LEIASDK_API
	int			GetNumViews() const;
	LEIASDK_API
	glm::vec3	GetViewOffset(int view) const;
	LEIASDK_API
	bool IsOnSameThread(std::thread::id const& callingThread) const;
	LEIASDK_API
	void SetFlipInputUVVertical(bool);
	LEIASDK_API
	void SetShaderDebugMode(eLeiaShaderDebugMode mode);
	LEIASDK_API
	void SetInterlaceMode(eLeiaInterlaceMode _interlaceMode);
	LEIASDK_API
	void SetSourceViewsSize(int sourceWidth, int sourceHeight, bool isHorizontalViews);
    LEIASDK_API
	void SetSingleViewMode(bool setSingleView) { mSingleViewMode = setSingleView;}
    LEIASDK_API
    void SetAlpha(float newAlpha) { mAlpha = newAlpha; }
    LEIASDK_API
    void SetViewportPosition(int newXPos, int newYPos) { mViewportXPosition = newXPos; mViewportYPosition = newYPos; }
	LEIASDK_API
	void ApplyInterlacing(int width, int height, bool blended);
	LEIASDK_API
	void ApplySharpening(int width, int height);
	LEIASDK_API
	bool UpdateBuffers(int viewCount, eBufferType bufferType, int width, int height);

#if defined(LEIA_USE_OPENGL)
    LEIASDK_API
    void InitializeOpenGL(HGLRC context, eLeiaTaskResponsibility buffersResponsibility, eLeiaTaskResponsibility shadersResponsibility, eLeiaTaskResponsibility renderingResponsibility);
    LEIASDK_API
    uint32_t GetRenderTargetForView(int view);
    LEIASDK_API
    void SetViewTextureId(int newId);
    LEIASDK_API
    void SetInterlaceViewTextureAtlas(uint32_t texture);
    LEIASDK_API
    void SetOutputRenderTarget(uint32_t renderTarget);
    LEIASDK_API
    void DoPostProcess(int width, int height, bool blendedInterlace = false, uint32_t outputRenderTarget = 0);
    LEIASDK_API
    void DoPostProcessPicture(int width, int height, uint32_t views);
    LEIASDK_API
    void DoPostProcessVideo(int width, int height, uint32_t views);
    LEIASDK_API
    uint32_t CreateGLTexture(leia_sdk_image_desc const&);
#elif defined(LEIA_USE_DIRECTX)
    // todo: implement above functions with DX parameters
    LEIASDK_API
    void InitializeD3D11(ID3D11DeviceContext* context, eLeiaTaskResponsibility buffersResponsibility, eLeiaTaskResponsibility shadersResponsibility, eLeiaTaskResponsibility renderingResponsibility);
    LEIASDK_API
    void DoPostProcess(int width, int height, bool blendedInterlace, ID3D11RenderTargetView* resultTexture);
    LEIASDK_API
    void DoPostProcessPicture(int width, int height, ID3D11ShaderResourceView* pictureShaderResourceView, ID3D11RenderTargetView* renderTargetView);
    LEIASDK_API
    void SetViewTextureId(ID3D11ShaderResourceView* view);
    LEIASDK_API
    void SetInterlaceViewTextureAtlas(ID3D11ShaderResourceView* texture);
    LEIASDK_API
    ID3D11RenderTargetView* GetRenderTargetView(int view);
    LEIASDK_API
    ID3D11DepthStencilView* GetDepthStencilView(int view);
#endif

	LEIASDK_API
	void OnWindowSizeChanged(int width, int height);
	LEIASDK_API
	void GetInterlaceParameters(LeiaInterlaceParameters& interlaceParameters) const;
	LEIASDK_API
	std::string GetInterlaceShaderText(eLeiaShaderType shaderType) const;
	LEIASDK_API
	std::string GetSharpenShaderText(eLeiaShaderType shaderType) const;

	LEIASDK_API
	void ToggleInterlaceMode(ILeiaSDK const& mReadOnlyLeiaSDK);
	LEIASDK_API
	void ToggleShaderDebugMode();

	LEIASDK_API
	void SetFitMode(eFitMode mode);
	LEIASDK_API
	eFitMode GetFitMode() const;
    LEIASDK_API
    void SetCustomTextureMatrix(const glm::mat4& textureTransform);
	LEIASDK_API
    void EnableReconvergence(bool enable);
	LEIASDK_API
    bool IsReconvergenceEnabled() const;
	LEIASDK_API
    void SetReconvergence(float value);
    LEIASDK_API
    void SetReconvergenceZoom(bool zoomX, bool zoomXY);
    LEIASDK_API
    float GetReconvergence() const;

    LEIASDK_API
    void InitializeGui(DebugMenuInitArgs const&);
    LEIASDK_API
    void ShutdownGui();
    LEIASDK_API
    bool ToggleGui();
    LEIASDK_API
    bool IsGuiVisible();
#if defined(LEIA_OS_WINDOWS)
    LEIASDK_API
    Gui::InputState ProcessGuiInput(GuiSurface surface, uint32_t msg, uint64_t wparam, int64_t lparam);
#elif defined(LEIA_OS_ANDROID)
    LEIASDK_API
    Gui::InputState ProcessGuiInput(AInputEvent const*);
    LEIASDK_API
    Gui::InputState ProcessGuiMotionInput(JNIEnv*, jobject motionInput);
#endif
    LEIASDK_API
    void RenderGui(int width, int height);
    LEIASDK_API
    void SetAspectRatioOffset(float offset);
    LEIASDK_API
    float GetAspectRatioOffset() const;

private:
	enum DebugTexture {
		kDebugTextureCalibration = 0,
		kDebugTextureBeerGlass,
		kDebugTexturesCount
	};

#ifdef LEIA_USE_NEW_RENDERER
    std::array<LeiaRenderer::ITexture*, kDebugTexturesCount> mDebugTexturesCache{ 0 };
    LeiaRenderer::ITexture* GetDebugTexture(DebugTexture);
#else
	std::array<uint32_t, kDebugTexturesCount> mDebugTexturesCache{0};
	uint32_t GetDebugTexture(DebugTexture);
#endif
    void InitializeGraphics();
	bool NoFaceThresholdReached() const;
	bool NeedsSharpenPass() const;
    glm::vec3 GetPrimaryFace() const;
	glm::vec3 GetNonPredictedPrimaryFace() const;

private:
    friend class leia::sdk::DebugMenu;
#if defined(LEIA_USE_OPENGL)
	OVR::GLEContext* mGleContext = NULL;
#endif
	void PrintInterlacerDebugStatus() const;

	const LeiaSDK& mReadOnlyLeiaSDK;

	std::thread::id mInitializedThreadID;
    std::mutex mMutex;

    std::unique_ptr<DebugMenu> mDebugMenu;

	bool mIsRendering = false;
	bool mIsYieldingRender = false;
	bool mRequestYieldAtRender = false;

	spdlog::stopwatch mFrameTime;
	spdlog::stopwatch mLifeTime;

	//On Creation this class will wait until interlacers are yielding their rendering
	//On destruction it will tell all interlacers to continue rendering
    class YieldRenderingAllInterlacers
    {
        LeiaSDK& leiaSdk;

    public:
        YieldRenderingAllInterlacers(LeiaSDK* leiaSdk);
        ~YieldRenderingAllInterlacers();

    };
};

} // namespace leia::sdk
