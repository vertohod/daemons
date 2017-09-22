#include "found_user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(found_user)

DEFINITION_CONSTS(found_user, "found_user", BIT_NULL, 1)

DEFINITION_FIELD(found_user, OID, user_oid)
DEFINITION_FIELD(found_user, sptr_cstr, nickname)

found_user::found_user() {}

DEFINITION_SPECIAL_MEMBERS(found_user)

} // end of namespace
