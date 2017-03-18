DROP    DATABASE IF EXISTS test;
DROP    USER IF EXISTS test;

CREATE  DATABASE test;
CREATE  USER test WITH PASSWORD 'testPassword';
GRANT   ALL ON DATABASE test TO test;


\connect test;

CREATE TABLE People ( ID  INTEGER, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE PRECISION);

INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (14, 'Tom Smith', 32, 'M', 56.34);
INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (38, 'Mary Twit', 29, 'F', 34.45678);

CREATE TABLE PeopleExtra ( ID  SERIAL NOT NULL PRIMARY KEY, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE PRECISION);

-- See test/TableIntTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE IntTypes(
     ID  INTEGER
    ,B0  BIT(4)                 -- 4  bit
    ,B1  Bit(12)                -- 12 bit
    ,B2  Bit(24)                -- 24 bit
    ,B3  Bit(48)                -- 48 bit

    ,I3  SMALLINT               -- 2 byte    -32768 -> 32767
    ,I6  INTEGER                -- 4 byte    -2147483648 -> 2147483647
    ,I7  BIGINT);               -- 8 byte    -9223372036854775808 -> 9223372036854775807

INSERT INTO IntTypes(ID,
                     B0, B1, B2, B3,
                     I3,I6,I7)
     VALUES(1,
            CAST(x'A' AS BIT(4)), CAST(x'B3D' AS BIT(12)), CAST(x'123456' AS BIT(24)), CAST(x'FEDCBA987654' AS BIT(48)),
            CAST(x'34' AS INT), CAST(x'9A' AS INT), CAST(x'CD' AS INT));
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     I3,I6,I7)
     VALUES(2,
            CAST(0 AS BIT(4)), CAST(0 AS BIT(12)), CAST(0 AS BIT(24)), CAST(0 AS BIT(48)),
            -32768, -2147483648, -9223372036854775808);
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     I3,I6,I7)
     VALUES(3,
            CAST(x'F' AS BIT(4)), CAST(x'FFF' AS BIT(12)), CAST(x'FFFFFF' AS BIT(24)), CAST(x'FFFFFFFFFFFF' AS BIT(48)),
            32767, 2147483647, 9223372036854775807);

INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     I3,I6,I7)
     VALUES(4,
            CAST(8 AS BIT(4)), CAST(8 AS BIT(12)), CAST(8 AS BIT(24)), CAST(8 AS BIT(48)),
            8, 8, 8);
INSERT INTO IntTypes(ID,
                     B0,B1,B2,B3,
                     I3,I6,I7)
     VALUES(5,
            CAST(14 AS BIT(4)), CAST(14 AS BIT(12)), CAST(14 AS BIT(24)), CAST(14 AS BIT(48)),
            14, 14, 14);

-- See test/TableRealTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE RealTypes(
     ID  INTEGER

    ,R1  DOUBLE PRECISION
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
    ,ST3  TIMESTAMP);

INSERT INTO TimeStampTypes(ID, ST1, ST2, ST3) VALUES (1, '1969-04-25', '06:15:43', '1973-04-25 06:15:43');

-- See test/TableBinaryTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE BinaryTypes(
     ID  INTEGER

    ,B4  BYTEA);
    

INSERT INTO BinaryTypes(ID, B4)
 VALUES(1, 'A Normal Blob');
INSERT INTO BinaryTypes(ID, B4)
 VALUES(2, 'K');

-- See test/TableStringTest.cpp
--      Add tests to this file for integers.
--      Add immutable test data below
CREATE TABLE StringTypes(
     ID  INTEGER

    ,S2  VARCHAR(32)
    ,S4  TEXT
);

INSERT INTO StringTypes(ID, S2, S4)
 VALUES(1, 'A normal Var char', 'This is normal text');
INSERT INTO StringTypes(ID, S2, S4)
 VALUES(2, 'F', 'F');

