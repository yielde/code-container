#!/bin/bash
###
# @Author: Galen Tong
# @Date: 2022-12-05 18:14:21
 # @LastEditTime: 2022-12-06 23:18:15
# @Description: replace some settings
###


# vps settings
SHADOW_SERVER_PATH=/usr/local/bin/ssserver
SHADOW_FILE_PATH=/etc/shadowconfig.json
KCP_SERVER_PATH=/root/server_linux_amd64
KCP_FILE_PATH=/root/kcp.json
RESTART_FALG=""

# aliyun settings
REMOTE_KCP_CLIENT_PATH=/home/psj/kcpfun/client_linux_amd64
REMOTE_KCP_PATH=/home/psj/kcpfun/myvps.json
REMOTE_KCP_LOG_PATH=/home/psj/kcpfun/nohup.log
REMOTE_IP="1.117.44.47"


change_ips()
{
    echo "---------- change ips ----------"
    OLD_IP=$(echo $(sed -n 's/"server": "\(.*\)",/\1/p' $SHADOW_FILE_PATH))
    sed -i "s/$OLD_IP/$NEW_IP/g" $SHADOW_FILE_PATH
    echo "[1] change shadow ip completed!"

    OLD_IP=$(echo $(sed -n 's/"target": "\(.*\):.*",/\1/p' $KCP_FILE_PATH))
    sed -i "s/$OLD_IP/$NEW_IP/g" $KCP_FILE_PATH
    echo "[2] change kcp ip completed!"
}

change_ports()
{
    echo "---------- change ports ----------"
    OLD_PORT=$(echo $(sed -n 's/"listen": ":\(.*\)",/\1/p' $KCP_FILE_PATH))
    sed -i "s/$OLD_PORT/$NEW_PORT/g" $KCP_FILE_PATH
    echo "[1] change kcp path completed!"
}

change_aliyun()
{
    echo "---------- change aliyun ----------"
    
    IP=$(echo $(sed -n 's/"server": "\(.*\)",/\1/p' $SHADOW_FILE_PATH))
    PORT=$(echo $(sed -n 's/"listen": ":\(.*\)",/\1/p' $KCP_FILE_PATH))
    OLD=$(sshpass -p "$PASSWD" ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR root@$REMOTE_IP "cat $REMOTE_KCP_PATH|grep remoteaddr" $REMOTE_KCP_PATH)
    OLD=$(echo $OLD|sed -n 's/"remoteaddr": "\(.*\)",/\1/p')
    sshpass -p "$PASSWD" ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR root@$REMOTE_IP sed -i "s/$OLD/$IP:$PORT/g" $REMOTE_KCP_PATH
    echo "[1] change port completed!"
    
    PID=$(sshpass -p "cleverman" ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR root@$REMOTE_IP "pidof client_linux_amd64")
    sshpass -p "$PASSWD" ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR root@$REMOTE_IP "kill -9 $PID"
    echo "[2] stop client_linux_amd64 completed!"

    sshpass -p "$PASSWD" ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR root@$REMOTE_IP "nohup $REMOTE_KCP_CLIENT_PATH -c $REMOTE_KCP_PATH >/dev/null 2>&1 &"
    echo "[3] start client_linux_amd64 completed!"
}

restart_service()
{
    echo "---------- restart service ----------"
    # restart kcp
    kill -9 $(pidof server_linux_amd64)
    nohup $KCP_SERVER_PATH -c $KCP_FILE_PATH &
    echo "[1] restart kcp completed!"
    
    # restart shadowsocks
    $SHADOW_SERVER_PATH -c $SHADOW_FILE_PATH -d stop
    sleep 1
    $SHADOW_SERVER_PATH -c $SHADOW_FILE_PATH -d start
    echo "[2] restart shadow completed!"
}

help()
{
    echo "Usage: replace_ips.sh [-n NEW_IP] [-p NEW_PORT] [-r] [-d PASSWRD]"
    echo "Example: replace_ips.sh -n 172.17.77.33 -p 29903 -r -d password"
    echo ""
    echo "-h           help info"
    echo "-p NEW_PORT  The new port you want to set"
    echo "-n NEW_IP    The new public IP you want to set"
    echo "-r           restart service(kcp shadow)"
    echo "-d PASSWRD   change aliyun kcp settings"
}

# main
if [ $# == 0 ] ;then
    help
fi

while getopts "rn:p:d:h" OPT
do
    case $OPT in
        n)  NEW_IP=$OPTARG;;
        p)  NEW_PORT=$OPTARG;;
        r)  RESTART_FALG=1;;
        d)  PASSWD=$OPTARG;;
        h)  help;;
        ?)  help
        exit 1;;
    esac
done

if [ -n "$NEW_IP" ]; then
    change_ips
fi

if [ -n "$NEW_PORT" ]; then
    change_ports
fi

if [ -n "$RESTART_FALG" ]; then
    restart_service
    RESTART_FALG=""
fi

if [ -n "$PASSWD" ]; then
    change_aliyun
fi

exit 0