#include "come_in.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(come_in)

DEFINITION_CONSTS(come_in, "come_in", BIT_NULL, 1)

DEFINITION_FIELD(come_in, OID, channel_oid)

come_in::come_in() : FIELD(channel_oid)(0) {}

DEFINITION_SPECIAL_MEMBERS(come_in)

} // end of namespace
