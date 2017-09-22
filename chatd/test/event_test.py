#!/usr/bin/env python

import websocket
import thread
import time

request_login = "{\"id\":2,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"login\",\"vr\":1,\"nickname\":\"test\",\"password\":\"asdf\"}}"
request_come_in = "{\"id\":5,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"come_in\",\"vr\":1,\"channel_oid\":1964630416631}}"
request_event = "{\"tp\":\"event\",\"vr\":1,\"time\":1491768206,\"channel_oid\":1964630416631,\"recipient_oid\":0,\"payload\":{\"tp\":\"typing\",\"vr\":1,\"user_oid\":1374917674892,\"nickname\":\"test\",\"flag\":true}}"
request_come_out = "{\"id\":7,\"tp\":\"command\",\"vr\":1,\"pl\":{\"tp\":\"come_out\",\"vr\":1,\"channel_oid\":1964630416631}}"

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
        send(ws, request_login)
        send(ws, request_come_in)
        send(ws, request_event)
        send(ws, request_come_out)
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
