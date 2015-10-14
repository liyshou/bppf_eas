#!/usr/bin/env python
# coding=utf-8
import paramiko
import commands
import datetime
import ConfigParser
import sys
import os
import re
import time
class BpepConfig:
    def __init__(self,option_name,conf_dir,env):
        self.section_name = "bpep"
        #模块名
        self.option_name = option_name
        self.env = env
        #生成一个配置文件对象，读取配置表
        self.conf_file = ConfigParser.ConfigParser()
        #取消默认读取的option值不区分大小写
        self.conf_file.optionxform = str
        #读取配置文件
        self.conf_file.read(conf_dir)  
        #从bpep.ini获取jenkins存放gz包目录
        self.jenkins_dir = self.conf_file.get(self.section_name,self.option_name).split('|')[0]
        #从bpep.ini获取登录应用服务器的用户    
        self.user = self.conf_file.get(self.section_name,self.option_name).split('|')[4]
        #判断家目录的是在u1下是home底下
        if self.env == "dev":
           home_dir = '/u1/%s/' %self.user
        else: 
           home_dir = '/home/%s/' %self.user
        #从bpep.ini获取应用服务器gz包存放目录
        self.gz_dir =  home_dir+self.conf_file.get(self.section_name,self.option_name).split('|')[1]
        #从bpep.ini获取应用服务器的应用目录
        self.app_dir = home_dir+self.conf_file.get(self.section_name,self.option_name).split('|')[2]
        #从bpep.ini获取应用服务器的备份目录
        self.app_bak_dir = home_dir+self.conf_file.get(self.section_name,self.option_name).split('|')[3]
    
    #在选定的环境中匹配gz包名称
    def get_tar_gz(self):
        list = os.listdir(self.jenkins_dir)
        re_tar_gz = re.compile(r'(\S*\.tar\.gz)$')
        for file_name in list:
           if re_tar_gz.match(file_name):
                 return file_name
#发布类
class Release:
    def __init__(self,ip,user,passwd,gz_name,gz_dir,app_name,target_dir,jenkins_dir,bak_dir):
        #初始化IP地址
        self.ip = ip
        #初始化用户登录
        self.user = user
        self.passwd = passwd 
        #生成paramiko的SSHClient对象，用于ssh连接
        self.ssh = paramiko.SSHClient()
        #允许连接不在know_hosts文件中的主机。
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        #建立连接
        self.ssh.connect(self.ip,22,self.user,self.passwd)
        #获取gz包名称
        self.gz_name = gz_name
        #获取gz包存放目录
        self.gz_dir = gz_dir
        #获取应用模块名
        self.app_name = app_name
        #获取应用服务器应用目录
        self.target_dir = target_dir
        #获取jenkins存放应用的目录
        self.jenkins_dir = jenkins_dir
        #获取应用服务器应用备份的目录
        self.bak_dir = bak_dir
        #判断并创建存储旧应用文件的临时目录、备份目录、存放gz包目录
        stdin, stdout, stderr = self.ssh.exec_command("if [ ! -d $HOME/temp ];then mkdir -p $HOME/temp; fi")
        stdin, stdout, stderr = self.ssh.exec_command("if [ ! -d $HOME/bak ];then mkdir -p $HOME/bak; fi")
        stdin, stdout, stderr = self.ssh.exec_command("if [ ! -d $HOME/source ];then mkdir -p $HOME/source; fi")
    #文件上传
    def put_file(self):
        #初始化传输通道
        t = paramiko.Transport((self.ip,22))
        #建立连接
        t.connect(username = self.user, password = self.passwd)
        #建立sftp进行文件传输
        sftp = paramiko.SFTPClient.from_transport(t)
        #jenks上gz包 
        jenkins_path=self.jenkins_dir+self.gz_name
        #应用服务器的gz包
        remote_path=self.gz_dir+self.gz_name
        #文件上传
        sftp.put(jenkins_path,remote_path)
        t.close()
    #文件备份
    def file_bak(self): 
        #生成以当前时间的字符串，用于备份gz包使用
        now = datetime.datetime.now().strftime("%Y%m%d%H%M") 
        cmd = "cd $HOME/temp/;tar -zcf %s%s.tar.gz%s ./* " %(self.bak_dir,self.app_name,now)
        print "发送文件备份指令：%s" %(cmd)
        #文件备份指令
        stdin, stdout, stderr = self.ssh.exec_command(cmd)
        stdout.read()
        print "显示错误信息：%s" %(stderr.read())
    #文件删除(使用mv移到一个临时目录，下次更新时会自动删除该目录内容)	
    def file_del(self): 
        cmd1 = "cd $HOME/temp/;rm -rf ./*"
        #mv前删除上一次应用的文件
        stdin, stdout, stderr = self.ssh.exec_command(cmd1)
        stdout.read()
        print "显示错误信息：%s" %(stderr.read())
        cmd2 = "mv %s $HOME/temp/" %(self.target_dir)
        print "发送文件删除指令：%s" %(cmd2)
        #执行移动旧应用的文件
        stdin, stdout, stderr = self.ssh.exec_command(cmd2)
        stdout.read()
        print "显示错误信息：%s" %(stderr.read())
    #文件解压
    def file_extract(self):
        cmd = "cd $HOME;tar -zxvf %s" %(self.gz_dir+self.gz_name)
        print "发送文件解压指令：%s" %(cmd)
        #文件解压指令
        stdin, stdout, stderr = self.ssh.exec_command(cmd)
        stdout.read()
        print "显示错误信息：%s" %(stderr.read())
#        self.ssh.close()
    #文件校验
    def check_sum(self):
        print "进行cksum值校验。"
        #获取本地Server应用cksum值
        (status,result)=commands.getstatusoutput('cd %s;cksum %s' %(self.jenkins_dir,self.gz_name))
        local_cksum = result.split(' ')
        #获取远程Server应用cksum值
        stdin, stdout, stderr = self.ssh.exec_command("cd %s;cksum %s" %(self.gz_dir,self.gz_name))
        remote_cksum = stdout.read().strip('\n').split(' ')
        print "jekins上包名称：%s  本地cksum值：%s 远程cksum值：%s" %(self.gz_name,local_cksum,remote_cksum)
        #返回一个存储本地与应用服务器的cksum值元组
        return (local_cksum,remote_cksum) 
    #重启应用
    def app_restart(self):
        #重启应用指令
        cmd = "cd %s;sh startup.sh restart" %(self.target_dir)
        print "发送应用重启指令：%s" %(cmd)
        stdin, stdout, stderr = self.ssh.exec_command(cmd)
        stdout.read()
        print "显示错误信息：%s" %(stderr.read())
        self.ssh.close()
    #检查旧文件是否移除干净
    def check_app(self):
        stdin, stdout, stderr = self.ssh.exec_command("cd $HOME;if [ -d %s ];then echo '1';else echo '0';fi" %(self.target_dir))
        result = stdout.read()
        return result
#判断脚本运行模式
if len(sys.argv) != 4:
    print "脚本运行模式:python " + sys.argv[0] + " 应用名称 IP地址1,IP地址2 环境参数"
    print "脚本运行模式:python " + sys.argv[0] + " bga-service 192.168.127.40,192.168.127.41 dev"
    sys.exit(-1)
else:
    #接收模块名
    app_name = sys.argv[1]
    #接收ip地址，如有多个地址使用逗号隔开
    ip = sys.argv[2]
    #接收环境参数
    env = sys.argv[3]
    #登录linux密码
    if env == 'dev':
       passwd = 'xxx'
    else:
       passwd = 'xxx'
    #print passwd
    #存放配置文件的目录参数
    conf_dir = "/u1/jenkins2/python_script/bpep.ini"
    #初始化一个配置类
    conf = BpepConfig(app_name,conf_dir,env)
    gz_name =  conf.get_tar_gz()
    if env in gz_name:
      #根据接收的IP参数进行发布操作
      for ip in sys.argv[2].split(','):
        #初始化一个发布类
        r=Release(ip,conf.user,passwd,gz_name,conf.gz_dir,app_name,conf.app_dir,conf.jenkins_dir,conf.app_bak_dir)
        #文件上传操作
        r.put_file()
        #文件移除操作
        r.file_del()
        #文件备份操作
        r.file_bak()
        #文件校验操作
        cksum=r.check_sum()
        if cksum[0] == cksum[1]:
            #如果cksum值正确，则解压文件，重启应用
            check_result = r.check_app()
            while True:
                print check_result
                print type(check_result)
                print int(check_result) == 1
                if int(check_result) == 1:
                   print "正在删除旧文件。"
                   time.sleep(15)
                   check_result = r.check_app()
                else:
                   print "正在部署应用"
                   r.file_extract()
                   r.app_restart()
                   break
        else:
            print "文件校验失败"
    else:
        print "未发现有%s，请检测环境选择是否错误，当前选择环境为%s" %(gz_name,env) 
