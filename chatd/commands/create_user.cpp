#include "create_user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(create_user)

DEFINITION_CONSTS(create_user, "create_user", BIT_NULL, 1)

DEFINITION_FIELD(create_user, sptr_cstr, nickname)
DEFINITION_FIELD(create_user, sptr_cstr, password)

create_user::create_user() {}

DEFINITION_SPECIAL_MEMBERS(create_user)

} // end of namespace
