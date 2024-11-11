from distutils.log import error
import taos
import sys
import time
import os
import platform

from util.log import *
from util.sql import *
from util.cases import *
from util.dnodes import *
import subprocess

class TDTestCase:

    def init(self, conn, logSql, replicaVar=1):
        self.replicaVar = int(replicaVar)
        self.funcNum = 100
        tdLog.debug(f"start to excute {__file__}")
        tdSql.init(conn.cursor(), logSql)

    def create_udf_function(self):
        for i in range(self.funcNum ):
            tdSql.execute(f"create or replace aggregate function ma_{i} as '/home/taos/udf/ma1.py' outputtype double bufsize 512 language 'Python';")

        functions = tdSql.getResult("show functions")
        function_nums = len(functions)
        if function_nums == self.funcNum :
            tdLog.info("create 100 udf functions success ")
            
        tdSql.execute(f"create database db;")
        tdSql.execute(f"create table db.t1(ts timestamp, c1 int, c2 float);")
        tdSql.execute(f"create table db.t2(ts timestamp, c1 double);")
        tdSql.execute(f"insert into db.t2 values(now, 1.1)")
        for i in range(200):
            tdSql.execute(f"insert into db.t1 values(now, {i}, {i*1.1})")
            
        for i in range(self.funcNum ):
            tdSql.execute(f"select tbname, ma_{i}(c1) from db.t1")

        sql = "select tbname"
        for i in range(self.funcNum ):
            sql += f", ma_{i}(c1)"
        sql += " from db.t1"
        tdSql.execute(sql)
        

    def clearFunctions(self):
        for i in range(self.funcNum ):
            tdSql.execute(f"drop function ma_{i};")

    def run(self):
        #self.clearFunctions()
        print(" env is ok for all ")
        self.create_udf_function()




    def stop(self):
        tdSql.close()
        tdLog.success(f"{__file__} successfully executed")

tdCases.addLinux(__file__, TDTestCase())
tdCases.addWindows(__file__, TDTestCase())
