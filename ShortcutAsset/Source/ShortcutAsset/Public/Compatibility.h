#pragma once

#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_OLDER_THAN(5, 0, 0)
#define OBJECT_PTR(Class) Class*
#else
#define OBJECT_PTR(Class) TObjectPtr<Class>
#endif

