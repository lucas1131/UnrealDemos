// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractorInterface.h"
#include "InteractableComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "RaycastInteractorComponent.generated.h"

UCLASS(ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API URaycastInteractorComponent : public UActorComponent, public IInteractorInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Debug;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionRaycastDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnInteractableUpdatedSignature OnInteractableUpdated;

private:
	UPROPERTY()
	UInteractableComponent* InteractableInRange;
	UPROPERTY()
	UInteractableComponent* CurrentInteractable;

public:
	URaycastInteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void SetRaycastCamera(UCameraComponent* InCamera);
	UFUNCTION(BlueprintCallable)
	UInteractableComponent* GetCurrentInteractableComponent() const { return CurrentInteractable; }

	/* IInteractableInterface */
	UFUNCTION(BlueprintCallable)
	virtual bool TryBeginInteraction() override;
	UFUNCTION(BlueprintCallable)
	virtual bool TryEndInteraction() override;
	UFUNCTION(BlueprintCallable)
	virtual UInteractableComponent* GetCurrentInteractable() override { return CurrentInteractable; }
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentInteractable(UInteractableComponent* Interactable) override;
	UFUNCTION(BlueprintCallable)
	virtual void ClearCurrentInteractable() override { CurrentInteractable = nullptr; }
	UFUNCTION(BlueprintCallable)
	virtual void SendDataToInteractable(const FInteractionData& Data) override;
	UFUNCTION(BlueprintCallable)
	virtual void BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback) override;
	UFUNCTION(BlueprintCallable)
	virtual float GetInteractionDistance() override { return InteractionDistance; }
	/* End IInteractableInterface */

protected:
	virtual void BeginPlay() override;

private:
	void DoInteractionTest();
	void UpdateInteractableInRange(UInteractableComponent* InteractableComponent);
	bool IsInteracting() const { return CurrentInteractable != nullptr; }
};
