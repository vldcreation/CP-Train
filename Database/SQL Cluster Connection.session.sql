/*
drop table if exists student;
create table student(
    id int PRIMARY KEY AUTO_INCREMENT,
    nim VARCHAR(12) UNIQUE not null,
    name VARCHAR(234) not null,
    gender enum('m','f'),
    address varchar(256) null
);
*/

/*
insert into student(`nim`,`name`,`gender`,`address`)
VALUES('11319028','Vicktor Desrony','m','Tanah Jawa');
*/

select * from student;