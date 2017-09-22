#include "look_for_user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(look_for_user)

DEFINITION_CONSTS(look_for_user, "look_for_user", BIT_NULL, 1)

DEFINITION_FIELD(look_for_user, sptr_cstr, text)

look_for_user::look_for_user() {}

DEFINITION_SPECIAL_MEMBERS(look_for_user)

} // end of namespace
