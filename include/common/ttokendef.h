/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_COMMON_TOKEN_H_
#define _TD_COMMON_TOKEN_H_

#define TK_OR                   1
#define TK_AND                  2
#define TK_UNION                3
#define TK_ALL                  4
#define TK_MINUS                5
#define TK_EXCEPT               6
#define TK_INTERSECT            7
#define TK_NK_BITAND            8
#define TK_NK_BITOR             9
#define TK_NK_LSHIFT            10
#define TK_NK_RSHIFT            11
#define TK_NK_PLUS              12
#define TK_NK_MINUS             13
#define TK_NK_STAR              14
#define TK_NK_SLASH             15
#define TK_NK_REM               16
#define TK_NK_CONCAT            17
#define TK_CREATE               18
#define TK_ACCOUNT              19
#define TK_NK_ID                20
#define TK_PASS                 21
#define TK_NK_STRING            22
#define TK_ALTER                23
#define TK_PPS                  24
#define TK_TSERIES              25
#define TK_STORAGE              26
#define TK_STREAMS              27
#define TK_QTIME                28
#define TK_DBS                  29
#define TK_USERS                30
#define TK_CONNS                31
#define TK_STATE                32
#define TK_NK_COMMA             33
#define TK_HOST                 34
#define TK_USER                 35
#define TK_ENABLE               36
#define TK_NK_INTEGER           37
#define TK_SYSINFO              38
#define TK_ADD                  39
#define TK_DROP                 40
#define TK_GRANT                41
#define TK_ON                   42
#define TK_TO                   43
#define TK_REVOKE               44
#define TK_FROM                 45
#define TK_SUBSCRIBE            46
#define TK_READ                 47
#define TK_WRITE                48
#define TK_NK_DOT               49
#define TK_WITH                 50
#define TK_DNODE                51
#define TK_PORT                 52
#define TK_DNODES               53
#define TK_RESTORE              54
#define TK_NK_IPTOKEN           55
#define TK_FORCE                56
#define TK_UNSAFE               57
#define TK_LOCAL                58
#define TK_QNODE                59
#define TK_BNODE                60
#define TK_SNODE                61
#define TK_MNODE                62
#define TK_VNODE                63
#define TK_DATABASE             64
#define TK_USE                  65
#define TK_FLUSH                66
#define TK_TRIM                 67
#define TK_COMPACT              68
#define TK_IF                   69
#define TK_NOT                  70
#define TK_EXISTS               71
#define TK_BUFFER               72
#define TK_CACHEMODEL           73
#define TK_CACHESIZE            74
#define TK_COMP                 75
#define TK_DURATION             76
#define TK_NK_VARIABLE          77
#define TK_MAXROWS              78
#define TK_MINROWS              79
#define TK_KEEP                 80
#define TK_PAGES                81
#define TK_PAGESIZE             82
#define TK_TSDB_PAGESIZE        83
#define TK_PRECISION            84
#define TK_REPLICA              85
#define TK_VGROUPS              86
#define TK_SINGLE_STABLE        87
#define TK_RETENTIONS           88
#define TK_SCHEMALESS           89
#define TK_WAL_LEVEL            90
#define TK_WAL_FSYNC_PERIOD     91
#define TK_WAL_RETENTION_PERIOD 92
#define TK_WAL_RETENTION_SIZE   93
#define TK_WAL_ROLL_PERIOD      94
#define TK_WAL_SEGMENT_SIZE     95
#define TK_STT_TRIGGER          96
#define TK_TABLE_PREFIX         97
#define TK_TABLE_SUFFIX         98
#define TK_NK_COLON             99
#define TK_BWLIMIT              100
#define TK_START                101
#define TK_TIMESTAMP            102
#define TK_END                  103
#define TK_TABLE                104
#define TK_NK_LP                105
#define TK_NK_RP                106
#define TK_STABLE               107
#define TK_COLUMN               108
#define TK_MODIFY               109
#define TK_RENAME               110
#define TK_TAG                  111
#define TK_SET                  112
#define TK_NK_EQ                113
#define TK_USING                114
#define TK_TAGS                 115
#define TK_BOOL                 116
#define TK_TINYINT              117
#define TK_SMALLINT             118
#define TK_INT                  119
#define TK_INTEGER              120
#define TK_BIGINT               121
#define TK_FLOAT                122
#define TK_DOUBLE               123
#define TK_BINARY               124
#define TK_NCHAR                125
#define TK_UNSIGNED             126
#define TK_JSON                 127
#define TK_VARCHAR              128
#define TK_MEDIUMBLOB           129
#define TK_BLOB                 130
#define TK_VARBINARY            131
#define TK_GEOMETRY             132
#define TK_DECIMAL              133
#define TK_COMMENT              134
#define TK_MAX_DELAY            135
#define TK_WATERMARK            136
#define TK_ROLLUP               137
#define TK_TTL                  138
#define TK_SMA                  139
#define TK_DELETE_MARK          140
#define TK_FIRST                141
#define TK_LAST                 142
#define TK_SHOW                 143
#define TK_PRIVILEGES           144
#define TK_DATABASES            145
#define TK_TABLES               146
#define TK_STABLES              147
#define TK_MNODES               148
#define TK_QNODES               149
#define TK_FUNCTIONS            150
#define TK_INDEXES              151
#define TK_ACCOUNTS             152
#define TK_APPS                 153
#define TK_CONNECTIONS          154
#define TK_LICENCES             155
#define TK_GRANTS               156
#define TK_QUERIES              157
#define TK_SCORES               158
#define TK_TOPICS               159
#define TK_VARIABLES            160
#define TK_CLUSTER              161
#define TK_BNODES               162
#define TK_SNODES               163
#define TK_TRANSACTIONS         164
#define TK_DISTRIBUTED          165
#define TK_CONSUMERS            166
#define TK_SUBSCRIPTIONS        167
#define TK_VNODES               168
#define TK_ALIVE                169
#define TK_COMPACTS             170
#define TK_LIKE                 171
#define TK_TBNAME               172
#define TK_QTAGS                173
#define TK_AS                   174
#define TK_INDEX                175
#define TK_FUNCTION             176
#define TK_INTERVAL             177
#define TK_COUNT                178
#define TK_LAST_ROW             179
#define TK_META                 180
#define TK_ONLY                 181
#define TK_TOPIC                182
#define TK_CONSUMER             183
#define TK_GROUP                184
#define TK_DESC                 185
#define TK_DESCRIBE             186
#define TK_RESET                187
#define TK_QUERY                188
#define TK_CACHE                189
#define TK_EXPLAIN              190
#define TK_ANALYZE              191
#define TK_VERBOSE              192
#define TK_NK_BOOL              193
#define TK_RATIO                194
#define TK_NK_FLOAT             195
#define TK_OUTPUTTYPE           196
#define TK_AGGREGATE            197
#define TK_BUFSIZE              198
#define TK_LANGUAGE             199
#define TK_REPLACE              200
#define TK_STREAM               201
#define TK_INTO                 202
#define TK_PAUSE                203
#define TK_RESUME               204
#define TK_TRIGGER              205
#define TK_AT_ONCE              206
#define TK_WINDOW_CLOSE         207
#define TK_IGNORE               208
#define TK_EXPIRED              209
#define TK_FILL_HISTORY         210
#define TK_UPDATE               211
#define TK_SUBTABLE             212
#define TK_UNTREATED            213
#define TK_KILL                 214
#define TK_CONNECTION           215
#define TK_TRANSACTION          216
#define TK_BALANCE              217
#define TK_VGROUP               218
#define TK_LEADER               219
#define TK_MERGE                220
#define TK_REDISTRIBUTE         221
#define TK_SPLIT                222
#define TK_DELETE               223
#define TK_INSERT               224
#define TK_NK_BIN               225
#define TK_NK_HEX               226
#define TK_NULL                 227
#define TK_NK_QUESTION          228
#define TK_NK_ARROW             229
#define TK_ROWTS                230
#define TK_QSTART               231
#define TK_QEND                 232
#define TK_QDURATION            233
#define TK_WSTART               234
#define TK_WEND                 235
#define TK_WDURATION            236
#define TK_IROWTS               237
#define TK_ISFILLED             238
#define TK_CAST                 239
#define TK_NOW                  240
#define TK_TODAY                241
#define TK_TIMEZONE             242
#define TK_CLIENT_VERSION       243
#define TK_SERVER_VERSION       244
#define TK_SERVER_STATUS        245
#define TK_CURRENT_USER         246
#define TK_CASE                 247
#define TK_WHEN                 248
#define TK_THEN                 249
#define TK_ELSE                 250
#define TK_BETWEEN              251
#define TK_IS                   252
#define TK_NK_LT                253
#define TK_NK_GT                254
#define TK_NK_LE                255
#define TK_NK_GE                256
#define TK_NK_NE                257
#define TK_MATCH                258
#define TK_NMATCH               259
#define TK_CONTAINS             260
#define TK_IN                   261
#define TK_JOIN                 262
#define TK_INNER                263
#define TK_SELECT               264
#define TK_NK_HINT              265
#define TK_DISTINCT             266
#define TK_WHERE                267
#define TK_PARTITION            268
#define TK_BY                   269
#define TK_SESSION              270
#define TK_STATE_WINDOW         271
#define TK_EVENT_WINDOW         272
#define TK_SLIDING              273
#define TK_FILL                 274
#define TK_VALUE                275
#define TK_VALUE_F              276
#define TK_NONE                 277
#define TK_PREV                 278
#define TK_NULL_F               279
#define TK_LINEAR               280
#define TK_NEXT                 281
#define TK_HAVING               282
#define TK_RANGE                283
#define TK_EVERY                284
#define TK_ORDER                285
#define TK_SLIMIT               286
#define TK_SOFFSET              287
#define TK_LIMIT                288
#define TK_OFFSET               289
#define TK_ASC                  290
#define TK_NULLS                291
#define TK_ABORT                292
#define TK_AFTER                293
#define TK_ATTACH               294
#define TK_BEFORE               295
#define TK_BEGIN                296
#define TK_BITAND               297
#define TK_BITNOT               298
#define TK_BITOR                299
#define TK_BLOCKS               300
#define TK_CHANGE               301
#define TK_COMMA                302
#define TK_CONCAT               303
#define TK_CONFLICT             304
#define TK_COPY                 305
#define TK_DEFERRED             306
#define TK_DELIMITERS           307
#define TK_DETACH               308
#define TK_DIVIDE               309
#define TK_DOT                  310
#define TK_EACH                 311
#define TK_FAIL                 312
#define TK_FILE                 313
#define TK_FOR                  314
#define TK_GLOB                 315
#define TK_ID                   316
#define TK_IMMEDIATE            317
#define TK_IMPORT               318
#define TK_INITIALLY            319
#define TK_INSTEAD              320
#define TK_ISNULL               321
#define TK_KEY                  322
#define TK_MODULES              323
#define TK_NK_BITNOT            324
#define TK_NK_SEMI              325
#define TK_NOTNULL              326
#define TK_OF                   327
#define TK_PLUS                 328
#define TK_PRIVILEGE            329
#define TK_RAISE                330
#define TK_RESTRICT             331
#define TK_ROW                  332
#define TK_SEMI                 333
#define TK_STAR                 334
#define TK_STATEMENT            335
#define TK_STRICT               336
#define TK_STRING               337
#define TK_TIMES                338
#define TK_VALUES               339
#define TK_VARIABLE             340
#define TK_VIEW                 341
#define TK_WAL                  342

#define TK_NK_SPACE   600
#define TK_NK_COMMENT 601
#define TK_NK_ILLEGAL 602
// #define TK_NK_HEX           603  // hex number  0x123
#define TK_NK_OCT 604  // oct number
// #define TK_NK_BIN           605  // bin format data 0b111
#define TK_BATCH_SCAN       606
#define TK_NO_BATCH_SCAN    607
#define TK_PARA_TABLES_SORT 608

#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
