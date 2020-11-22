#include "PrFirebaseLibrary.h"

#include "PrFirebase.h"

UPrFirebaseProxy* UPrFirebaseLibrary::GetFirebaseProxy()
{
	return IPrFirebase::Get().FirebaseProxy;
}
