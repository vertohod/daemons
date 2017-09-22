#include "login.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(login)

DEFINITION_CONSTS(login, "login", BIT_NULL, 1)

DEFINITION_FIELD(login, sptr_cstr, nickname)
DEFINITION_FIELD(login, sptr_cstr, password)
DEFINITION_FIELD(login, sptr_cstr, hash)

login::login() {}

DEFINITION_SPECIAL_MEMBERS(login)

} // end of namespace
