#include <boost/functional/hash.hpp>

#include "channel_user.h"
#include "defines_key_store.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(channel_user)

DEFINITION_CONSTS(channel_user, "channel_user", BIT_INCREMENT | BIT_SYNCHRONIZE, 1)

DEFINITION_FIELD(channel_user, OID, channel_oid)
DEFINITION_FIELD(channel_user, OID, user_oid)

channel_user::channel_user() : FIELD(channel_oid)(0), FIELD(user_oid)(0) {}

DEFINITION_INIT_KEYS(channel_user,
    DEFINITION_KEY_STORE(UNIQUE_MODEL, UNIQUE_UNSORTED_STORE, key_channel_user_channel_user)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_UNSORTED_STORE, key_channel_user_user)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_UNSORTED_STORE, key_channel_user_channel)
)

DEFINITION_UNIQUE_KYES(channel_user)
DEFINITION_SERIALIZATION(channel_user)
DEFINITION_DESERIALIZATION_1(channel_user)
DEFINITION_DESERIALIZATION_2(channel_user)
DEFINITION_MAKE_COPY(channel_user)
DEFINITION_SET_INDEX_FIELDS(channel_user)


// ------------------------ keys -------------------------------------
// USER
DEFINITION_KEY(key_channel_user_user, INIT(user_oid), EQUAL(user_oid), LESS(user_oid))
DEFINITION_FIELD(key_channel_user_user, OID, user_oid)
size_t key_channel_user_user::hash() const
{
	return std::hash<OID>()(FIELD(user_oid));
}

// CHANNEL
DEFINITION_KEY(key_channel_user_channel, INIT(channel_oid), EQUAL(channel_oid), LESS(channel_oid))
DEFINITION_FIELD(key_channel_user_channel, OID, channel_oid)
size_t key_channel_user_channel::hash() const
{
	return std::hash<OID>()(FIELD(channel_oid));
}

// CHANNEL_USER
DEFINITION_KEY(key_channel_user_channel_user, INIT(channel_oid) INIT(user_oid), EQUAL(channel_oid) && EQUAL(user_oid), LESS(channel_oid) || EQUAL(channel_oid) && LESS(user_oid))
DEFINITION_FIELD(key_channel_user_channel_user, OID, channel_oid)
DEFINITION_FIELD(key_channel_user_channel_user, OID, user_oid)
size_t key_channel_user_channel_user::hash() const
{
    std::size_t res = 0 ;
    boost::hash_combine(res, FIELD(channel_oid));
    boost::hash_combine(res, FIELD(user_oid));
    return res;
}

} // end of namespace chatd
