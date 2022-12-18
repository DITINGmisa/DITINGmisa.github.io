import socket,os,hashlib

server  = socket.socket()
server.bind(('143.198.100.127',9999))

server.listen()

while True:
    conn, addr = server.accept()
    print("new connect:",addr)
    while True:
        data= conn.recv(1024)
        if not data:
            print("客户端已断开")
            break
        cmd,fileName = data.decode().split()
        print("fileName is：",fileName)
        if os.path.isfile(fileName):
            f = open(fileName,"rb")
            m= hashlib.md5()
            #获取文件大小
            file_size = os.stat(fileName).st_size
            
            #向客户端发送文件大小
            conn.send(str(file_size).encode())
            #wait for ack
            conn.recv(1024)
            
            for line in f:
                # 获取md5
                m.update(line) 
                #发送数据
                conn.send(line)  
            
            print("file md5:",m.hexdigest())
            conn.send(m.hexdigest().encode())
            f.close()
             
#             
            
            
server.close()