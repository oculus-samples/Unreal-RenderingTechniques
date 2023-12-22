// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalCaptureActor.generated.h"

UCLASS()
class PORTALS_API APortalCaptureActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalCaptureActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	bool IsRenderingStereo() const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	FMatrix GetLeftEyeProjection() const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	FMatrix GetRightEyeProjection() const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	void AdjustLeftEyePose(FRotator InRotation, FVector InPosition, FRotator& OutRotation, FVector& OutPosition) const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	void AdjustRightEyePose(FRotator InRotation, FVector InPosition, FRotator& OutRotation, FVector& OutPosition) const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	FMinimalViewInfo AdjustLeftEyeViewInfo(FMinimalViewInfo InViewInfo) const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	FMinimalViewInfo AdjustRightEyeViewInfo(FMinimalViewInfo InViewInfo) const;

	UFUNCTION(BlueprintPure, Category = "StereoRendering")
	void ClipPortalScreenBounds(FVector MinLocalPoint, FVector MaxLocalPoint, FMatrix LocalToProjection, FVector2D& MinPointNDC, FVector2D& MaxPointNDC, bool& IsInView) const;
};
