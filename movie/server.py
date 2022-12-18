while True:
    import socket,os
    server = socket.socket()
    server.bind(('143.198.100.127',9999))
    server.listen()
    while True:
        print("\n\n等待连接...")
        #等待客户端连接
        filename1 = "RecordNew.txt"
        skt, addr = server.accept()
        print("new connect:",addr)
        #接收新产生的观影记录
        d = skt.recv(1024)
        with open(filename1, mode='ab') as f:
            f.write(d)
        f.close()
        print('接收完毕')
        #发送数据库和记录文件
        server.listen()
        i=0
        skt, addr = server.accept()
        while i<3:
            data= skt.recv(1024)
            fileName2 = data.decode()
            if os.path.isfile(fileName2):
                print("fileName is：",fileName2)
                f = open(fileName2,"rb")
                #获取文件大小
                file_size = os.stat(fileName2).st_size
                #向客户端发送文件大小
                skt.send(str(file_size).encode())
                for line in f:
                    #发送数据
                    skt.send(line)  
                print("发送完毕")
                f.close()
                i=i+1
    server.close()