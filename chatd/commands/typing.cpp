#include "typing.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(typing)

DEFINITION_CONSTS(typing, "typing", BIT_NULL, 1)

DEFINITION_FIELD(typing, OID, user_oid)
DEFINITION_FIELD(typing, sptr_cstr, nickname)
DEFINITION_FIELD(typing, bool, flag)

typing::typing() : FIELD(user_oid)(0), FIELD(flag)(true) {}

DEFINITION_SPECIAL_MEMBERS(typing)

} // end of namespace
