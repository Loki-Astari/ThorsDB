
CREATE  DATABASE IF NOT EXISTS test;
-- CREATE  USER 'test'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'testPassword';
CREATE  USER 'test'@'localhost' IDENTIFIED BY 'testPassword';
CREATE  USER 'ssluser'@'localhost' IDENTIFIED BY 'sslPassword';
CREATE  USER 'ssluserEmpty'@'localhost' IDENTIFIED BY '';
GRANT   ALL ON test.* TO 'test'@'localhost';
GRANT   ALL ON test.* TO 'ssluser'@'localhost';
GRANT   ALL ON test.* TO 'ssluserEmpty'@'localhost';


