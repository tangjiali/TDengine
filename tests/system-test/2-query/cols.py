from os import system
from util.log import *
from util.sql import *
from util.cases import *
from util.sqlset import *
class TDTestCase:
    def init(self, conn, logSql, replicaVar=1):
        self.replicaVar = int(replicaVar)
        tdLog.debug("start to execute %s" % __file__)
        tdSql.init(conn.cursor(),True)
        self.setsql = TDSetSql()
        self.rowNum = 10
        self.ts = 1537146000000

        dbname = "db"
        self.ntbname = f'{dbname}.ntb'
        self.stbname = f'{dbname}.stb'
        self.column_dict = {
            'ts':'timestamp',
            'c1':'int',
            'c2':'float',
            'c3':'double',
            'c4':'timestamp'
        }
        self.tag_dict = {
            't0':'int'
        }
        # The number of tag_values should be same as tbnum
        self.tbnum = 2
        self.tag_values = [
            f'10',
            f'100'
        ]
    

    def colstest(self):

        os.system(f'taos -f {sql_file}')
        tdSql.query('select count(c_1) from d2.t2 where c_1 < 10', queryTimes=1)
        tdSql.checkData(0, 0, 0)
        tdSql.query('select count(c_1), min(c_1),tbname from d2.can partition by tbname order by 3', queryTimes=1)
        tdSql.checkData(0, 0, 0)
        tdSql.checkData(0, 1, None)
        tdSql.checkData(0, 2, 't1')

        tdSql.checkData(1, 0, 15)
        tdSql.checkData(1, 1, 1471617148940980000)
        tdSql.checkData(1, 2, 't2')

        tdSql.checkData(2, 0, 0)
        tdSql.checkData(2, 1, None)
        tdSql.checkData(2, 2, 't3')

    def run(self):
        self.colstest()
    def stop(self):
        tdSql.close()
        tdLog.success("%s successfully executed" % __file__)


tdCases.addWindows(__file__, TDTestCase())
tdCases.addLinux(__file__, TDTestCase())
