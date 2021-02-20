#include <Helpers/Macro.h>
#include <GeneralStructures.h>
#include <YRMath.h>

DEFINE_HOOK(6FE4F6, TechnoClass_FireAt_ArcingFix, 0)
{
	LEA_STACK(CoordStruct*, coordA, 0x44);
	LEA_STACK(CoordStruct*, coordB, 0x88);

	double distanceSquared =
		(double)(coordA->X - coordB->X) * (double)(coordA->X - coordB->X) +
		(double)(coordA->Y - coordB->Y) * (double)(coordA->Y - coordB->Y) +
		(double)(coordA->Z - coordB->Z) * (double)(coordA->Z - coordB->Z);
	
	R->EAX((int)Math::sqrt(distanceSquared));
	
	return 0x6FE537;
}