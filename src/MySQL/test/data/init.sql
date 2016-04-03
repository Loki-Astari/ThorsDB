show databases;

DROP    DATABASE IF EXISTS test;
DROP    USER 'test'@'localhost';

CREATE  DATABASE IF NOT EXISTS test;
CREATE  USER 'test'@'localhost' IDENTIFIED BY 'testPassword';
GRANT   ALL ON test.* TO 'test'@'localhost';


USE test;

CREATE TABLE People ( ID  INTEGER, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE);

INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (14, "Tom Smith", 32, 'M', 56.34);
INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (38, "Mary Twit", 29, 'F', 34.45678);


CREATE TABLE IntTypes(
     ID  INTEGER
    ,B0  BIT(4)                 # 4  bit
    ,B1  Bit(12)                # 12 bit
    ,B2  Bit(24)                # 24 bit
    ,B3  Bit(48)                # 48 bit

    ,U2  TINYINT     UNSIGNED   # 1 byte    0 -> 255
    ,U3  SMALLINT    UNSIGNED   # 2 byte    0 -> 65536
    ,U4  MEDIUMINT   UNSIGNED   # 3 byte    0 -> 16777215
    ,U5  INT         UNSIGNED   # 4 byte    0 -> 4294967295
    ,U6  INTEGER     UNSIGNED
    ,U7  BIGINT      UNSIGNED   # 8 byte    0 -> 18446744073709551615

    ,I2  TINYINT                # 1 byte    -128 -> 127
    ,I3  SMALLINT               # 2 byte    -32768 -> 32767
    ,I4  MEDIUMINT              # 3 byte    -8388608 -> 8388607
    ,I5  INT                    # 4 byte    -2147483648 -> 2147483647
    ,I6  INTEGER
    ,I7  BIGINT);               # 8 byte    -9223372036854775808 -> 9223372036854775807

INSERT INTO IntTypes(ID,
                     B0, B1, B2, B3,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(1,
            0xA, 0xB3D, 0x123456, 0xFEDCBA987654,
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xCD,
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xCD);
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(2,
            0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            -128, -32768, -8388608, -2147483648, -2147483648, -9223372036854775808);
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(3,
            0xF, 0xFFF, 0xFFFFFF, 0xFFFFFFFFFFFF,
            255, 65535, 16777215, 4294967295, 4294967295, 18446744073709551615,
            127, 32767, 8388607, 2147483647, 2147483647, 9223372036854775807);

CREATE TABLE RealTypes(
     ID  INTEGER

    ,R1  REAL        UNSIGNED
    ,R2  DOUBLE      UNSIGNED
    ,R3  FLOAT       UNSIGNED
    ,R4  REAL
    ,R5  DOUBLE
    ,R6  FLOAT

    ,D1  DECIMAL     UNSIGNED
    ,D2  NUMERIC     UNSIGNED
    ,D3  DECIMAL
    ,D4  NUMERIC);

CREATE TABLE TimeStampTypes(
     ID  INTEGER

    ,ST1  DATE
    ,ST2  TIME
    ,ST3  TIMESTAMP
    ,ST4  DATETIME
    ,ST5  YEAR);

CREATE TABLE BinaryTypes(
     ID  INTEGER

    ,B1  BINARY
    ,B2  VARBINARY(32)
    ,B3  TINYBLOB
    ,B4  BLOB
    ,B5  MEDIUMBLOB
    ,B6  LONGBLOB);

CREATE TABLE StringTypes(
     ID  INTEGER

    ,S1  CHAR
    ,S2  VARCHAR(32)
    ,S3  TINYTEXT
    ,S4  TEXT
    ,S5  MEDIUMTEXT
    ,S6  LONGTEXT);

