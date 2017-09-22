#include <boost/functional/hash.hpp>

#include "message.h"
#include "defines_key_store.h"

namespace chatd
{

DEFINITION_FIELDS_FUNCTIONS_STORES(message)

DEFINITION_CONSTS(message, "message", BIT_INCREMENT | BIT_SYNCHRONIZE, 1)

DEFINITION_FIELD(message, OID, id)
DEFINITION_FIELD(message, OID, channel_oid)
DEFINITION_FIELD(message, OID, sender_oid)
DEFINITION_FIELD(message, OID, recipient_oid)
DEFINITION_FIELD(message, time_tt, time)
DEFINITION_FIELD(message, bool, encrypted)
DEFINITION_FIELD(message, sptr_cstr, payload)

message::message() : FIELD(id)(0), FIELD(channel_oid)(0), FIELD(sender_oid)(0), FIELD(recipient_oid)(0), FIELD(time)(time(NULL)), FIELD(encrypted)(false) {}

// обязательные методы ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_INIT_KEYS(message,
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_message_recipient)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_message_ascend)
    DEFINITION_KEY_STORE(MULTI_MODEL, MULTI_SORTED_STORE, key_message_descend)
)

DEFINITION_UNIQUE_KYES(message)
DEFINITION_SERIALIZATION(message)
DEFINITION_DESERIALIZATION_1(message)
DEFINITION_DESERIALIZATION_2(message)
DEFINITION_MAKE_COPY(message)
DEFINITION_SET_INDEX_FIELDS(message)

// ключи ++++++++++++++++++++++++++++++++++++++++++++++++++++++

DEFINITION_KEY(key_message_recipient, INIT(recipient_oid) INIT(time), EQUAL(recipient_oid) && EQUAL(time), MORE(recipient_oid) || EQUAL(recipient_oid) && MORE(time))
DEFINITION_FIELD(key_message_recipient, OID, recipient_oid)
DEFINITION_FIELD(key_message_recipient, time_tt, time)
size_t key_message_recipient::hash() const
{
    std::size_t res = 0 ;
    boost::hash_combine(res, FIELD(recipient_oid));
    boost::hash_combine(res, FIELD(time));
    return res;
}

DEFINITION_KEY(key_message_ascend, INIT(channel_oid) INIT(time), EQUAL(channel_oid) && EQUAL(time), LESS(channel_oid) || EQUAL(channel_oid) && LESS(time))
DEFINITION_FIELD(key_message_ascend, OID, channel_oid)
DEFINITION_FIELD(key_message_ascend, time_tt, time)
size_t key_message_ascend::hash() const
{
    std::size_t res = 0 ;
    boost::hash_combine(res, FIELD(channel_oid));
    boost::hash_combine(res, FIELD(time));
    return res;
}


DEFINITION_KEY(key_message_descend, INIT(channel_oid) INIT(time), EQUAL(channel_oid) && EQUAL(time), MORE(channel_oid) || EQUAL(channel_oid) && MORE(time))
DEFINITION_FIELD(key_message_descend, OID, channel_oid)
DEFINITION_FIELD(key_message_descend, time_tt, time)
size_t key_message_descend::hash() const
{
    std::size_t res = 0 ;
    boost::hash_combine(res, FIELD(channel_oid));
    boost::hash_combine(res, FIELD(time));
    return res;
}

} // end of namespace
