python 标准模块 logging
----liys
20150827

1.logging提供以下几个子模块：loggers,handlers ,filters , formatters.loggers  把应用需要直接调用的接口暴露出来
	a.Handlers把log记录发到相应的目的地
	b.Filters决定哪些记录需要发给handler
	c.Formatters定义了log记录的输出格式.
	
2.loggers 暴露给应用几个方法以便应用可以在运行时写log ,
		  Logger对象按照log信息的严重程度或者根据filter对象来决定如何处理log信息(默认的过滤功能).
		  logger还负责把log信息传送给相关的loghandlers
		  
	loggers最常用的方法分两部分：configuration and message sending.
		configuration方法：
			setLevel(level) :定义一个logger处理的级别：debug->info->warning->error->critical
			
			ebug(log_message, [*args[, **kwargs]])
			info(log_message, [*args[, **kwargs]])
			warning(log_message, [*args[, **kwargs]])
			error(log_message, [*args[, **kwargs]])
			critical(log_message, [*args[, **kwargs]])
			exception(message[, *args]) //方法跟error基本一样，exception()方法只能从一个exeption handlers里面调用
			log(log_level, log_message, [*args[, **kwargs]])
	Logging.getLogger([name]) 返回一个logger实例的引用，有一个名字是foo的logger,
	那么诸如foo.bar,foo.bar.baz和foo.bam这样的logger都是foo这个logger的子,
	子loggers自动继承父loggers的log信息,正因为此,没有必要把一个应用的所有logger都配置一边,
	只要把顶层的logger配置好了,然后子logger根据需要继承就行了.
	
3.Handlers
	
	Handler对象负责分配合适的log信息(基于log信息的严重程度)到handler指定的目的地.
	Logger对象可以用addHandler()方法添加零个或多个handler对象到它自身.
	一个常见的场景是,一个应用可能希望把所有的log信息都发送到一个log文件中去,所有的error级别以上的log信息都发送到stdout,所有critical的log信息通过email发送.
	这个场景里要求三个不同handler处理,每个handler负责把特定的log信息发送到特定的地方
	
	
	