#include <Helpers/Macro.h>

#include <UnitClass.h>
#include <BuildingClass.h>
#include "Body.h"


DEFINE_HOOK(739956, UnitClass_Deploy_Transfer, 6)
{
	GET(UnitClass*, pUnit, EBP);
	GET(BuildingClass*, pStructure, EBX);

	// Vehicle-to-building deployer targeting
	auto pTypeData = TechnoTypeExt::ExtMap.Find(pStructure->GetTechnoType());
	if (pTypeData->Deployed_RememberTarget)
	{ // && pUnit->Target > 0)
		pStructure->Target = pUnit->Target;
	}

	return 0;
}

DEFINE_HOOK(6F9E50, TechnoClass_Update, 5)
{
	GET(TechnoClass*, pThis, ECX);

	if (pThis->WhatAmI() == AbstractType::Building) {
		auto pTypeData = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		// Prevent target loss when vehicles are deployed into buildings.
		if (pTypeData->Deployed_RememberTarget)
		{
			auto currentMission = pThis->CurrentMission;
			// With this the vehicle will not forget who is the target until the deploy process finish
			if (pThis->Target > 0 &&
				currentMission != Mission::Construction &&
				currentMission != Mission::Guard &&
				currentMission != Mission::Attack &&
				currentMission != Mission::Selling
				) {
				pThis->QueueMission(Mission::Construction, 0);
				pThis->LastTarget = pThis->Target;
			}
			else if (pThis->Target == 0 && currentMission == Mission::Construction) {
				// Just when the deployment into structure ended the vehicle forgot the target. Just attack the original target.
				pThis->Target = pThis->LastTarget;
				pThis->QueueMission(Mission::Attack, 0);
			}
		}
	}

	return 0;
}

