#include "event.h"
#include "empty.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(event)

DEFINITION_CONSTS(event, "event", BIT_NULL, 1)

DEFINITION_FIELD(event, time_tt, time)
DEFINITION_FIELD(event, OID, channel_oid)
DEFINITION_FIELD(event, OID, recipient_oid)
DEFINITION_FIELD(event, std::shared_ptr<const swarm::object>, payload)

event::event() : FIELD(time)(time(NULL)), FIELD(channel_oid)(0), FIELD(recipient_oid)(0)
{
    FIELD(payload) = std::make_shared<swarm::empty>();
}

DEFINITION_SPECIAL_MEMBERS(event)

} // end of namespace
