#include <ctime>

#include "pong.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(pong)

DEFINITION_CONSTS(pong, "pong", BIT_NULL, 1)

DEFINITION_FIELD(pong, OID, id)
DEFINITION_FIELD(pong, time_tt, time)

pong::pong() : FIELD(id)(0), FIELD(time)(time(NULL)) {}

DEFINITION_SPECIAL_MEMBERS(pong)

} // end of namespace
