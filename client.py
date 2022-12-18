import socket

client  = socket.socket()
client.connect(('192.168.1.125',6666))

filename = "RecordNew.txt"
client.sendall(filename.encode())

print("发送完毕")
client.close()

    



