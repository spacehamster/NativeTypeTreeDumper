#pragma once
#include "Util.h"
struct AllocationRootWithSalt {
	uint32_t m_Salt;
	uint32_t m_RootReferenceIndex;
};

struct AllocationRootReference;

enum MemLabelIdentifier {
	// Not worth filling out
};

struct MemLabelId
{
#if VERSION_GE(5, 0)
	AllocationRootWithSalt m_RootReferenceWithSalt;
	MemLabelIdentifier label;
#else
	MemLabelIdentifier label;
	AllocationRootReference* rootReference;
#endif
};