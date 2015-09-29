// === Copyright 2014, Eagle Dynamics, All Rights Reserved ===
/*
 * ED Sounder API
 */
#pragma once
#include "./api_types.h"

namespace ed {

// type-safe opaque handles
typedef struct _Sounder_s* SounderID;
typedef struct _SounderParam_s* SounderParamID;
typedef struct _SounderEvent_s* SounderEventID;

/**
 * Working with sounders:
 *
 * 1. create(customInstanceName, sounderResouceName)
 * 2. get handles to params and events via bindParam()/bindEvent()
 * 3. during the lifetime of your object:
 *    a) update continuous parameters via setParam() or setVectorParam()
 *    b) to send events do setEventParams() [optional] followed by sendEvent() call(s).
 * 4. call destroy() when the object is no longer needed
 */
class ISounderAPI {
public:
    /**
     * Create sounder.
     * If sounder resource is invalid or not found, returns nullptr.
     * On success, returns opaque sounder handle.
     * To start processing, a startSounder() call is required.
     */
    virtual SounderID create(const char* sounderName, const char* sounderResource/*, size_t numArgs, const double* args*/) = 0;

    /**
     * Destroy sounder.
     * All future references to the same SounderID will fail.
     */
    virtual bool destroy(SounderID) = 0;

    /**
     * Find sounder by name.
     * If more than one sounder shares the same name, it returns the id of the last created one.
     */
    virtual SounderID getByName(const char* sounderName) = 0;

    /**
     * Get a permanent handle to the sounder's named parameter.
     * Handles are global.
     */
    virtual SounderParamID bindParam(const char* paramName) = 0;

    /**
     * Set the value of a sounder's continuous scalar parameter.
     * The sounder's onUpdate(params) will receive the most recent value of the parameter.
     */
    virtual bool setParam(SounderID snd, SounderParamID param, double val) = 0;

    /**
     * Get a permanent handle to a sounder's event type.
     * Handles are global.
     */
    virtual SounderEventID bindEvent(const char* eventName) = 0;

    /**
     * Send event to the sounder.
     * Sounder will receive the event via onEvent_eventName(...).
     * Each call generates a separate event to the sounder.
     */
    virtual bool sendEvent(SounderID snd, SounderEventID, size_t numArgs, const double* args) = 0;

}; // class ISounderAPI

} // namespace ed

/// DO NOT USE!
ED_EXTERN ed::ISounderAPI* ED_CALLTYPE ED_SounderAPI_v1();

// USE THIS to access the interface:
inline ed::ISounderAPI* ED_SounderAPI() { return ED_SounderAPI_v1(); }
