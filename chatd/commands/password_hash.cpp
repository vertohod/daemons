#include "password_hash.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(password_hash)

DEFINITION_CONSTS(password_hash, "password_hash", BIT_NULL, 1)

DEFINITION_FIELD(password_hash, sptr_cstr, value)

password_hash::password_hash() {}

DEFINITION_SPECIAL_MEMBERS(password_hash)

} // end of namespace
