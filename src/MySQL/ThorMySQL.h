#ifndef THORSANVIL_MYSQL_DETAILS_THOR_MYSQL_H
#define THORSANVIL_MYSQL_DETAILS_THOR_MYSQL_H

#define CLIENT_LONG_PASSWORD                    0x00000001
#define CLIENT_FOUND_ROWS                       0x00000002
#define CLIENT_LONG_FLAG                        0x00000004
#define CLIENT_CONNECT_WITH_DB                  0x00000008

#define CLIENT_NO_SCHEMA                        0x00000010
#define CLIENT_COMPRESS                         0x00000020
#define CLIENT_ODBC                             0x00000040
#define CLIENT_LOCAL_FILES                      0x00000080

#define CLIENT_IGNORE_SPACE                     0x00000100
#define CLIENT_PROTOCOL_41                      0x00000200
#define CLIENT_INTERACTIVE                      0x00000400
#define CLIENT_SSL                              0x00000800

#define CLIENT_IGNORE_SIGPIPE                   0x00001000
#define CLIENT_TRANSACTIONS                     0x00002000
#define CLIENT_RESERVED                         0x00004000
#define CLIENT_SECURE_CONNECTION                0x00008000

#define CLIENT_MULTI_STATEMENTS                 0x00010000
#define CLIENT_MULTI_RESULTS                    0x00020000
#define CLIENT_PS_MULTI_RESULTS                 0x00040000
#define CLIENT_PLUGIN_AUTH                      0x00080000

#define CLIENT_CONNECT_ATTRS                    0x00100000
#define CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA   0x00200000
#define CLIENT_SET_CLIENT                       0x00400000
#define CLIENT_SESSION_TRACK                    0x00800000

#define CLIENT_DEPRECATE_EOF                    0x01000000


#define SERVER_STATUS_IN_TRANS                  0x0001
#define SERVER_STATUS_AUTOCOMMIT                0x0002
#define SERVER_MORE_RESULTS_EXISTS              0x0008
#define SERVER_STATUS_NO_GOOD_INDEX_USED        0x0010
#define SERVER_STATUS_NO_INDEX_USED             0x0020
#define SERVER_STATUS_CURSOR_EXISTS             0x0040
#define SERVER_STATUS_LAST_ROW_SENT             0x0080
#define SERVER_STATUS_DB_DROPPED                0x0100
#define SERVER_STATUS_NO_BACKSLASH_ESCAPES      0x0200
#define SERVER_STATUS_METADATA_CHANGED          0x0400
#define SERVER_QUERY_WAS_SLOW                   0x0800
#define SERVER_PS_OUT_PARAMS                    0x1000
#define SERVER_STATUS_IN_TRANS_READONLY         0x2000
#define SERVER_SESSION_STATE_CHANGED            0x4000


#define CURSOR_TYPE_NO_CURSOR                   0x00
#define CURSOR_TYPE_READ_ONLY                   0x01
#define CURSOR_TYPE_FOR_UPDATE                  0x02
#define CURSOR_TYPE_SCROLLABLE                  0x04

#define COM_STMT_PREPARE                        0x16
#define COM_STMT_EXECUTE                        0x17
#define COM_STMT_CLOSE                          0x19
#define COM_STMT_RESET                          0x1A

#define MYSQL_TYPE_DECIMAL                      0x00
#define MYSQL_TYPE_TINY                         0x01
#define MYSQL_TYPE_SHORT                        0x02
#define MYSQL_TYPE_LONG                         0x03
#define MYSQL_TYPE_FLOAT                        0x04
#define MYSQL_TYPE_DOUBLE                       0x05
#define MYSQL_TYPE_NULL                         0x06
#define MYSQL_TYPE_TIMESTAMP                    0x07
#define MYSQL_TYPE_LONGLONG                     0x08
#define MYSQL_TYPE_INT24                        0x09
#define MYSQL_TYPE_DATE                         0x0a
#define MYSQL_TYPE_TIME                         0x0b
#define MYSQL_TYPE_DATETIME                     0x0c
#define MYSQL_TYPE_YEAR                         0x0d
#define MYSQL_TYPE_NEWDATE                      0x0e
#define MYSQL_TYPE_VARCHAR                      0x0f
#define MYSQL_TYPE_BIT                          0x10
#define MYSQL_TYPE_TIMESTAMP2                   0x11
#define MYSQL_TYPE_DATETIME2                    0x12
#define MYSQL_TYPE_TIME2                        0x13
#define MYSQL_TYPE_NEWDECIMAL                   0xf6
#define MYSQL_TYPE_ENUM                         0xf7
#define MYSQL_TYPE_SET                          0xf8
#define MYSQL_TYPE_TINY_BLOB                    0xf9
#define MYSQL_TYPE_MEDIUM_BLOB                  0xfa
#define MYSQL_TYPE_LONG_BLOB                    0xfb
#define MYSQL_TYPE_BLOB                         0xfc
#define MYSQL_TYPE_VAR_STRING                   0xfd
#define MYSQL_TYPE_STRING                       0xfe
#define MYSQL_TYPE_GEOMETRY                     0xff

#define MYSQL_TYPE_TINY_UNSIGNED                  0x101
#define MYSQL_TYPE_SHORT_UNSIGNED                 0x102
#define MYSQL_TYPE_LONG_UNSIGNED                  0x103
#define MYSQL_TYPE_LONGLONG_UNSIGNED              0x108
#define MYSQL_TYPE_INT24_UNSIGNED                 0x109
#endif
