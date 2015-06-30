show databases;

DROP    DATABASE IF EXISTS test;
CREATE  DATABASE IF NOT EXISTS test;

USE test;

CREATE TABLE People ( ID  INTEGER, Name VARCHAR(64), Age  SMALLINT, Sex  CHAR(1), Height DOUBLE);

INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (14, "Tom Smith", 32, 'M', 56.34);
INSERT INTO  People (ID, Name, Age, Sex, Height) VALUES (38, "Mary Twit", 29, 'F', 34.45678);

