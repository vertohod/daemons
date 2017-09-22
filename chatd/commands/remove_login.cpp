#include "remove_login.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(remove_login)

DEFINITION_CONSTS(remove_login, "remove_login", BIT_NULL, 1)

DEFINITION_FIELD(remove_login, sptr_cstr, password)

remove_login::remove_login() : {}

DEFINITION_SPECIAL_MEMBERS(remove_login)

} // end of namespace
