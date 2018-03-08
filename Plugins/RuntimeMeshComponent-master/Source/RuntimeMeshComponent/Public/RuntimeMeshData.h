// Copyright 2016-2018 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshCore.h"
#include "RuntimeMeshCollision.h"
#include "RuntimeMeshSection.h"
#include "RuntimeMeshBlueprint.h"

class URuntimeMesh;
class FRuntimeMeshProxy;
class FRuntimeMeshBuilder;

using FRuntimeMeshProxyPtr = TSharedPtr<FRuntimeMeshProxy, ESPMode::ThreadSafe>;

DECLARE_DELEGATE_OneParam(FRuntimeMeshGameThreadTaskDelegate, URuntimeMesh*);



DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section - No Data"), STAT_RuntimeMesh_CreateMeshSection_NoData, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Dual Buffer Mesh Section - No Data"), STAT_RuntimeMesh_CreateMeshSectionDualBuffer_NoData, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Triple Buffer Mesh Section - No Data"), STAT_RuntimeMesh_CreateMeshSectionTripleBuffer_NoData, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section"), STAT_RuntimeMesh_CreateMeshSection, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section - With Bounding Box"), STAT_RuntimeMesh_CreateMeshSection_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section Dual Buffer"), STAT_RuntimeMesh_CreateMeshSectionDualBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section Dual Buffer - With Bounding Box"), STAT_RuntimeMesh_CreateMeshSectionDualBuffer_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section Triple Buffer"), STAT_RuntimeMesh_CreateMeshSectionTripleBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Create Mesh Section Triple Buffer - With Bounding Box"), STAT_RuntimeMesh_CreateMeshSectionTripleBuffer_BoundingBox, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section - No Triangles"), STAT_RuntimeMesh_UpdateMeshSection_NoTriangles, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section - No Triangles - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSection_NoTriangles_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section"), STAT_RuntimeMesh_UpdateMeshSection, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSection_BoundingBox, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Dual Buffer - No Triangles"), STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_NoTriangles, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Dual Buffer - No Triangles - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_NoTriangles_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Dual Buffer"), STAT_RuntimeMesh_UpdateMeshSectionDualBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Dual Buffer - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_BoundingBox, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Triple Buffer - No Triangles"), STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_NoTriangles, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Triple Buffer - No Triangles - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_NoTriangles_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Triple Buffer"), STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Triple Buffer - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_BoundingBox, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Primary Buffer"), STAT_RuntimeMesh_UpdateMeshSectionPrimaryBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Primary Buffer - With Bounding Box"), STAT_RuntimeMesh_UpdateMeshSectionPrimaryBuffer_BoundingBox, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Secondary Buffer"), STAT_RuntimeMesh_UpdateMeshSectionSecondaryBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Tertiary Buffer"), STAT_RuntimeMesh_UpdateMeshSectionTertiaryBuffer, STATGROUP_RuntimeMesh);
DECLARE_CYCLE_STAT(TEXT("RM - Update Mesh Section Triangles"), STAT_RuntimeMesh_UpdateMeshSectionTriangles, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Set Section Tessellation Triangles"), STAT_RuntimeMesh_SetSectionTessellationTriangles, STATGROUP_RuntimeMesh);

DECLARE_CYCLE_STAT(TEXT("RM - Serialize Data"), STAT_RuntimeMesh_SerializationOperator, STATGROUP_RuntimeMesh);


/**
 *
 */
class RUNTIMEMESHCOMPONENT_API FRuntimeMeshData
{
	/** Array of sections of mesh */
	TArray<FRuntimeMeshSectionPtr> MeshSections;

	/* Array of collision only mesh sections*/
	TMap<int32, FRuntimeMeshCollisionSection> MeshCollisionSections;

	/** Convex shapes used for simple collision */
	TMap<int32, FRuntimeMeshCollisionConvexMesh> ConvexCollisionSections;

	TArray<FRuntimeMeshCollisionBox> CollisionBoxes;
	TArray<FRuntimeMeshCollisionSphere> CollisionSpheres;
	TArray<FRuntimeMeshCollisionCapsule> CollisionCapsules;

	/** Local space bounds of mesh */
	FBoxSphereBounds LocalBounds;

	/** Parent mesh object that owns this data. */
	TWeakObjectPtr<URuntimeMesh> ParentMeshObject;

	/** Render proxy for this mesh */
	FRuntimeMeshProxyPtr RenderProxy;

	TUniquePtr<FRuntimeMeshLockProvider> SyncRoot;
	TFunction<FRuntimeMeshLockProvider*()> LockFactory;




public:
	FRuntimeMeshData();
	~FRuntimeMeshData();

private:
	void CheckCreate(const FRuntimeMeshVertexStreamStructure Stream0Structure,
		const FRuntimeMeshVertexStreamStructure& Stream1Structure, const FRuntimeMeshVertexStreamStructure& Stream2Structure, bool bIndexIsValid) const;

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void CheckCreate() const
	{
#if DO_CHECK
		CheckCreate(GetStreamStructure<VertexType0>(), GetStreamStructure<VertexType1>(), GetStreamStructure<VertexType2>(), FRuntimeMeshIndexTraits<IndexType>::IsValidIndexType);
#endif
	}

	void CheckUpdate(const FRuntimeMeshVertexStreamStructure& Stream0Structure, const FRuntimeMeshVertexStreamStructure& Stream1Structure,
		const FRuntimeMeshVertexStreamStructure& Stream2Structure, bool b32BitIndices,
		int32 SectionIndex, bool bShouldCheckIndexType, bool bCheckVertexStream0 = true, bool bCheckVertexStream1 = true, bool bCheckVertexStream2 = true) const;

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void CheckUpdate(int32 SectionIndex, bool bShouldCheckIndexType, bool bCheckVertexStream0 = true, bool bCheckVertexStream1 = true, bool bCheckVertexStream2 = true) const
	{
#if DO_CHECK
		CheckUpdate(GetStreamStructure<VertexType0>(), GetStreamStructure<VertexType1>(), GetStreamStructure<VertexType2>(), FRuntimeMeshIndexTraits<IndexType>::Is32Bit, SectionIndex,
			!bShouldCheckIndexType || FRuntimeMeshIndexTraits<IndexType>::IsValidIndexType, bCheckVertexStream0, bCheckVertexStream1, bCheckVertexStream2);
#endif
	}

	void CheckBoundingBox(const FBox& Box) const;

public:

	
	template<typename VertexType0, typename IndexType>
	void CreateMeshSection(int32 SectionIndex, bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSection_NoData);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, ESectionUpdateFlags::None);
	}

	template<typename VertexType0, typename VertexType1, typename IndexType>
	void CreateMeshSectionDualBuffer(int32 SectionIndex, bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionDualBuffer_NoData);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, ESectionUpdateFlags::None);
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void CreateMeshSectionTripleBuffer(int32 SectionIndex, bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionTripleBuffer_NoData);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, VertexType2, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, VertexType2, IndexType>(SectionIndex, UpdateFrequency);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, ESectionUpdateFlags::None);
	}

	

	template<typename VertexType0, typename IndexType>
	void CreateMeshSection(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<IndexType>& InTriangles, bool bCreateCollision = false,
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSection);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}

	template<typename VertexType0, typename IndexType>
	void CreateMeshSection(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<IndexType>& InTriangles, const FBox& BoundingBox,
		bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSection_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}

	template<typename VertexType0, typename VertexType1, typename IndexType>
	void CreateMeshSectionDualBuffer(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<IndexType>& InTriangles, bool bCreateCollision = false,
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionDualBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0);
		Section->UpdateVertexBuffer1(InVertices1);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}

	template<typename VertexType0, typename VertexType1, typename IndexType>
	void CreateMeshSectionDualBuffer(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<IndexType>& InTriangles, const FBox& BoundingBox,
		bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionDualBuffer_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
		Section->UpdateVertexBuffer1(InVertices1);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void CreateMeshSectionTripleBuffer(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2, TArray<IndexType>& InTriangles,
		bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionTripleBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, VertexType2, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, VertexType2, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0);
		Section->UpdateVertexBuffer1(InVertices1);
		Section->UpdateVertexBuffer2(InVertices2);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void CreateMeshSectionTripleBuffer(int32 SectionIndex, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2, TArray<IndexType>& InTriangles,
		const FBox& BoundingBox, bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_CreateMeshSectionTripleBuffer_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckCreate<VertexType0, VertexType1, VertexType2, IndexType>();

		// Create the section
		FRuntimeMeshSectionPtr Section = CreateOrResetSection<VertexType0, VertexType1, VertexType2, IndexType>(SectionIndex, UpdateFrequency);

		// Set the vertex and index buffers
		Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
		Section->UpdateVertexBuffer1(InVertices1);
		Section->UpdateVertexBuffer2(InVertices2);
		Section->UpdateIndexBuffer(InTriangles);

		// Track collision status and update collision information if necessary
		Section->SetCollisionEnabled(bCreateCollision);

		// Finalize section.
		CreateSectionInternal(SectionIndex, UpdateFlags);
	}



	template<typename VertexType0>
	void UpdateMeshSection(int32 SectionId, TArray<VertexType0>& InVertices0, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSection_NoTriangles);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, uint16>(SectionId, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0>
	void UpdateMeshSection(int32 SectionId, TArray<VertexType0>& InVertices0, const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSection_NoTriangles_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, uint16>(SectionId, false);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename IndexType>
	void UpdateMeshSection(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<IndexType>& InTriangles, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSection);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionId, true);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename IndexType>
	void UpdateMeshSection(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<IndexType>& InTriangles,
		const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSection_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionId, true);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1>
	void UpdateMeshSectionDualBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1,
		ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_NoTriangles);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, FRuntimeMeshNullVertex, uint16>(SectionId, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1>
	void UpdateMeshSectionDualBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1,
		const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_NoTriangles_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, FRuntimeMeshNullVertex, uint16>(SectionId, false);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename IndexType>
	void UpdateMeshSectionDualBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1,
		TArray<IndexType>& InTriangles, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionDualBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>(SectionId, true);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename IndexType>
	void UpdateMeshSectionDualBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<IndexType>& InTriangles,
		const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionDualBuffer_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, FRuntimeMeshNullVertex, IndexType>(SectionId, true);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2>
	void UpdateMeshSectionTripleBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2,
		ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_NoTriangles);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, VertexType2, uint16>(SectionId, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update vertices if supplied
		if (InVertices2.Num() > 0)
		{
			Section->UpdateVertexBuffer2(InVertices2);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer2;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2>
	void UpdateMeshSectionTripleBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2,
		const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_NoTriangles_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, VertexType2, uint16>(SectionId, false);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update vertices if supplied
		if (InVertices2.Num() > 0)
		{
			Section->UpdateVertexBuffer2(InVertices2);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer2;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void UpdateMeshSectionTripleBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2,
		TArray<IndexType>& InTriangles, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, VertexType2, IndexType>(SectionId, true);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update vertices if supplied
		if (InVertices2.Num() > 0)
		{
			Section->UpdateVertexBuffer2(InVertices2);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer2;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	void UpdateMeshSectionTripleBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, TArray<VertexType1>& InVertices1, TArray<VertexType2>& InVertices2,
		TArray<IndexType>& InTriangles, const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTripleBuffer_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, VertexType1, VertexType2, IndexType>(SectionId, true);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Update vertices if supplied
		if (InVertices2.Num() > 0)
		{
			Section->UpdateVertexBuffer2(InVertices2);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer2;
		}

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}
	


	template<typename VertexType0>
	void UpdateMeshSectionPrimaryBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionPrimaryBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, uint16>(SectionId, false, true, false, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType0>
	void UpdateMeshSectionPrimaryBuffer(int32 SectionId, TArray<VertexType0>& InVertices0, const FBox& BoundingBox, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionPrimaryBuffer_BoundingBox);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<VertexType0, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, uint16>(SectionId, false, true, false, false);
		CheckBoundingBox(BoundingBox);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices0.Num() > 0)
		{
			Section->UpdateVertexBuffer0(InVertices0, &BoundingBox);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer0;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType1>
	void UpdateMeshSectionSecondaryBuffer(int32 SectionId, TArray<VertexType1>& InVertices1, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionSecondaryBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<FRuntimeMeshNullVertex, VertexType1, FRuntimeMeshNullVertex, uint16>(SectionId, false, false, true, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices1.Num() > 0)
		{
			Section->UpdateVertexBuffer1(InVertices1);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer1;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename VertexType2>
	void UpdateMeshSectionTertiaryBuffer(int32 SectionId, TArray<VertexType2>& InVertices2, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTertiaryBuffer);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, VertexType2, uint16>(SectionId, false, false, false, true);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];
		
		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update vertices if supplied
		if (InVertices2.Num() > 0)
		{
			Section->UpdateVertexBuffer2(InVertices2);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::VertexBuffer2;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}

	template<typename IndexType>
	void UpdateMeshSectionTriangles(int32 SectionId, TArray<IndexType>& InTriangles, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_UpdateMeshSectionTriangles);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionId, true, false, false, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update triangles if supplied
		if (InTriangles.Num() > 0)
		{
			Section->UpdateIndexBuffer(InTriangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::IndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, UpdateFlags);
		}
	}



	void CreateMeshSection(int32 SectionId, const TSharedPtr<FRuntimeMeshBuilder>& MeshData, bool bCreateCollision = false,
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	void CreateMeshSectionByMove(int32 SectionId, const TSharedPtr<FRuntimeMeshBuilder>& MeshData, bool bCreateCollision = false,
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);



	void UpdateMeshSection(int32 SectionId, const TSharedPtr<FRuntimeMeshBuilder>& MeshData, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	void UpdateMeshSectionByMove(int32 SectionId, const TSharedPtr<FRuntimeMeshBuilder>& MeshData, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);


private:
	void CreateMeshSectionFromComponents(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, TFunction<FColor(int32 Index)> ColorAccessor, int32 NumColors, const TArray<FRuntimeMeshTangent>& Tangents,
		bool bCreateCollision, EUpdateFrequency UpdateFrequency, ESectionUpdateFlags UpdateFlags, bool bUseHighPrecisionTangents, bool bUseHighPrecisionUVs, bool bWantsSecondUV);

	void UpdateMeshSectionFromComponents(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, TFunction<FColor(int32 Index)> ColorAccessor, int32 NumColors, const TArray<FRuntimeMeshTangent>& Tangents, ESectionUpdateFlags UpdateFlags);

public:

	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents, bool bCreateCollision = false,
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None,
		bool bUseHighPrecisionTangents = false, bool bUseHighPrecisionUVs = true);

	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents,
		bool bCreateCollision = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None,
		bool bUseHighPrecisionTangents = false, bool bUseHighPrecisionUVs = true);


	void UpdateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0,
		const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	void UpdateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0,
		const TArray<FVector2D>& UV1, const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	void UpdateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	void UpdateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, const TArray<FColor>& Colors, const TArray<FRuntimeMeshTangent>& Tangents, ESectionUpdateFlags UpdateFlags = ESectionUpdateFlags::None);

	

	void CreateMeshSection_Blueprint(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FRuntimeMeshTangent>& Tangents, const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, const TArray<FLinearColor>& Colors,
		bool bCreateCollision = false, bool bCalculateNormalTangent = false, bool bShouldCreateHardTangents = false, bool bGenerateTessellationTriangles = false, 
		EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average, bool bUseHighPrecisionTangents = false, bool bUseHighPrecisionUVs = true);
	
	void UpdateMeshSection_Blueprint(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
		const TArray<FRuntimeMeshTangent>& Tangents, const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, const TArray<FLinearColor>& Colors,
		bool bCalculateNormalTangent = false, bool bShouldCreateHardTangents = false, bool bGenerateTessellationTriangles = false);
	
	void CreateMeshSectionPacked_Blueprint(int32 SectionIndex, const TArray<FRuntimeMeshBlueprintVertexSimple>& Vertices, const TArray<int32>& Triangles,
		bool bCreateCollision = false, bool bCalculateNormalTangent = false, bool bShouldCreateHardTangents = false, bool bGenerateTessellationTriangles = false, EUpdateFrequency UpdateFrequency = EUpdateFrequency::Average,
		bool bUseHighPrecisionTangents = false, bool bUseHighPrecisionUVs = true);
		
	void UpdateMeshSectionPacked_Blueprint(int32 SectionIndex, const TArray<FRuntimeMeshBlueprintVertexSimple>& Vertices, const TArray<int32>& Triangles,
		bool bCalculateNormalTangent = false, bool bShouldCreateHardTangents = false, bool bGenerateTessellationTriangles = false);

	

	TSharedPtr<const FRuntimeMeshAccessor> GetReadonlyMeshAccessor(int32 SectionId);
	

	template<typename IndexType>
	void SetSectionTessellationTriangles(int32 SectionId, const TArray<IndexType>& Triangles)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_SetSectionTessellationTriangles);

		FRuntimeMeshScopeLock Lock(SyncRoot);

		CheckUpdate<FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, FRuntimeMeshNullVertex, IndexType>(SectionId, true, false, false, false);

		FRuntimeMeshSectionPtr Section = MeshSections[SectionId];

		ERuntimeMeshBuffersToUpdate BuffersToUpdate = ERuntimeMeshBuffersToUpdate::None;

		// Update indices if supplied
		if (Triangles.Num() > 0)
		{
			Section->UpdateAdjacencyIndexBuffer(Triangles);
			BuffersToUpdate |= ERuntimeMeshBuffersToUpdate::AdjacencyIndexBuffer;
		}

		// Finalize section update if we have anything to apply
		if (BuffersToUpdate != ERuntimeMeshBuffersToUpdate::None)
		{
			UpdateSectionInternal(SectionId, BuffersToUpdate, ESectionUpdateFlags::None);
		}
	}


	/** Clear a section of the procedural mesh. */
	void ClearMeshSection(int32 SectionIndex);

	/** Clear all mesh sections and reset to empty state */
	void ClearAllMeshSections();



	/** Gets the bounding box of a specific section */
	FBox GetSectionBoundingBox(int32 SectionIndex);

	/** Control visibility of a particular section */
	void SetMeshSectionVisible(int32 SectionIndex, bool bNewVisibility);

	/** Returns whether a particular section is currently visible */
	bool IsMeshSectionVisible(int32 SectionIndex) const;


	/** Control whether a particular section casts a shadow */
	void SetMeshSectionCastsShadow(int32 SectionIndex, bool bNewCastsShadow);

	/** Returns whether a particular section is currently casting shadows */
	bool IsMeshSectionCastingShadows(int32 SectionIndex) const;


	/** Control whether a particular section has collision */
	void SetMeshSectionCollisionEnabled(int32 SectionIndex, bool bNewCollisionEnabled);

	/** Returns whether a particular section has collision */
	bool IsMeshSectionCollisionEnabled(int32 SectionIndex);


	/** Returns number of sections currently created for this component */
	int32 GetNumSections() const;

	/** Returns whether a particular section currently exists */
	bool DoesSectionExist(int32 SectionIndex) const;

	/** Returns first available section index */
	int32 GetAvailableSectionIndex() const;

	int32 GetLastSectionIndex() const;


	TArray<int32> GetSectionIds() const;


	void SetMeshCollisionSection(int32 CollisionSectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles);

	void ClearMeshCollisionSection(int32 CollisionSectionIndex);

	void ClearAllMeshCollisionSections();



	int32 AddConvexCollisionSection(TArray<FVector> ConvexVerts);

	void SetConvexCollisionSection(int32 ConvexSectionIndex, TArray<FVector> ConvexVerts);

	void RemoveConvexCollisionSection(int32 ConvexSectionIndex);

	void ClearConvexCollisionSections();

	void SetCollisionConvexMeshes(const TArray<TArray<FVector>>& ConvexMeshes);


	int32 AddCollisionBox(const FRuntimeMeshCollisionBox& NewBox);

	void RemoveCollisionBox(int32 Index);

	void ClearCollisionBoxes();

	void SetCollisionBoxes(const TArray<FRuntimeMeshCollisionBox>& NewBoxes);

	
	int32 AddCollisionSphere(const FRuntimeMeshCollisionSphere& NewSphere);

	void RemoveCollisionSphere(int32 Index);

	void ClearCollisionSpheres();

	void SetCollisionSpheres(const TArray<FRuntimeMeshCollisionSphere>& NewSpheres);

	
	int32 AddCollisionCapsule(const FRuntimeMeshCollisionCapsule& NewCapsule);

	void RemoveCollisionCapsule(int32 Index);

	void ClearCollisionCapsules();

	void SetCollisionCapsules(const TArray<FRuntimeMeshCollisionCapsule>& NewCapsules);


	FBoxSphereBounds GetLocalBounds() const;

private:
	
	FRuntimeMeshSectionPtr GetSection(int32 SectionIndex) { return MeshSections[SectionIndex]; }

	void Setup(TWeakObjectPtr<URuntimeMesh> InParentMeshObject);

	FRuntimeMeshProxyPtr GetRenderProxy() const { return RenderProxy; }


	/* Creates an mesh section of a specified type at the specified index. */
	template<typename VertexType0, typename VertexType1, typename VertexType2, typename IndexType>
	FRuntimeMeshSectionPtr CreateOrResetSection(int32 SectionId, EUpdateFrequency UpdateFrequency)
	{
		static_assert(FRuntimeMeshIndexTraits<IndexType>::IsValidIndexType, "Indices can only be of type uint16, uint32, or int32");

		return CreateOrResetSection(SectionId, GetStreamStructure<VertexType0>(), GetStreamStructure<VertexType1>(),
			GetStreamStructure<VertexType2>(), FRuntimeMeshIndexTraits<IndexType>::Is32Bit, UpdateFrequency);
	}

	FRuntimeMeshSectionPtr CreateOrResetSection(int32 SectionId,
		const FRuntimeMeshVertexStreamStructure& Stream0, const FRuntimeMeshVertexStreamStructure& Stream1,
		const FRuntimeMeshVertexStreamStructure& Stream2, bool b32BitIndices, EUpdateFrequency UpdateFrequency);

	FRuntimeMeshSectionPtr CreateOrResetSectionForBlueprint(int32 SectionId, bool bWantsSecondUV,
		bool bHighPrecisionTangents, bool bHighPrecisionUVs, EUpdateFrequency UpdateFrequency);

	/* Finishes creating a section, including entering it for batch updating, or updating the RT directly */
	void CreateSectionInternal(int32 SectionIndex, ESectionUpdateFlags UpdateFlags);

	/* Finishes updating a section, including entering it for batch updating, or updating the RT directly */
	void UpdateSectionInternal(int32 SectionIndex, ERuntimeMeshBuffersToUpdate BuffersToUpdate, ESectionUpdateFlags UpdateFlags);

	/* Handles things like automatic tessellation and tangent calculation that is common to both section creation and update. */
	void HandleCommonSectionUpdateFlags(int32 SectionIndex, ESectionUpdateFlags UpdateFlags, ERuntimeMeshBuffersToUpdate& BuffersToUpdate);

	/* Finishes updating a sections properties, like visible/casts shadow, a*/
	void UpdateSectionPropertiesInternal(int32 SectionIndex, bool bUpdateRequiresProxyRecreateIfStatic);

	/** Update LocalBounds member from the local box of each section */
	void UpdateLocalBounds();

	FRuntimeMeshProxyPtr EnsureProxyCreated();

	void Initialize();

	bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const;
	bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData);
	void CopyCollisionElementsToBodySetup(UBodySetup* Setup);
	void MarkCollisionDirty(bool bSkipChangedFlag = false);

	void MarkRenderStateDirty();
	void SendSectionPropertiesUpdate(int32 SectionIndex);

	int32 GetSectionFromCollisionFaceIndex(int32 FaceIndex) const;


	void DoOnGameThread(FRuntimeMeshGameThreadTaskDelegate Func);

	void MarkChanged();


	friend FArchive& operator <<(FArchive& Ar, FRuntimeMeshData& MeshData)
	{
		SCOPE_CYCLE_COUNTER(STAT_RuntimeMesh_SerializationOperator);

		//FRuntimeMeshScopeLock Lock(MeshData.SyncRoot);

		Ar << MeshData.MeshSections;

		Ar << MeshData.MeshCollisionSections;
		Ar << MeshData.ConvexCollisionSections;

		Ar << MeshData.CollisionBoxes;
		Ar << MeshData.CollisionSpheres;
		Ar << MeshData.CollisionCapsules;

		// Update all state since we don't know what really changed (or this could be an initial load)
		if (Ar.IsLoading())
		{
			MeshData.UpdateLocalBounds();
		}
		return Ar;
	}

	friend class URuntimeMesh;
};

using FRuntimeMeshDataRef = TSharedRef<FRuntimeMeshData, ESPMode::ThreadSafe>;
using FRuntimeMeshDataPtr = TSharedPtr<FRuntimeMeshData, ESPMode::ThreadSafe>;