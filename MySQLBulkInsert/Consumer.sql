-- MYSQL 테이블 생성
CREATE TABLE `Consumer` (
   `No` int NOT NULL AUTO_INCREMENT COMMENT '일련번호',
   `Name1` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '이름1',
   `Name2` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '이름2',
   `Flag` bit(1) NOT NULL COMMENT '상태(0/1: false/true)',
   `Age` int NOT NULL COMMENT '나이',
   PRIMARY KEY (`No`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='소비자';