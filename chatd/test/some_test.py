#!/usr/bin/env python

import websocket
import thread
import time

request_create_user = "{\"id\":1,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_user\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_login = "{\"id\":2,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"login\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_create_channel = "{\"id\":3,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_channel\",\"vr\":1,\"name\":\"new_channel\",\"description\":\"This is some new channel\"}}"
request_look_for_channel = "{\"id\":4,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"look_for_channel\",\"vr\":1,\"text\":\"test\"}}"
request_come_in = "{\"id\":5,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"come_in\",\"vr\":1,\"channel_oid\":1964630416631}}"
request_message = "{\"id\":22,\"tp\":\"message\",\"vr\":1,\"channel_oid\":1964630416632,\"sender_oid\":112233,\"recipient_oid\":0,\"encrypted\":false,\"payload\":\"some text\"}"
request_come_out = "{\"id\":7,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"come_out\",\"vr\":1,\"channel_oid\":1964630416631}}"
request_logout = "{\"id\":8,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"logout\",\"vr\":1}}"


def send(ws, message):
    print "Request:"
    print message
    ws.send(message)
    time.sleep(1)

def on_message(ws, message):
    print "Response:"
    print message

def on_error(ws, error):
    print error

def on_close(ws):
    print "### closed ###"

def on_open(ws):
    def run(*args):
        send(ws, request_create_user)
        send(ws, request_login)
        send(ws, request_create_channel)
        send(ws, request_look_for_channel)
        send(ws, request_come_in)
        send(ws, request_message)
        send(ws, request_come_out)
        send(ws, request_logout)
        ws.close()
        print "thread terminating..."
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    websocket.enableTrace(False)
    ws = websocket.WebSocketApp("wss://friendsoft.ru/ws/",
        on_message = on_message,
        on_error = on_error,
        on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
