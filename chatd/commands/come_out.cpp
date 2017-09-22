#include "come_out.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(come_out)

DEFINITION_CONSTS(come_out, "come_out", BIT_NULL, 1)

DEFINITION_FIELD(come_out, OID, channel_oid)

come_out::come_out() : FIELD(channel_oid)(0) {}

DEFINITION_SPECIAL_MEMBERS(come_out)

} // end of namespace
