#include "inside.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(inside)

DEFINITION_CONSTS(inside, "inside", BIT_NULL, 1)

DEFINITION_FIELD(inside, OID, user_oid)
DEFINITION_FIELD(inside, sptr_cstr, nickname)
DEFINITION_FIELD(inside, bool, flag)

inside::inside() : FIELD(user_oid)(0), FIELD(flag)(true) {}

DEFINITION_SPECIAL_MEMBERS(inside)

} // end of namespace
