import threading
import atexit
from socket import socket, AF_INET, SOCK_STREAM
import base64

from flask import Flask, render_template, request, copy_current_request_context
from flask_socketio import SocketIO, emit

app = Flask(__name__)
app.config['TEMPLATES_AUTO_RELOAD'] = True
socketio = SocketIO(app, debug=True)

DEFAULT_DEBUG_PORT = 500

def conn_print(sid, msg, dir_to_server):
    dir = "C -> S" if dir_to_server else "C <- S"
    print(f"{dir} {hex(hash(sid))[-4:]}: {msg}")

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on("send")
def send_cmd(data):
    msg = data["data"]

    sid = request.sid
    conn_print(sid, msg, True)
    # fetch socket
    with connections_dict_lock:
        if sid not in connections:
            # no socket found
            emit("connection_report", {
                "open": False, 
                "reason": "closed previously, or never opened"
            }, to=sid)
            return

        sock, _ = connections[sid]
    
    # send message
    sock.send(msg.encode())

@socketio.on("disconnect")
def frontend_disconnected():
    close_server_socket(request.sid, "frontend disconnected")

@socketio.on("close")
def disconnect_cmd():
    # close connection
    close_server_socket(request.sid, "connection closed by frontend")

@socketio.on("open")
def connect_cmd(data):
    # remove any duplicate connections
    sid = request.sid
    with connections_dict_lock:
        if sid in connections:
            close_server_socket(sid, "new session opened")

    raw_address = data["addr"]
    split_adress = raw_address.split(':')

    ip_addr = split_adress[0]
    port = DEFAULT_DEBUG_PORT
    if len(split_adress) == 2:
        port = int(split_adress[1])

    # check whether there are already connections
    # to this ip, and if so disconnect them
    with connections_dict_lock:
        for _sid in list(connections.keys()):
            item = connections.get(_sid, None)
            if item is None:
                continue

            sock, _ = item
            if sock.getpeername() == (ip_addr, port):
                close_server_socket(_sid, "new session on another client")

    connect_to_server(ip_addr, port, sid)

connections_dict_lock = threading.RLock()
connections: dict[int, tuple[socket, threading.Thread]] = {}

def close_server_socket(sid, reason=None):    
    # as soon as socket is closed
    # recv thread will exit beacause of OSError raised
    # close socket
    with connections_dict_lock:
        sock, _ = connections.pop(sid, (None, None))
    if sock:
        sock.close()

    # notify client
    emit("connection_report", {
                "open": False, 
                "reason": reason
            }, to=sid)

def recv_forever(sock: socket, sid):
    close_reason = "connection closed by server"
    try:
        while True:
            # will block till recieves data
            data = sock.recv(4096)
            # test client dissconnect
            if data == b'':
                return

            conn_print(sid, data, False)
            # convert data
            data = base64.encodebytes(data).decode()
            # forward data
            emit("recv", {
                "data": data
            }, to=sid)
    except OSError:
        # ignore OSErrors
        pass
    except Exception as e:
        # something bad happened to the connection
        close_reason = e
    finally:
        # close socket and remove thread
        # from the connections list
        close_server_socket(sid, close_reason)

def connect_to_server(ip, port, sid):
    sock = socket(AF_INET, SOCK_STREAM)

    try:
        sock.connect((ip, port))
    except Exception:
        # connection refused
        emit("connection_report", {
            "open": False, 
            "reason": "connection refused"
        }, to=sid)
        return
    
    # connection established
    emit("connection_report", {
        "open": True,
        "reason": None
    }, to=sid)

    # start recv thread
    # we make it daemonic cause we assure that at 
    # main thread exit all sockets will be closed
    recv_thread = threading.Thread(target=copy_current_request_context(recv_forever), args=(sock, sid), daemon=True)
    
    # add to connections list
    with connections_dict_lock:
        connections[sid] = (sock, recv_thread)

    recv_thread.start()

def graceful_shutdown():
    # close remaining sockets
    with connections_dict_lock:
        for sock, _ in connections.values():
            sock.close()

atexit.register(graceful_shutdown)
