#pragma once

#include "InteractorInterface.h"
#include "VolumeInteractorComponent.generated.h"

class USphereComponent;
class UBoxComponent;

UENUM()
enum class EInteractableSelectionMethod : uint8
{
	CameraCenter,
	ActorDistance,
	FirstDetected,
	LastDetected,
};

UENUM()
enum class EInteractableDetectionVolume : uint8
{
	Cube,
	Sphere,
};

UCLASS(BlueprintType, ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API UVolumeInteractorComponent : public USceneComponent, public IInteractorInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Debug;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="DetectionVolumeType == EInteractableDetectionVolume::Cube", EditConditionHides))
	FVector VolumeSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="DetectionVolumeType == EInteractableDetectionVolume::Sphere", EditConditionHides))
	float VolumeRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnInteractableUpdatedSignature OnInteractableUpdated;
	
private:
	UPROPERTY()
	TArray<UInteractableComponent*> InteractablesInRange;
	UPROPERTY()
	UInteractableComponent* ChosenInteractableInRange;
	UPROPERTY()
	UInteractableComponent* CurrentInteractable;
	UPROPERTY()
	UShapeComponent* TriggerVolume;

	UPROPERTY(EditAnywhere)
	EInteractableSelectionMethod InteractableSelectionMethod = EInteractableSelectionMethod::CameraCenter;
	UPROPERTY(EditAnywhere)
	EInteractableDetectionVolume DetectionVolumeType = EInteractableDetectionVolume::Sphere;


	////////////
	// Methods
public:
	UVolumeInteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister();
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	/* IInteractableInterface */
	UFUNCTION(BlueprintCallable)
	virtual bool TryBeginInteraction() override;
	UFUNCTION(BlueprintCallable)
	virtual bool TryEndInteraction() override;
	UFUNCTION(BlueprintCallable)
	virtual UInteractableComponent* GetCurrentInteractable() override { return CurrentInteractable; }
	UFUNCTION(BlueprintCallable)
	virtual void AttachInteractable(UInteractableComponent* Interactable) override;
	UFUNCTION(BlueprintCallable)
	virtual void DetachInteractable() override { CurrentInteractable = nullptr; }
	UFUNCTION(BlueprintCallable)
	virtual void SendDataToInteractable(const FInteractionData& Data) override;
	UFUNCTION(BlueprintCallable)
	virtual void BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback) override;
	/* End IInteractableInterface */

private:
	UBoxComponent* InitBox(AActor* Owner);
	USphereComponent* InitSphere(AActor* Owner);
	void UpdateInteractableInRange(UInteractableComponent* InteractableComponent);
	bool IsInteracting() const { return CurrentInteractable != nullptr; }
};
