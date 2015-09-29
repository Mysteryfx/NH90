Samplers

For proper compile and link with DCS core headers and libraries you project should have the next preprocessor defines:

_CRT_SECURE_NO_DEPRECATE
_USE_MATH_DEFINES
NOMINMAX
_HAS_ITERATOR_DEBUGGING=0			// very important when you use any stl containers exported from DCS 
_SECURE_SCL=0						// very important when you use any stl containers exported from DCS 
USE_NEDMALLOC_STL_ALLOCATOR 		// very important when you use any stl containers exported from DCS 
ED_MODULE_NAME="CLIENT"  			// as you wish , it is just name for log output

NDEBUG  as SDK is compiled in release configuration 
_DEBUG  define need to be removed to correctly allocate object as in some headers menebrs are declared as debug only ( will be fixed ASAP )



other compile options 

/GR (Enable Run-Time Type Information)
/TP (Specify Source File Type as C++)

Runtime library : Multi-threaded DLL (/MD)  as because SDK libs linked with it
Debug Information Format  :	/Zi
Enable C+++ exceptions : Yes (/EHsc)



