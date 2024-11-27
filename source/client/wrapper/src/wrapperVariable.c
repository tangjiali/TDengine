/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY = NULL; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "wrapper.h"

// from taos.h
void (*fp_taos_cleanup)(void) = NULL;
int (*fp_taos_options)(TSDB_OPTION option, const void *arg, ...) = NULL;
setConfRet (*fp_taos_set_config)(const char *config) = NULL;
int (*fp_taos_init)(void) = NULL;
TAOS *(*fp_taos_connect)(const char *ip, const char *user, const char *pass, const char *db, uint16_t port) = NULL;
TAOS *(*fp_taos_connect_auth)(const char *ip, const char *user, const char *auth, const char *db, uint16_t port) = NULL;
void (*fp_taos_close)(TAOS *taos) = NULL;
const char *(*fp_taos_data_type)(int type) = NULL;
TAOS_STMT *(*fp_taos_stmt_init)(TAOS *taos) = NULL;
TAOS_STMT *(*fp_taos_stmt_init_with_reqid)(TAOS *taos, int64_t reqid) = NULL;
TAOS_STMT *(*fp_taos_stmt_init_with_options)(TAOS *taos, TAOS_STMT_OPTIONS *options) = NULL;
int (*fp_taos_stmt_prepare)(TAOS_STMT *stmt, const char *sql, unsigned long length) = NULL;
int (*fp_taos_stmt_set_tbname_tags)(TAOS_STMT *stmt, const char *name, TAOS_MULTI_BIND *tags) = NULL;
int (*fp_taos_stmt_set_tbname)(TAOS_STMT *stmt, const char *name) = NULL;
int (*fp_taos_stmt_set_tags)(TAOS_STMT *stmt, TAOS_MULTI_BIND *tags) = NULL;
int (*fp_taos_stmt_set_sub_tbname)(TAOS_STMT *stmt, const char *name) = NULL;
int (*fp_taos_stmt_get_tag_fields)(TAOS_STMT *stmt, int *fieldNum, TAOS_FIELD_E **fields) = NULL;
int (*fp_taos_stmt_get_col_fields)(TAOS_STMT *stmt, int *fieldNum, TAOS_FIELD_E **fields) = NULL;
void (*fp_taos_stmt_reclaim_fields)(TAOS_STMT *stmt, TAOS_FIELD_E *fields) = NULL;
int (*fp_taos_stmt_is_insert)(TAOS_STMT *stmt, int *insert) = NULL;
int (*fp_taos_stmt_num_params)(TAOS_STMT *stmt, int *nums) = NULL;
int (*fp_taos_stmt_get_param)(TAOS_STMT *stmt, int idx, int *type, int *bytes) = NULL;
int (*fp_taos_stmt_bind_param)(TAOS_STMT *stmt, TAOS_MULTI_BIND *bind) = NULL;
int (*fp_taos_stmt_bind_param_batch)(TAOS_STMT *stmt, TAOS_MULTI_BIND *bind) = NULL;
int (*fp_taos_stmt_bind_single_param_batch)(TAOS_STMT *stmt, TAOS_MULTI_BIND *bind, int colIdx) = NULL;
int (*fp_taos_stmt_add_batch)(TAOS_STMT *stmt) = NULL;
int (*fp_taos_stmt_execute)(TAOS_STMT *stmt) = NULL;
TAOS_RES *(*fp_taos_stmt_use_result)(TAOS_STMT *stmt) = NULL;
int (*fp_taos_stmt_close)(TAOS_STMT *stmt) = NULL;
char *(*fp_taos_stmt_errstr)(TAOS_STMT *stmt) = NULL;
int (*fp_taos_stmt_affected_rows)(TAOS_STMT *stmt) = NULL;
int (*fp_taos_stmt_affected_rows_once)(TAOS_STMT *stmt) = NULL;
TAOS_STMT2 *(*fp_taos_stmt2_init)(TAOS *taos, TAOS_STMT2_OPTION *option) = NULL;
int (*fp_taos_stmt2_prepare)(TAOS_STMT2 *stmt, const char *sql, unsigned long length) = NULL;
int (*fp_taos_stmt2_bind_param)(TAOS_STMT2 *stmt, TAOS_STMT2_BINDV *bindv, int32_t col_idx) = NULL;
int (*fp_taos_stmt2_exec)(TAOS_STMT2 *stmt, int *affected_rows) = NULL;
int (*fp_taos_stmt2_close)(TAOS_STMT2 *stmt) = NULL;
int (*fp_taos_stmt2_is_insert)(TAOS_STMT2 *stmt, int *insert) = NULL;
int (*fp_taos_stmt2_get_fields)(TAOS_STMT2 *stmt, TAOS_FIELD_T field_type, int *count, TAOS_FIELD_E **fields) = NULL;
int (*fp_taos_stmt2_get_stb_fields)(TAOS_STMT2 *stmt, int *count, TAOS_FIELD_STB **fields) = NULL;
void (*fp_taos_stmt2_free_fields)(TAOS_STMT2 *stmt, TAOS_FIELD_E *fields) = NULL;
void (*fp_taos_stmt2_free_stb_fields)(TAOS_STMT2 *stmt, TAOS_FIELD_STB *fields) = NULL;
TAOS_RES *(*fp_taos_stmt2_result)(TAOS_STMT2 *stmt) = NULL;
char *(*fp_taos_stmt2_error)(TAOS_STMT2 *stmt) = NULL;
TAOS_RES *(*fp_taos_query)(TAOS *taos, const char *sql) = NULL;
TAOS_RES *(*fp_taos_query_with_reqid)(TAOS *taos, const char *sql, int64_t reqId) = NULL;
TAOS_ROW (*fp_taos_fetch_row)(TAOS_RES *res) = NULL;
int (*fp_taos_result_precision)(TAOS_RES *res) = NULL;  // get the time precision of result
void (*fp_taos_free_result)(TAOS_RES *res) = NULL;
void (*fp_taos_kill_query)(TAOS *taos) = NULL;
int (*fp_taos_field_count)(TAOS_RES *res) = NULL;
int (*fp_taos_num_fields)(TAOS_RES *res) = NULL;
int (*fp_taos_affected_rows)(TAOS_RES *res) = NULL;
int64_t (*fp_taos_affected_rows64)(TAOS_RES *res) = NULL;
TAOS_FIELD *(*fp_taos_fetch_fields)(TAOS_RES *res) = NULL;
int (*fp_taos_select_db)(TAOS *taos, const char *db) = NULL;
int (*fp_taos_print_row)(char *str, TAOS_ROW row, TAOS_FIELD *fields, int num_fields) = NULL;
int (*fp_taos_print_row_with_size)(char *str, uint32_t size, TAOS_ROW row, TAOS_FIELD *fields, int num_fields) = NULL;
void (*fp_taos_stop_query)(TAOS_RES *res) = NULL;
bool (*fp_taos_is_null)(TAOS_RES *res, int32_t row, int32_t col) = NULL;
int (*fp_taos_is_null_by_column)(TAOS_RES *res, int columnIndex, bool result[], int *rows) = NULL;
bool (*fp_taos_is_update_query)(TAOS_RES *res) = NULL;
int (*fp_taos_fetch_block)(TAOS_RES *res, TAOS_ROW *rows) = NULL;
int (*fp_taos_fetch_block_s)(TAOS_RES *res, int *numOfRows, TAOS_ROW *rows) = NULL;
int (*fp_taos_fetch_raw_block)(TAOS_RES *res, int *numOfRows, void **pData) = NULL;
int *(*fp_taos_get_column_data_offset)(TAOS_RES *res, int columnIndex) = NULL;
int (*fp_taos_validate_sql)(TAOS *taos, const char *sql) = NULL;
void (*fp_taos_reset_current_db)(TAOS *taos) = NULL;
int *(*fp_taos_fetch_lengths)(TAOS_RES *res) = NULL;
TAOS_ROW *(*fp_taos_result_block)(TAOS_RES *res) = NULL;
const char *(*fp_taos_get_server_info)(TAOS *taos) = NULL;
const char *(*fp_taos_get_client_info)() = NULL;
int (*fp_taos_get_current_db)(TAOS *taos, char *database, int len, int *required) = NULL;
const char *(*fp_taos_errstr)(TAOS_RES *res) = NULL;
int (*fp_taos_errno)(TAOS_RES *res) = NULL;
void (*fp_taos_query_a)(TAOS *taos, const char *sql, __taos_async_fn_t fp, void *param) = NULL;
void (*fp_taos_query_a_with_reqid)(TAOS *taos, const char *sql, __taos_async_fn_t fp, void *param,
                                   int64_t reqid) = NULL;
void (*fp_taos_fetch_rows_a)(TAOS_RES *res, __taos_async_fn_t fp, void *param) = NULL;
void (*fp_taos_fetch_raw_block_a)(TAOS_RES *res, __taos_async_fn_t fp, void *param) = NULL;
const void *(*fp_taos_get_raw_block)(TAOS_RES *res) = NULL;
int (*fp_taos_get_db_route_info)(TAOS *taos, const char *db, TAOS_DB_ROUTE_INFO *dbInfo) = NULL;
int (*fp_taos_get_table_vgId)(TAOS *taos, const char *db, const char *table, int *vgId) = NULL;
int (*fp_taos_get_tables_vgId)(TAOS *taos, const char *db, const char *table[], int tableNum, int *vgId) = NULL;
int (*fp_taos_load_table_info)(TAOS *taos, const char *tableNameList) = NULL;
void (*fp_taos_set_hb_quit)(int8_t quitByKill) = NULL;
int (*fp_taos_set_notify_cb)(TAOS *taos, __taos_notify_fn_t fp, void *param, int type) = NULL;
void (*fp_taos_fetch_whitelist_a)(TAOS *taos, __taos_async_whitelist_fn_t fp, void *param) = NULL;
int (*fp_taos_set_conn_mode)(TAOS *taos, int mode, int value) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert)(TAOS *taos, char *lines[], int numLines, int protocol, int precision) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_with_reqid)(TAOS *taos, char *lines[], int numLines, int protocol, int precision,
                                                  int64_t reqid) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_raw)(TAOS *taos, char *lines, int len, int32_t *totalRows, int protocol,
                                           int precision) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_raw_with_reqid)(TAOS *taos, char *lines, int len, int32_t *totalRows,
                                                      int protocol, int precision, int64_t reqid) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_ttl)(TAOS *taos, char *lines[], int numLines, int protocol, int precision,
                                           int32_t ttl) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_ttl_with_reqid)(TAOS *taos, char *lines[], int numLines, int protocol,
                                                      int precision, int32_t ttl, int64_t reqid) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_raw_ttl)(TAOS *taos, char *lines, int len, int32_t *totalRows, int protocol,
                                               int precision, int32_t ttl) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_raw_ttl_with_reqid)(TAOS *taos, char *lines, int len, int32_t *totalRows,
                                                          int protocol, int precision, int32_t ttl,
                                                          int64_t reqid) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_raw_ttl_with_reqid_tbname_key)(TAOS *taos, char *lines, int len,
                                                                     int32_t *totalRows, int protocol, int precision,
                                                                     int32_t ttl, int64_t reqid,
                                                                     char *tbnameKey) = NULL;
TAOS_RES *(*fp_taos_schemaless_insert_ttl_with_reqid_tbname_key)(TAOS *taos, char *lines[], int numLines, int protocol,
                                                                 int precision, int32_t ttl, int64_t reqid,
                                                                 char *tbnameKey) = NULL;
tmq_conf_t *(*fp_tmq_conf_new)() = NULL;
tmq_conf_res_t (*fp_tmq_conf_set)(tmq_conf_t *conf, const char *key, const char *value) = NULL;
void (*fp_tmq_conf_destroy)(tmq_conf_t *conf) = NULL;
void (*fp_tmq_conf_set_auto_commit_cb)(tmq_conf_t *conf, tmq_commit_cb *cb, void *param) = NULL;
tmq_list_t *(*fp_tmq_list_new)() = NULL;
int32_t (*fp_tmq_list_append)(tmq_list_t *, const char *) = NULL;
void (*fp_tmq_list_destroy)(tmq_list_t *) = NULL;
int32_t (*fp_tmq_list_get_size)(const tmq_list_t *) = NULL;
char **(*fp_tmq_list_to_c_array)(const tmq_list_t *) = NULL;
tmq_t *(*fp_tmq_consumer_new)(tmq_conf_t *conf, char *errstr, int32_t errstrLen) = NULL;
int32_t (*fp_tmq_subscribe)(tmq_t *tmq, const tmq_list_t *topic_list) = NULL;
int32_t (*fp_tmq_unsubscribe)(tmq_t *tmq) = NULL;
int32_t (*fp_tmq_subscription)(tmq_t *tmq, tmq_list_t **topics) = NULL;
TAOS_RES *(*fp_tmq_consumer_poll)(tmq_t *tmq, int64_t timeout) = NULL;
int32_t (*fp_tmq_consumer_close)(tmq_t *tmq) = NULL;
int32_t (*fp_tmq_commit_sync)(tmq_t *tmq, const TAOS_RES *msg) = NULL;
void (*fp_tmq_commit_async)(tmq_t *tmq, const TAOS_RES *msg, tmq_commit_cb *cb, void *param) = NULL;
int32_t (*fp_tmq_commit_offset_sync)(tmq_t *tmq, const char *pTopicName, int32_t vgId, int64_t offset) = NULL;
void (*fp_tmq_commit_offset_async)(tmq_t *tmq, const char *pTopicName, int32_t vgId, int64_t offset, tmq_commit_cb *cb,
                                   void *param) = NULL;
int32_t (*fp_tmq_get_topic_assignment)(tmq_t *tmq, const char *pTopicName, tmq_topic_assignment **assignment,
                                       int32_t *numOfAssignment) = NULL;
void (*fp_tmq_free_assignment)(tmq_topic_assignment *pAssignment) = NULL;
int32_t (*fp_tmq_offset_seek)(tmq_t *tmq, const char *pTopicName, int32_t vgId, int64_t offset) = NULL;
int64_t (*fp_tmq_position)(tmq_t *tmq, const char *pTopicName, int32_t vgId) = NULL;
int64_t (*fp_tmq_committed)(tmq_t *tmq, const char *pTopicName, int32_t vgId) = NULL;
TAOS *(*fp_tmq_get_connect)(tmq_t *tmq) = NULL;
const char *(*fp_tmq_get_table_name)(TAOS_RES *res) = NULL;
tmq_res_t (*fp_tmq_get_res_type)(TAOS_RES *res) = NULL;
const char *(*fp_tmq_get_topic_name)(TAOS_RES *res) = NULL;
const char *(*fp_tmq_get_db_name)(TAOS_RES *res) = NULL;
int32_t (*fp_tmq_get_vgroup_id)(TAOS_RES *res) = NULL;
int64_t (*fp_tmq_get_vgroup_offset)(TAOS_RES *res) = NULL;
const char *(*fp_tmq_err2str)(int32_t code) = NULL;
int32_t (*fp_tmq_get_raw)(TAOS_RES *res, tmq_raw_data *raw) = NULL;
int32_t (*fp_tmq_write_raw)(TAOS *taos, tmq_raw_data raw) = NULL;
int (*fp_taos_write_raw_block)(TAOS *taos, int numOfRows, char *pData, const char *tbname) = NULL;
int (*fp_taos_write_raw_block_with_reqid)(TAOS *taos, int numOfRows, char *pData, const char *tbname,
                                          int64_t reqid) = NULL;
int (*fp_taos_write_raw_block_with_fields)(TAOS *taos, int rows, char *pData, const char *tbname, TAOS_FIELD *fields,
                                           int numFields) = NULL;
int (*fp_taos_write_raw_block_with_fields_with_reqid)(TAOS *taos, int rows, char *pData, const char *tbname,
                                                      TAOS_FIELD *fields, int numFields, int64_t reqid) = NULL;
void (*fp_tmq_free_raw)(tmq_raw_data raw) = NULL;
char *(*fp_tmq_get_json_meta)(TAOS_RES *res) = NULL;
void (*fp_tmq_free_json_meta)(char *jsonMeta) = NULL;
TSDB_SERVER_STATUS (*fp_taos_check_server_status)(const char *fqdn, int port, char *details, int maxlen) = NULL;
char *(*fp_getBuildInfo)() = NULL;

// from taosws.h
#ifdef WEBSOCKET
int32_t (*fp_ws_enable_log)(const char *log_level) = NULL;
const char *(*fp_ws_data_type)(int32_t type) = NULL;
WS_TAOS *(*fp_ws_connect)(const char *dsn) = NULL;
const char *(*fp_ws_get_server_info)(WS_TAOS *taos) = NULL;
int32_t (*fp_ws_close)(WS_TAOS *taos) = NULL;
WS_RES *(*fp_ws_query)(WS_TAOS *taos, const char *sql) = NULL;
WS_RES *(*fp_ws_query_with_reqid)(WS_TAOS *taos, const char *sql, uint64_t req_id) = NULL;
int32_t (*fp_ws_stop_query)(WS_RES *rs) = NULL;
WS_RES *(*fp_ws_query_timeout)(WS_TAOS *taos, const char *sql, uint32_t seconds) = NULL;
int64_t (*fp_ws_take_timing)(WS_RES *rs) = NULL;
int32_t (*fp_ws_errno)(WS_RES *rs) = NULL;
const char *(*fp_ws_errstr)(WS_RES *rs) = NULL;
int32_t (*fp_ws_affected_rows)(const WS_RES *rs) = NULL;
int64_t (*fp_ws_affected_rows64)(const WS_RES *rs) = NULL;
int32_t (*fp_ws_select_db)(WS_TAOS *taos, const char *db) = NULL;
const char *(*fp_ws_get_client_info)(void) = NULL;
int32_t (*fp_ws_field_count)(const WS_RES *rs) = NULL;
bool (*fp_ws_is_update_query)(const WS_RES *rs) = NULL;
const struct WS_FIELD *(*fp_ws_fetch_fields)(WS_RES *rs) = NULL;
const struct WS_FIELD_V2 *(*fp_ws_fetch_fields_v2)(WS_RES *rs) = NULL;
int32_t (*fp_ws_fetch_raw_block)(WS_RES *rs, const void **pData, int32_t *numOfRows) = NULL;
bool (*fp_ws_is_null)(const WS_RES *rs, int32_t row, int32_t col) = NULL;
WS_ROW (*fp_ws_fetch_row)(WS_RES *rs) = NULL;
int32_t (*fp_ws_num_fields)(const WS_RES *rs) = NULL;
int32_t (*fp_ws_free_result)(WS_RES *rs) = NULL;
int32_t (*fp_ws_result_precision)(const WS_RES *rs) = NULL;
const void *(*fp_ws_get_value_in_block)(WS_RES *rs, int32_t row, int32_t col, uint8_t *ty, uint32_t *len) = NULL;
void (*fp_ws_timestamp_to_rfc3339)(uint8_t *dest, int64_t raw, int32_t precision, bool use_z) = NULL;
int32_t (*fp_ws_print_row)(char *str, int32_t str_len, WS_ROW row, const struct WS_FIELD *fields,
                           int32_t num_fields) = NULL;
int32_t (*fp_ws_get_current_db)(WS_TAOS *taos, char *database, int len, int *required) = NULL;
WS_RES *(*fp_ws_schemaless_insert_raw)(WS_TAOS *taos, const char *lines, int len, int32_t *totalRows, int protocal,
                                       int precision) = NULL;
WS_RES *(*fp_ws_schemaless_insert_raw_with_reqid)(WS_TAOS *taos, const char *lines, int len, int32_t *totalRows,
                                                  int protocal, int precision, uint64_t reqid) = NULL;
WS_RES *(*fp_ws_schemaless_insert_raw_ttl)(WS_TAOS *taos, const char *lines, int len, int32_t *totalRows, int protocal,
                                           int precision, int ttl) = NULL;
WS_RES *(*fp_ws_schemaless_insert_raw_ttl_with_reqid)(WS_TAOS *taos, const char *lines, int len, int32_t *totalRows,
                                                      int protocal, int precision, int ttl, uint64_t reqid) = NULL;
WS_STMT *(*fp_ws_stmt_init)(const WS_TAOS *taos) = NULL;
WS_STMT *(*fp_ws_stmt_init_with_reqid)(const WS_TAOS *taos, uint64_t req_id) = NULL;
int (*fp_ws_stmt_prepare)(WS_STMT *stmt, const char *sql, unsigned long len) = NULL;
int (*fp_ws_stmt_set_tbname)(WS_STMT *stmt, const char *name) = NULL;
int (*fp_ws_stmt_set_sub_tbname)(WS_STMT *stmt, const char *name) = NULL;
int (*fp_ws_stmt_set_tbname_tags)(WS_STMT *stmt, const char *name, const WS_MULTI_BIND *bind, uint32_t len) = NULL;
int (*fp_ws_stmt_get_tag_fields)(WS_STMT *stmt, int *fieldNum, struct StmtField **fields) = NULL;
int (*fp_ws_stmt_get_col_fields)(WS_STMT *stmt, int *fieldNum, struct StmtField **fields) = NULL;
int (*fp_ws_stmt_reclaim_fields)(WS_STMT *stmt, struct StmtField **fields, int fieldNum) = NULL;
int (*fp_ws_stmt_is_insert)(WS_STMT *stmt, int *insert) = NULL;
int (*fp_ws_stmt_set_tags)(WS_STMT *stmt, const WS_MULTI_BIND *bind, uint32_t len) = NULL;
int (*fp_ws_stmt_bind_param_batch)(WS_STMT *stmt, const WS_MULTI_BIND *bind, uint32_t len) = NULL;
int (*fp_ws_stmt_add_batch)(WS_STMT *stmt) = NULL;
int (*fp_ws_stmt_execute)(WS_STMT *stmt, int32_t *affected_rows) = NULL;
int (*fp_ws_stmt_affected_rows)(WS_STMT *stmt) = NULL;
int (*fp_ws_stmt_affected_rows_once)(WS_STMT *stmt) = NULL;
int (*fp_ws_stmt_num_params)(WS_STMT *stmt, int *nums) = NULL;
int (*fp_ws_stmt_get_param)(WS_STMT *stmt, int idx, int *type, int *bytes) = NULL;
const char *(*fp_ws_stmt_errstr)(WS_STMT *stmt) = NULL;
int32_t (*fp_ws_stmt_close)(WS_STMT *stmt) = NULL;
ws_tmq_conf_t *(*fp_ws_tmq_conf_new)(void) = NULL;
enum ws_tmq_conf_res_t (*fp_ws_tmq_conf_set)(ws_tmq_conf_t *conf, const char *key, const char *value) = NULL;
int32_t (*fp_ws_tmq_conf_destroy)(ws_tmq_conf_t *conf) = NULL;
ws_tmq_list_t *(*fp_ws_tmq_list_new)(void) = NULL;
int32_t (*fp_ws_tmq_list_append)(ws_tmq_list_t *list, const char *topic) = NULL;
int32_t (*fp_ws_tmq_list_destroy)(ws_tmq_list_t *list) = NULL;
int32_t (*fp_ws_tmq_list_get_size)(ws_tmq_list_t *list) = NULL;
char **(*fp_ws_tmq_list_to_c_array)(const ws_tmq_list_t *list, uint32_t *topic_num) = NULL;
int32_t (*fp_ws_tmq_list_free_c_array)(char **c_str_arry, uint32_t topic_num) = NULL;
ws_tmq_t *(*fp_ws_tmq_consumer_new)(ws_tmq_conf_t *conf, const char *dsn, char *errstr, int errstr_len) = NULL;
int32_t (*fp_ws_tmq_consumer_close)(ws_tmq_t *tmq) = NULL;
int32_t (*fp_ws_tmq_subscribe)(ws_tmq_t *tmq, const ws_tmq_list_t *topic_list) = NULL;
int32_t (*fp_ws_tmq_unsubscribe)(ws_tmq_t *tmq) = NULL;
WS_RES *(*fp_ws_tmq_consumer_poll)(ws_tmq_t *tmq, int64_t timeout) = NULL;
const char *(*fp_ws_tmq_get_topic_name)(const WS_RES *rs) = NULL;
const char *(*fp_ws_tmq_get_db_name)(const WS_RES *rs) = NULL;
const char *(*fp_ws_tmq_get_table_name)(const WS_RES *rs) = NULL;
int32_t (*fp_ws_tmq_get_vgroup_id)(const WS_RES *rs) = NULL;
int64_t (*fp_ws_tmq_get_vgroup_offset)(const WS_RES *rs) = NULL;
enum ws_tmq_res_t (*fp_ws_tmq_get_res_type)(const WS_RES *rs) = NULL;
int32_t (*fp_ws_tmq_get_topic_assignment)(ws_tmq_t *tmq, const char *pTopicName,
                                          struct ws_tmq_topic_assignment **assignment, int32_t *numOfAssignment) = NULL;
int32_t (*fp_ws_tmq_free_assignment)(struct ws_tmq_topic_assignment *pAssignment, int32_t numOfAssignment) = NULL;
int32_t (*fp_ws_tmq_commit_sync)(ws_tmq_t *tmq, const WS_RES *rs) = NULL;
int32_t (*fp_ws_tmq_commit_offset_sync)(ws_tmq_t *tmq, const char *pTopicName, int32_t vgId, int64_t offset) = NULL;
int64_t (*fp_ws_tmq_committed)(ws_tmq_t *tmq, const char *pTopicName, int32_t vgId) = NULL;
int32_t (*fp_ws_tmq_offset_seek)(ws_tmq_t *tmq, const char *pTopicName, int32_t vgId, int64_t offset) = NULL;
int64_t (*fp_ws_tmq_position)(ws_tmq_t *tmq, const char *pTopicName, int32_t vgId) = NULL;
const char *(*fp_ws_tmq_errstr)(ws_tmq_t *tmq) = NULL;
#endif