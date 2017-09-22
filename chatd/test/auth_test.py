#!/usr/bin/env python

import websocket
import thread
import time

request_create_user = "{\"id\":1,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"create_user\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_login1 = "{\"id\":2,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"login\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\",\"hash\":\"0\"}}"
request_login2 = "{\"id\":3,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"login\",\"vr\":1,\"nickname\":\"test\",\"password\":\"\",\"hash\":\"0\"}}"
request_get_status = "{\"id\":4,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"get_status\",\"vr\":1}}"
request_logout = "{\"id\":5,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"logout\",\"vr\":1}}"


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
        send(ws, request_login1)
        send(ws, request_login2)
        send(ws, request_get_status)
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
