#include "rename_user.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(rename_user)

DEFINITION_CONSTS(rename_user, "rename_user", BIT_NULL, 1)

DEFINITION_FIELD(rename_user, sptr_cstr, nickname)

rename_user::rename_user() {}

DEFINITION_SPECIAL_MEMBERS(rename_user)

} // end of namespace
