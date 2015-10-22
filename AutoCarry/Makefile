# Project: bkautopay application
# Makefile.in created by Tang Wang 2013/01/16

CC = g++

#############################################################
HOMEPATH = $(TEDO_HOME)
OBJPATH = $(HOMEPATH)/obj
LIBPATH = $(HOMEPATH)/lib
BINPATH = $(HOMEPATH)/bin
SRCPATH = ./src
INCPATH = $(COMMONCPP2_INCLUDE)	\
          -I./inc	\
          -I$(HOMEPATH)/public/base_component/inc	\
          -I$(HOMEPATH)/public/libclient/inc	\
	 -I$(HOMEPATH)/serv/EntityLib/PUB_ET/include	\
	-I$(HOMEPATH)/serv/EntityLib/EBK_ET/include	\
	-I$(HOMEPATH)/serv/EntityLib/CUM_ET/inlude	\
	-I$(HOMEPATH)/serv/EntityLib/PDM_ET/include	\
	-I$(HOMEPATH)/serv/EntityLib/TMN_ET/include	\
	-I$(HOMEPATH)/serv/EntityLib/SCS_ET/include	\
	-I$(HOMEPATH)/serv/EntityLib/PNM_ET/include	\
	-I$(HOMEPATH)/public/utility/inc	\
	-I$(HOMEPATH)/public/OracleDb/include


include ../../serv/Makefile.serv

LINKOBJ = -L$(LIBPATH) -lbase -lclient

CXXFLAGS = $(COMMONCPP2_COMPILE_FLAG)
APP_COMPILE = $(CC) $(CXXFLAGS)


OBJ  = $(OBJPATH)/AutoDatabase.o	\
	$(OBJPATH)/CarryConfig.o	\
	$(OBJPATH)/CarryDatabase.o	\
	$(OBJPATH)/OpCarryOperator.o

#############################################################
.PHONY: all all-before all-after clean clean-custom

all: AutoDatabase.o CarryDatabase.o CarryConfig.o OpCarryOperator.o AutoWithdrawal

clean: clean-custom
	 ${RM} $(OBJ) $(BINPATH)/AutoWithdrawal

AutoDatabase.o:
	$(CC) -o $(OBJPATH)/AutoDatabase.o -c $(SRCPATH)/AutoDatabase.cpp	\
	$(INCPATH)	\
	$(LINKOBJ)	\
	$(DB_INCLUDE)
         
CarryDatabase.o:								
	 $(CC) -o $(OBJPATH)/CarryDatabase.o -c $(SRCPATH)/CarryDatabase.cpp	\
	 $(INCPATH)	\
	$(LINKOBJ)	\
	$(DB_INCLUDE)
         
CarryConfig.o:
	$(CC) -o $(OBJPATH)/CarryConfig.o -c $(SRCPATH)/CarryConfig.cpp	\
	$(INCPATH)	\
	$(LINKOBJ)	\
	$(DB_INCLUDE)

OpCarryOperator.o:
	$(CC) -o $(OBJPATH)/OpCarryOperator.o -c $(SRCPATH)/OpCarryOperator.cpp   \
	$(INCPATH)	\
	$(LINKOBJ)	\
	$(DB_INCLUDE)

AutoWithdrawal:
	$(APP_COMPILE) $(SRCPATH)/AutoCarry.cpp -o $(BINPATH)/AutoWithdrawal	\
	$(COMMONCPP2_INCLUDE)	\
	$(COMMONCPP2_LINK)	\
	$(INCPATH)	\
	$(DB_INCLUDE)	\
	$(LINKOBJ)	\
	$(DBLINK)	\
	$(OBJPATH)/pub_et.a	\
	$(OBJPATH)/scs_et.a	\
	$(OBJPATH)/cum_et.a	\
	$(OBJPATH)/ebk_et.a	\
	$(OBJPATH)/pdm_et.a	\
	$(OBJPATH)/et_base.o	\
	$(OBJ) 


