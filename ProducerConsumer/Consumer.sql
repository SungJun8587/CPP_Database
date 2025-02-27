-- MSSQL 테이블 생성
CREATE TABLE [dbo].[Consumer](
	[No] [int] IDENTITY(1,1) NOT NULL,
	[Name1] [nvarchar](50) NULL,
	[Name2] [nvarchar](50) NOT NULL,
	[Flag] [tinyint] NOT NULL,
	[Age] [int] NULL,
 CONSTRAINT [pk_Consumer_No] PRIMARY KEY CLUSTERED 
(
	[No] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO


-- MYSQL 테이블 생성
CREATE TABLE `Consumer` (
   `No` int NOT NULL AUTO_INCREMENT,
   `Name1` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
   `Name2` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
   `Flag` bit(1) NOT NULL,
   `Age` int DEFAULT NULL,
   PRIMARY KEY (`No`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;