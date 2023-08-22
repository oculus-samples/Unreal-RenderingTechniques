#include "PortalCaptureActor.h"

#include "EngineDefines.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "UnrealEngine.h"
#include "StereoRendering.h"
#include "IXRTrackingSystem.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraTypes.h"

// Sets default values
APortalCaptureActor::APortalCaptureActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APortalCaptureActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APortalCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APortalCaptureActor::IsRenderingStereo() const
{
	return GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetStereoRenderingDevice() && GEngine->XRSystem->GetStereoRenderingDevice()->IsStereoEnabled();
}

FMatrix APortalCaptureActor::GetLeftEyeProjection() const
{
	TSharedPtr<IStereoRendering, ESPMode::ThreadSafe> Stereo = GEngine->XRSystem->GetStereoRenderingDevice();
	FMatrix Result = Stereo->GetStereoProjectionMatrix(EStereoscopicEye::eSSE_LEFT_EYE);
	return Result;
}

FMatrix APortalCaptureActor::GetRightEyeProjection() const
{
	TSharedPtr<IStereoRendering, ESPMode::ThreadSafe> Stereo = GEngine->XRSystem->GetStereoRenderingDevice();
	FMatrix Result = Stereo->GetStereoProjectionMatrix(EStereoscopicEye::eSSE_RIGHT_EYE);
	return Result;
}

void APortalCaptureActor::AdjustLeftEyePose(FRotator InRotation, FVector InPosition, FRotator& OutRotation, FVector& OutPosition) const
{
	GEngine->StereoRenderingDevice->CalculateStereoViewOffset(EStereoscopicEye::eSSE_LEFT_EYE, InRotation, GetWorld()->GetWorldSettings()->WorldToMeters, InPosition);
	OutRotation = InRotation;
	OutPosition = InPosition;
}

void APortalCaptureActor::AdjustRightEyePose(FRotator InRotation, FVector InPosition, FRotator& OutRotation, FVector& OutPosition) const
{
	GEngine->StereoRenderingDevice->CalculateStereoViewOffset(EStereoscopicEye::eSSE_RIGHT_EYE, InRotation, GetWorld()->GetWorldSettings()->WorldToMeters, InPosition);
	OutRotation = InRotation;
	OutPosition = InPosition;
}

FMinimalViewInfo APortalCaptureActor::AdjustLeftEyeViewInfo(FMinimalViewInfo ViewInfo) const
{
	GEngine->StereoRenderingDevice->CalculateStereoViewOffset(EStereoscopicEye::eSSE_LEFT_EYE, ViewInfo.Rotation, GetWorld()->GetWorldSettings()->WorldToMeters, ViewInfo.Location);
	return ViewInfo;
}

FMinimalViewInfo APortalCaptureActor::AdjustRightEyeViewInfo(FMinimalViewInfo ViewInfo) const
{
	GEngine->StereoRenderingDevice->CalculateStereoViewOffset(EStereoscopicEye::eSSE_RIGHT_EYE, ViewInfo.Rotation, GetWorld()->GetWorldSettings()->WorldToMeters, ViewInfo.Location);
	return ViewInfo;
}

void APortalCaptureActor::ClipPortalScreenBounds(FVector MinLocalPoint, FVector MaxLocalPoint, FMatrix LocalToProjection, FVector2D& MinPointNDCOut, FVector2D& MaxPointNDCOut, bool& IsInView) const
{
	checkf(FMath::Abs(MinLocalPoint.Z - MaxLocalPoint.Z) < 1e-5f, TEXT("The portal should be a plane with normal (0, 0, 1)"));
	FVector LocalPoints[4] = {
		FVector(MinLocalPoint.X, MinLocalPoint.Y, MinLocalPoint.Z),
		FVector(MinLocalPoint.X, MaxLocalPoint.Y, MinLocalPoint.Z),
		FVector(MaxLocalPoint.X, MinLocalPoint.Y, MinLocalPoint.Z),
		FVector(MaxLocalPoint.X, MaxLocalPoint.Y, MinLocalPoint.Z)
	};

	IsInView = true;

	FVector4 HomogenousClipPoints[4];
	for (uint32 i = 0; i < 4; ++i)
	{
		HomogenousClipPoints[i] = LocalToProjection.TransformPosition(LocalPoints[i]);
	}

	FVector MinPointNDC = FVector(FLT_MAX);
	FVector MaxPointNDC = FVector(-FLT_MAX);

	uint32 BehindNearPlaneCount = 0;
	for (uint32 i = 0; i < 4; ++i)
	{
		FVector NDCPoint = FVector(HomogenousClipPoints[i].X, HomogenousClipPoints[i].Y, HomogenousClipPoints[i].Z) / HomogenousClipPoints[i].W;

		if (NDCPoint.Z < 0)
		{
			BehindNearPlaneCount += 1;
			continue;
		}

		if (NDCPoint.X < MinPointNDC.X)
			MinPointNDC.X = NDCPoint.X;
		if (NDCPoint.Y < MinPointNDC.Y)
			MinPointNDC.Y = NDCPoint.Y;
		if (NDCPoint.Z < MinPointNDC.Z)
			MinPointNDC.Z = NDCPoint.Z;

		if (NDCPoint.X > MaxPointNDC.X)
			MaxPointNDC.X = NDCPoint.X;
		if (NDCPoint.Y > MaxPointNDC.Y)
			MaxPointNDC.Y = NDCPoint.Y;
		if (NDCPoint.Z > MaxPointNDC.Z)
			MaxPointNDC.Z = NDCPoint.Z;
	}

	if (BehindNearPlaneCount == 4)
	{
		IsInView = false;
		return;
	}

	// Clamp to NDC space.
	if (MinPointNDC.X < -1)
		MinPointNDC.X = -1;
	if (MinPointNDC.Y < -1)
		MinPointNDC.Y = -1;
	if (MaxPointNDC.X > 1)
		MaxPointNDC.X = 1;
	if (MaxPointNDC.Y > 1)
		MaxPointNDC.Y = 1;

	if (BehindNearPlaneCount > 0)
	{
		// We're intersecting the near plane, so don't scale the projection matrix to avoid having to clip against the clip frustum.
		// We're probably close enough to the portal that the resolution difference won't be significant.
		// As an optimisation, you could compute the clipped portal plane against the clip frustum to generate tighter bounds.
		MinPointNDC = FVector(-1, -1, 0);
		MaxPointNDC = FVector(1, 1, 1);
	}
	else if (MinPointNDC.X >= 1 || MinPointNDC.Y >= 1 || MinPointNDC.Z >= 1 || MaxPointNDC.X <= -1 || MaxPointNDC.Y <= -1 || MaxPointNDC.Z <= -1)
	{
		IsInView = false;
	}
	MinPointNDCOut = FVector2D(MinPointNDC.X, MinPointNDC.Y);
	MaxPointNDCOut = FVector2D(MaxPointNDC.X, MaxPointNDC.Y);
}
