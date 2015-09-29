#ifndef CLIP_VOLUMES_INTERSECTION_TYPE
#define CLIP_VOLUMES_INTERSECTION_TYPE

enum IntersectionType 
{
	itInside,     // shape is inside question object
	itIntersects, // shape intersects object
	itContains,   // shape contains object
	itOutside     // shape don't have common points with object
};

#endif // INTERSECTION_TYPE
