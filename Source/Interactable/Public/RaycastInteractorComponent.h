// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "RaycastInteractorComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteractableUpdatedSignature, UInteractableComponent*, InteractableComponent);

UCLASS(ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API URaycastInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Debug;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsInteracting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnInteractableUpdatedSignature OnInteractableUpdated;

private:
	UPROPERTY()
	UInteractableComponent* CurrentInteractable;

public:
	URaycastInteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetRaycastCamera(UCameraComponent* InCamera);
	bool TryBeginInteraction();
	bool TryEndInteraction();

protected:
	virtual void BeginPlay() override;

private:
	void DoInteractionTest();
	void UpdateCachedInteractable(UInteractableComponent* InteractableComponent);
	void CleanupCachedInteractable();
};
