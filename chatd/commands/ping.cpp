#include <ctime>

#include "ping.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(ping)

DEFINITION_CONSTS(ping, "ping", BIT_NULL, 1)

DEFINITION_FIELD(ping, OID, id)
DEFINITION_FIELD(ping, time_tt, time)

ping::ping() : FIELD(id)(0), FIELD(time)(time(NULL)) {}

DEFINITION_SPECIAL_MEMBERS(ping)

} // end of namespace
