#include "status.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(status)

DEFINITION_CONSTS(status, "status", BIT_NULL, 1)

DEFINITION_FIELD(status, OID, user_oid)
DEFINITION_FIELD(status, sptr_cstr, nickname)
DEFINITION_FIELD(status, time_tt, created)
DEFINITION_FIELD(status, size_tt, connections_amount)
DEFINITION_FIELD(status, std::shared_ptr<std::list<std::shared_ptr<const chatd::channel>>>, channels)

status::status() : FIELD(user_oid)(0), FIELD(created)(0), FIELD(connections_amount)(0)
{
    FIELD(channels) = std::make_shared<std::list<std::shared_ptr<const chatd::channel>>>();
}

DEFINITION_SPECIAL_MEMBERS(status)

} // end of namespace
