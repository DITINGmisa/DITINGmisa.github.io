import socket
server = socket.socket()
server.bind(('143.198.100.127',9999))
server.listen()
print("\n\n等待连接...")
skt, addr = server.accept()
print("连接来自：" + str(addr))
#等待客户端连接
flag=True
while flag:
    data= skt.recv(1024)
    message = data.decode()
    if message != "end!!!":
        print("\nrecv:" + message + "\nsend：" + message)
        skt.send(message.encode()) 
        print("发送完毕")
    else :
        print("对方断开连接")
        flag=False
server.close()