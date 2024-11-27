from socket import socket, AF_INET, SOCK_STREAM

PORT = 6000

with socket(AF_INET, SOCK_STREAM) as sock:
    sock.bind(("127.0.0.1", PORT))
    sock.listen(1)
    print(f"serving at port {PORT}")
    print("waiting for connection")
    client, client_addr = sock.accept()
    print(f"connection from {client_addr}")

    print("packet in this format:")
    print("{stream_id} {func_id}: {data}")
    while True:
        raw_data = input()
        
        try:
            data_sep_idx = raw_data.find(": ")
            ids, data = raw_data[:data_sep_idx], raw_data[data_sep_idx+2:]

            stream_id, func_id = ids.split(" ")
            stream_id = int(stream_id)
            func_id = int(func_id)
        except Exception:
            print("format error")
            continue

        # create packet
        packet = b''
        packet += stream_id.to_bytes(1)
        packet += func_id.to_bytes(1)
        packet += data.encode()
        client.send(packet)