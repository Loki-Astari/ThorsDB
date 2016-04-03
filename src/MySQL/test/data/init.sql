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
    ,B0  BIT(4)                 # 4 bit (default)

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
                     B0,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(1,
            0xA,
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xCD,
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xCD);
INSERT INTO IntTypes(ID,
                     B0,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(2,
            0,
            0, 0, 0, 0, 0, 0,
            -128, -32768, -8388608, -2147483648, -2147483648, -9223372036854775808);
INSERT INTO IntTypes(ID,
                     B0,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(3,
            0xF,
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

CREATE TABLE ExtendedTypes(
     ID  INTEGER
    ,E1  ENUM('value1','value2','value3')
    ,E2  SET('value1','value2','value3'));

                     R1,R2,R3,R4,R5,R5,
                     D1,D2,D3,D4,
                     T1,T2,T3,T4,T5,
                     B1,B2,B3,B4,B5,B6,
                     S1,S2,S3,S4,S5,S6,
                     E1,E2)
        VALUES(1,
                

