# UDP Server Demo

這是一個UDP Server的範例程式，使用C語言實做。

## 範例影片

https://github.com/Meitaiyang/UDPEchoClient/assets/76491618/f7ca6e81-5b9c-400e-ad60-bfd62bdaaa08

## 安裝

你可以使用Docker來建立Debain環境，並且使用以下指令來安裝所需的套件。

```bash
sudo docker run -it --name UDP -v $PWD:/source debian:10
apt-get update
apt-get install -y gcc
```

如果你想要進行測試，你可以使用以下指令來安裝所需的套件。

```bash
apt-get install -y gdb
```

## 編譯
### Server

請使用gcc對server.c進行編譯。

```bash
gcc -g server.c -o server
```

### Client

請使用gcc對client.c進行編譯。

```bash 
gcc -g client.c -lm -o client
```

## 執行

在Server中，若輸入`--no-resp`，則Server只會接收，不會回應Client的請求。Client會在exponential backoff的設定時間後停止等待。

執行Server，並決定要不要回應Client的請求。
```bash
./server 
./server --no-resp
```

執行Client。
```bash
./client
```
