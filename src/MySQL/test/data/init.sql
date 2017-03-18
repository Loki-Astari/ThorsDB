show databases;

DROP    DATABASE IF EXISTS test;
DROP    USER IF EXISTS 'test'@'localhost';

CREATE  DATABASE IF NOT EXISTS test;
CREATE  USER 'test'@'localhost' IDENTIFIED BY 'testPassword';
GRANT   ALL ON test.* TO 'test'@'localhost';


USE test;

CREATE TABLE People ( ID  INTEGER, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE);

INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (14, "Tom Smith", 32, 'M', 56.34);
INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (38, "Mary Twit", 29, 'F', 34.45678);

CREATE TABLE PeopleExtra ( ID  INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE);

-- See test/TableIntTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE IntTypes(
     ID  INTEGER
    ,B0  BIT(4)                 -- 4  bit
    ,B1  Bit(12)                -- 12 bit
    ,B2  Bit(24)                -- 24 bit
    ,B3  Bit(48)                -- 48 bit

    ,U2  TINYINT     UNSIGNED   -- 1 byte    0 -> 255
    ,U3  SMALLINT    UNSIGNED   -- 2 byte    0 -> 65536
    ,U4  MEDIUMINT   UNSIGNED   -- 3 byte    0 -> 16777215
    ,U5  INT         UNSIGNED   -- 4 byte    0 -> 4294967295
    ,U6  INTEGER     UNSIGNED
    ,U7  BIGINT      UNSIGNED   -- 8 byte    0 -> 18446744073709551615

    ,I2  TINYINT                -- 1 byte    -128 -> 127
    ,I3  SMALLINT               -- 2 byte    -32768 -> 32767
    ,I4  MEDIUMINT              -- 3 byte    -8388608 -> 8388607
    ,I5  INT                    -- 4 byte    -2147483648 -> 2147483647
    ,I6  INTEGER
    ,I7  BIGINT);               -- 8 byte    -9223372036854775808 -> 9223372036854775807

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

INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(4,
            8, 8, 8, 8,
            8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8);
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     U2,U3,U4,U5,U6,U7,
                     I2,I3,I4,I5,I6,I7)
     VALUES(5,
            14, 14, 14, 14,
            14, 14, 14, 14, 14, 14,
            14, 14, 14, 14, 14, 14);

-- See test/TableRealTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE RealTypes(
     ID  INTEGER

    ,R1  DOUBLE
    ,R2  FLOAT

    ,D1  DECIMAL
    ,D2  NUMERIC);

INSERT INTO RealTypes(ID, R1, R2, D1, D2) VALUES(1, 10, 11, 12, 13);

-- See test/TableTimeTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE TimeStampTypes(
     ID  INTEGER

    ,ST1  DATE
    ,ST2  TIME
    ,ST3  TIMESTAMP
    ,ST4  DATETIME
    ,ST5  YEAR);

INSERT INTO TimeStampTypes(ID, ST1, ST2, ST3, ST4, ST5) VALUES (1, '1969-04-25', '06:15:43', '1973-04-25 06:15:43', '1969-04-25 06:15:43', '2016');

-- See test/TableBinaryTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE BinaryTypes(
     ID  INTEGER

    ,B1  BINARY
    ,B2  VARBINARY(32)
    ,B3  TINYBLOB
    ,B4  BLOB
    ,B5  MEDIUMBLOB
    ,B6  LONGBLOB);

INSERT INTO BinaryTypes(ID, B1, B2, B3, B4, B5, B6)
 VALUES(1, 'B', 'A variable binary string', 'A Tiny Blob', 'A Normal Blob', 'A Medium Sized Blob', 'This is the best I can do for long Blob');
INSERT INTO BinaryTypes(ID, B1, B2, B3, B4, B5, B6)
 VALUES(2, 'K', 'K', 'K', 'K', 'K', 'K');

-- See test/TableStringTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE StringTypes(
     ID  INTEGER

    ,S1  CHAR(1)
    ,S2  VARCHAR(32)
    ,S3  TINYTEXT       -- BLOB
    ,S4  TEXT           -- BLOB
    ,S5  MEDIUMTEXT     -- BLOB
    ,S6  LONGTEXT       -- BLOB
);

INSERT INTO StringTypes(ID, S1, S2, S3, S4, S5, S6)
 VALUES(1, 'x', "A normal Var char", "Tiny Text", "This is normal text", "This is some medium length test", "This is some long text");
INSERT INTO StringTypes(ID, S1, S2, S3, S4, S5, S6)
 VALUES(2, 'F', "F", "F", "F", "F", "F");

