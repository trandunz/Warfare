#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GunInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGunInterface : public UInterface
{
	GENERATED_BODY()
};

class WARFARE_API IGunInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Fire() = 0;
	virtual void Reload() = 0;
};
