#include "changed_nickname.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(changed_nickname)

DEFINITION_CONSTS(changed_nickname, "changed_nickname", BIT_NULL, 1)

DEFINITION_FIELD(changed_nickname, OID, user_oid)
DEFINITION_FIELD(changed_nickname, sptr_cstr, nickname)
DEFINITION_FIELD(changed_nickname, sptr_cstr, nickname_past)

changed_nickname::changed_nickname() {}

DEFINITION_SPECIAL_MEMBERS(changed_nickname)

} // end of namespace
