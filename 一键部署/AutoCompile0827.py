#!/user/bin/python
#-*- coding: utf-8 -*-

'''
@create time:2015/07/13
@author: liys
@desc: 核心一键编译 

'''
import commands
import os
import ConfigParser
import time,datetime
import sys
import paramiko
import pxssh
from LoggingInit import LOGDebug,LOGError#相当于从另外的py读取函数

script_addr = '/app/bppf_omp/eas_script/'

#读取配置文件conf.ini
def GetConf(confName,label,res):
	config=ConfigParser.SafeConfigParser()
	config.read(confName)
	result = config.get(label,res)
       	return result

#编译错误异常类
class CompileError(Exception):
	def __init__(self, msg):
		Exception.__init__(self)
		self.msg = msg

class ModelNotExistError(Exception):
	def __init__(self, msg):
		Exception.__init__(self)
		self.msg = msg

#获得小程序名与工程名的映射字典
def jobMapping():
	dictProgNames = {}
	originPath = os.getcwd()
	homeaddr = GetConf(script_addr + 'conf.ini','home','addr')
	os.chdir(homeaddr)
	fileHandle = open(script_addr + 'programNameMapping')
	for line in fileHandle.readlines():
		listName = line.split(" ")
		dictProgNames[listName[0].strip()] = listName[1].strip()
	os.chdir(originPath)
	return dictProgNames
	
	
#组装编译参数
def ConstructPara(servPara, jobPara, mgrPara):
	modelDict = {}
	servModelName = ""
	jobModelName = ""
	mgrModelName = ""
	servNames = servPara[1:len(servPara)-1]
	jobNames = jobPara[1:len(jobPara)-1]
	mgrNames = mgrPara[1:len(mgrPara)-1]
	
	if servNames != "":
		servModelName = servNames.split(',')
	if jobNames != "":
		jobModelName = jobNames.split(',')
	if mgrNames != "":
		mgrModelName = mgrNames.split(',')
	
	modelDict = {'serv':servModelName,'job':jobModelName,'mgr':mgrModelName}
	
	return modelDict

#组环境参数
def ConstructEnvPara(envPara):
	envNames = envPara[1:len(envPara)-1]
	envName = []
	
	if envNames != "":
		envName = envNames.split(',')
	return envName

#编译
def MakeProgram(makefileName):
	#(status, result) = commands.getstatusoutput("make -f %s"%makefileName)
	status = os.system("make -f %s"%makefileName)

	if status != 0 :
		return False
	else:
		return True
	
#参数判断
def Jobparamter(serv, job, mgr):
	modelDict = ConstructPara(serv, job, mgr)
	
	#全量编译
	
	#公共库编译
	funPublic()
	
	if modelDict['serv'] != "":   #serv
		funServ(modelDict['serv'])
	if modelDict['job'] != "":    #小程序
		funJob(modelDict['job']) 
	if modelDict['mgr'] != "":    #tm模块
		funMgr(modelDict['mgr'])

	
#小程序编译	
def funJob(modelName):
	homeaddr = GetConf(script_addr +'conf.ini','home','addr')
	originPath = os.getcwd()
	#os.chdir(homeaddr1)
	dictJobNames = jobMapping()   #获取小程序名与工程名的映射字典
	for i in range(len(modelName)):
		try:
			LOGDebug.debug("compile little program %s..."%modelName[i])
			jobProName = ""
			if modelName[i] in dictJobNames.keys():
				jobProName = dictJobNames[modelName[i]]
			else:
				raise ModelNotExistError("This program is not exists,please check!")
			LOGDebug.debug("program-project mapping: %s-%s"%(modelName[i],jobProName))
			list1=[]
			list1.append(homeaddr)
			list1.append("job/")
			list1.append(jobProName)
			string=''.join(list1)
			if os.path.exists(string):
				os.chdir(string)
				#ret = os.system('make -f %s/Makefile' %string)
				makefileName = ('%s/Makefile'%string)
				ret = MakeProgram(makefileName)
				if False == ret:
					raise CompileError("Compile little program failed!!")
				funBinCksumValue(modelName[i],homeaddr)
				os.chdir(homeaddr)
			else :
				LOGError.error("小程序文件不存在，无法编译！!")
				os.chdir(homeaddr)
				continue;
			LOGDebug.debug("compile little program %s sucess!"%modelName[i])
			os.chdir(originPath)
		except ModelNotExistError:
			LOGError.error("This program %s is not exists,please check!" %modelName[i])
			sys.exit(1)
		except CompileError:
			LOGError.error("Compile little program failed!!")
			sys.exit(1)
			

#tm模块编译
def funMgr(modelName):
	homeaddr = GetConf(script_addr +'conf.ini','home','addr')
	originPath = os.getcwd()
	for iLoop in range(len(modelName)):
		try:
			LOGDebug.debug("compile tm program %s..."%modelName[iLoop])
			pathList = []
			pathList.append(homeaddr)
			pathList.append("mgr/")
			pathList.append(modelName[iLoop])
			compilePath=''.join(pathList)
			if os.path.exists(compilePath):
				os.chdir(compilePath)
				#ret = os.system('make -f %s/Makefile' %compilePath)
				makefileName = ('%s/Makefile'%compilePath)
				ret = MakeProgram(makefileName)
				if False == ret:
					raise CompileError("Compile mgr failed!!")
				funBinCksumValue(modelName[iLoop], homeaddr)
				os.chdir(homeaddr)
			else:
				raise ModelNotExistError("%s not exist" %modelName[iLoop])
			LOGDebug.debug("compile tm program %s success!" %modelName[iLoop])
			os.chdir(originPath)
		except ModelNotExistError:
			LOGError.error("Model %s not exist!" %modelName[iLoop])
			sys.exit(1)
		except CompileError:
			LOGError.error("Compile %s model failed!!" %modelName[iLoop])
			sys.exit(1)

		
#由模块名得知依赖的实体类
def GetEntityByServname(servName):
	homeaddr=GetConf(script_addr + 'conf.ini','home','addr')
	originPath = os.getcwd()
	listEntityName = []
	os.chdir(homeaddr)
	EntityNames = GetConf(script_addr + 'Entity.ini',servName,'entity')
	if "" == EntityNames:
		LOGError.error("EntityNames not existed,please check!!")
	else:
		listEntityName = EntityNames.split(" ")
	os.chdir(originPath)
	return listEntityName
	
#编译实体类
def MakeEntityLib(listEntityName):
	codeRootPath = GetConf(script_addr + 'conf.ini','home','addr')
	EntityPath = codeRootPath + "serv/EntityLib/"
	originPath = os.getcwd()
	for iLoop in listEntityName:
		try:
			LOGDebug.debug("compile Entitilib lib %s..."%iLoop)
			compilePath = EntityPath + iLoop
			os.chdir(compilePath)
			if os.path.exists('Makefile'):
				#ret = os.system("make")
				makefileName = ('%s/Makefile'%compilePath)
				ret = MakeProgram(makefileName)
				if False == ret:
					LOGError.error("print Entity %s failed"%iLoop)
					raise CompileError("Compile Entitylib failed!!")
			else:
				raise ModelNotExistError("%s not exist" %iLoop)
			LOGDebug.debug("compile Entitilib lib %s sucess!"%iLoop)
			os.chdir(originPath)
		except ModelNotExistError:
			LOGError.error("Model not exist!")
			sys.exit(1)
		except CompileError:
			LOGError.error("Compile Entitylib failed!!")
			sys.exit(1)
	
			
#模块编译
def funServ(modelName):
#	list=[]
	homeaddr=GetConf(script_addr + 'conf.ini','home','addr')#存放公共路径
	originPath = os.getcwd()
	
	for i in range(len(modelName)):
		try:
			LOGDebug.debug("compile %s model"%modelName[i])
			listEntity = GetEntityByServname(modelName[i])
			LOGDebug.debug("compile entitylib for %s"%modelName[i])
			MakeEntityLib(listEntity)
			list=[]
			list.append(homeaddr)
			list.append('serv/')
			if modelName[i]!='procstep':
				list.append('S_')
				
			list.append(modelName[i])
			string=''.join(list)
			if os.path.exists(string):		
				os.chdir(string)   #改变路劲执行makefile
				#ret = os.system('make -f %s/Makefile' %string)#讨论
				makefileName = ('%s/Makefile'%string)
				ret = MakeProgram(makefileName)
				if False == ret:
					raise CompileError("Compile serv model failed!!")
				funCksumvalue(modelName[i],homeaddr)
			else:
				raise ModelNotExistError("%s not exist" %modelName[i])
			LOGDebug.debug("compile %s model sucess!"%modelName[i])
			os.chdir(homeaddr)
		except ModelNotExistError:
			LOGError.error("Model not exist!")
			sys.exit(1)
		except CompileError:
			LOGError.error("Compile serv model failed!!")
			sys.exit(1)
			os.chdir(originPath)
	
			
			
#公共库编译
def funPublic():
	codePath=GetConf(script_addr + 'conf.ini','home','addr')#存放公共路径
	originPath = os.getcwd()
	try:
		LOGDebug.debug("compile public lib...")
		publicPath = codePath 
		os.chdir(publicPath)
		cmd = "sh " + script_addr + "make_public.sh"
		ret = os.system(cmd)
		if ret != 0:
			raise CompileError("Compile public failed!!")
		LOGDebug.debug("compile public lib success")
		os.chdir(originPath)
	except CompileError:
		LOGError.error("Compile Public model failed!!")
		sys.exit(1)

def checkReleaseError(err):
	if err != "":
		LOGError.error("release file failed!err: %s "%err)
		sys.exit(1)
		
def GetFileReleasePara(fileName, envName):
	user = ""
	password = ""
	remotePath = ""
	host = GetConf(script_addr + 'conf.ini',envName,'ip')
	if fileName.find("CUM") >= 0:
		user = GetConf(script_addr + 'conf.ini',envName,'usernameBis')
		password = GetConf(script_addr + 'conf.ini',envName,'pwBis')
		remotePath=GetConf(script_addr + 'conf.ini',envName,'releasePathBis')
	elif fileName.find("EBK") >= 0:
		user = GetConf(script_addr + 'conf.ini',envName,'usernameEbk')
		password = GetConf(script_addr + 'conf.ini',envName,'pwEbk')
		remotePath=GetConf(script_addr + 'conf.ini',envName,'releasePathEbk')
	else:
		user = GetConf(script_addr + 'conf.ini',envName,'usernameEas')
		password = GetConf(script_addr + 'conf.ini',envName,'pwEas')
		remotePath=GetConf(script_addr + 'conf.ini',envName,'releasePathEas')
	return (host, user, password, remotePath)


def ResetKernelSource(releasePath, host, user, password, soFlg, fileName):
	pxsshObj = pxssh.pxssh()
	# login
	pxsshObj.login (host, user, password)
	if 1 == soFlg:		
		cmd="cd %s;sh %stm_restart.sh;y" %(releasePath, releasePath)
		LOGDebug.debug("execute cmd:%s"%cmd)
		pxsshObj.sendline("cd %s"%releasePath)
		pxsshObj.sendline("sh %stm_restart.sh"%releasePath)
		pxsshObj.sendline("y")
	else:
		if fileName.find("tm") < 0:  #小程序
			cmd = "cd %s; python %stm_program.py %s restart" %(releasePath,releasePath,fileName)
			LOGDebug.debug("execute cmd:%s"%cmd)
			pxsshObj.sendline("cd %s"%releasePath)
			pxsshObj.sendline("python %stm_program.py %s restart"%(releasePath,fileName))
		else:  #tm程序的发布
			cmd = "cd %s; sh tm_restart.sh;" %releasePath
			LOGDebug.debug("execute cmd:%s"%cmd)
			pxsshObj.sendline("cd %s"%releasePath)
			pxsshObj.sendline("sh %stm_restart.sh"%releasePath)
			#pxsshObj.sendline("y")
	pxsshObj.logout()
	
	
		
#发布文件		
def releaseFile(envName, localPath, fileName):
	(host, user, password, remotePath) = GetFileReleasePara(fileName, envName)
	originPath = os.getcwd()	
	LOGDebug.debug("copy %s to remote ..."%fileName)
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	try:
		ssh.connect(host, 22, user, password, timeout=20)
		LOGDebug.debug("%s\t%s\t%s\n" % (host, user, password))
		t = paramiko.Transport((host,22))
		#建立连接
		t.connect(username=user,password=password)
		#建立sftp进行文件传输
		sftp = paramiko.SFTPClient.from_transport(t)
		
		if fileName[len(fileName)-3:len(fileName)]=='.so':
			soFlg = 1 #判断是否库文件
			releasePath=remotePath+'lib/'
		else:
			soFlg = 0
			releasePath=remotePath+'bin/'
			
		#删除已有备份
		stdin,stdout,stderr=ssh.exec_command('cd %s;rm -rf %s.%s' %(releasePath,fileName,'*'))
		checkReleaseError(stderr.read())
		
		now = datetime.datetime.now().strftime("%Y%m%d%H%M")
		#备份原有文件
		stdin,stdout,stderr=ssh.exec_command('cd %s;tar zcvf %s.tar.gz%s %s' %(releasePath,fileName,now,fileName))
		checkReleaseError(stderr.read())
		
		progNameBak = releasePath + fileName + now
		#发布文件
		sftp.put(localPath+fileName,progNameBak)
		stdin,stdout,stderr=ssh.exec_command('cd %s;mv %s %s' %(releasePath,progNameBak,fileName))
		checkReleaseError(stderr.read())
		
		#修改权限
		stdin,stdout,stderr=ssh.exec_command('cd %s; chmod 755 %s'%(releasePath,fileName))
		checkReleaseError(stderr.read())
		
		#获取本地Server应用cksum值
		(status,result)=commands.getstatusoutput('cd %s;cksum %s' %(localPath,fileName))
		local_cksum = result.split(' ')
		#获取远程cksum
		stdin,stdout,stderr=ssh.exec_command("cd %s;cksum %s" %(releasePath,fileName))
		checkReleaseError(stderr.read())
		remote_cksum = stdout.read().strip('\n').split(' ')
		LOGDebug.debug("cksum value:%s"%remote_cksum)
		
		if remote_cksum==local_cksum:
			LOGDebug.debug("release %s ..."%fileName)
			#cksum正确，重启进程
			ResetKernelSource(remotePath + 'bin/', host, user, password, soFlg, fileName)
			time.sleep(10)
			LOGDebug.debug("release %s success!"%fileName)
		else:
			LOGError.error("chsum is incorrect!")
		t.close()
		LOGDebug.debug("realese %s success!"%fileName)
		os.chdir(originPath)
	except Exception,e:
		LOGError.error("%s@%s--%s:%s" % (user,host,e.__class__, e))
		sys.exit(1)

		
#输出模块cksum 值：
def funCksumvalue(str,publicaddr):
	localPath=publicaddr+'lib/'
	if str=='procstep':
		soName="libprocstep.so"
	else:
		soName='S_'+str+'.so'
	os.chdir(publicaddr)
	envNames = ConstructEnvPara(sys.argv[4])
	for envName in envNames:
		LOGDebug.debug("In %s environment, release %s" %(envName, soName))
		releaseFile(envName, localPath, soName)

#输出bin目录的cksum值
def funBinCksumValue(str,publicaddr):
	localPath=publicaddr+'bin/'
	os.chdir(publicaddr)
	envNames = ConstructEnvPara(sys.argv[4])
	for envName in envNames:
		LOGDebug.debug("In %s environment, release %s" %(envName, str))
		releaseFile(envName, localPath, str)

#打印帮助信息
def PrintHelpInfo():
	print "##########Please run AutoCompile.py like the way below:##########"
	print "python AutoCompile.py 'serv lib names' 'little program names' 'tm program names' 'release environments'"
	print "where \",\" will be the spliter between models,note that '' is needed."
	print "for example, you can run this script like this:"
	print "python AutoCompile.py 'SCS,EBK' 'AutoTransfer,AutoPayReward' 'tmlisten,tmservics' 'develop'"
	print "#################################################################"
	
	
def main(argv):
	if len(sys.argv)==5:
		Jobparamter(argv[1],argv[2],argv[3])
		
	else:
		PrintHelpInfo()

if __name__=='__main__':
        main(sys.argv)