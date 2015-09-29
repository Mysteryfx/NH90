#ifndef _DXMeshHdr_h_
#define _DXMeshHdr_h_

#include "vector.h"

namespace Graphics
{
typedef struct DXMESH_HEADER
{
DWORD dwFVF; //,  sizeof(dwFVF));   dataByte += sizeof(dwFVF);
DWORD dwVertexSize; // memcpy(dataByte, &vertexSize, sizeof(vertexSize)); dataByte += sizeof(vertexSize);
DWORD dwIndexSize; // memcpy(dataByte, &indexSize, sizeof(indexSize));  dataByte += sizeof(indexSize);
DWORD dwNumVerts; // memcpy(dataByte, &numVerts,  sizeof(numVerts));  dataByte += sizeof(numVerts);
DWORD dwNumInds; // memcpy(dataByte, &numIndexes, sizeof(numIndexes)); dataByte += sizeof(numIndexes);
DWORD dwPrimType; //memcpy(dataByte, &primType,  sizeof(primType));  dataByte += sizeof(primType);
DWORD dwVertxOffset; //memcpy(dataByte, &vertexOffset, sizeof(vertexOffset)); dataByte += sizeof(vertexOffset);
DWORD dwIndexOffset; //memcpy(dataByte, &indexOffset, sizeof(indexOffset)); dataByte += sizeof(indexOffset);
Vector3 MinDims;
Vector3 MaxDims;
float bound_radius;
Vector3 bound_sphere_center;
DWORD dwReserved[16];

} DXMESH_HEADER;
}
#endif