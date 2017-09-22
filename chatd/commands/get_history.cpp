#include "get_history.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(get_history)

DEFINITION_CONSTS(get_history, "get_history", BIT_NULL, 1)

DEFINITION_FIELD(get_history, OID, channel_oid)
DEFINITION_FIELD(get_history, time_tt, time)
DEFINITION_FIELD(get_history, size_tt, limit)

get_history::get_history() {}

DEFINITION_SPECIAL_MEMBERS(get_history)

} // end of namespace
