#include "get_user_list.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(get_user_list)

DEFINITION_CONSTS(get_user_list, "get_user_list", BIT_NULL, 1)

DEFINITION_FIELD(get_user_list, OID, channel_oid)

get_user_list::get_user_list() {}

DEFINITION_SPECIAL_MEMBERS(get_user_list)

} // end of namespace
