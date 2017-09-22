#ifndef EVENT_H
#define EVENT_H

#include <memory>

#include "object_includes.h"
#include "defines_object.h"

namespace chatd
{

DECLARATION_OBJECT(event,
    DECLARATION_FIELD(time_tt, time)
    DECLARATION_FIELD(OID, channel_oid)
    DECLARATION_FIELD(OID, recipient_oid)
    DECLARATION_FIELD(std::shared_ptr<const swarm::object>, payload)
)

} // end of namespace

#endif
