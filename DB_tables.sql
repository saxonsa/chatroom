drop table if exists private_history;	
create table private_history(
`user_name` varchar(100),
`create_time` DATETIME,
`content` text(1200),
`recevie_name` varchar(30),
primary key(user_name, recevie_name, create_time)
)default charset=utf8;

drop table if exists group_history;	
create table group_history(
`ghid` int primary key not null auto_increment,
`user_name` varchar(100),
`create_time` varchar (50),
`content` text(1200),
`rid` int
)default charset=utf8;

drop table if exists users;	
create table users(
`user_name` varchar(100) primary key,
`password` varchar(100),
`status` int
)default charset=utf8;

drop table if exists room_mem;	
create table room_mem(
`rid` int,
`user_name` varchar(100)
)default charset=utf8;

drop table if exists room_info;	
create table room_info(
`rid` int primary key not null auto_increment,
`administrator` varchar(100),
`room_name` varchar(100)
)default charset=utf8;

-- add test data
insert into users values ("Saxon", "123", 0);
insert into users values ("Michael", "123", 0);
insert into users values ("Mano", 123, 0);
insert into users values ("Tina", 123, 0);

insert into room_info values (1, "Saxon", "Saxon room");
insert into room_info values (2, "Michael", "Michael room");
insert into room_info values (3, "Mano", "Mano room");
insert into room_info values (4, "Tina", "Tina room");
insert into room_info values (5, "Saxon", "CAPE TOWN");

insert into room_mem values (1, "Saxon");
insert into room_mem values (1, "Tina");
insert into room_mem values (2, "Michael");
insert into room_mem values (3, "Mano");
insert into room_mem values (4, "Tina");
insert into room_mem values (5, "Saxon");

insert into private_history values ("Saxon", "2020-12-14 12:00:00", "Hello Mano", "Mano");
insert into private_history values ("Saxon", "2020-12-14 12:00:13", "Hello Michael", "Michael");
insert into private_history values ("Mano", "2020-12-14 12:13:13", "Hello Saxon", "Saxon");
insert into private_history values ("Mano", "2020-12-14 12:13:13", "Hello Michael", "Michael");

insert into group_history values (1, "Saxon", "2020-12-14 12:15:00", "Hello Saxon room member", 1);
insert into group_history values (2, "Michael", "2020-12-14 12:15:00", "Hello Michael room member", 2);
insert into group_history values (3, "Mano", "2020-12-14 12:15:13", "Hello Mano room member", 3);
insert into group_history values (4, "Tina", "2020-12-14 12:15:33", "Hello Tina room member", 4);
insert into group_history values (5, "Saxon", "2020-12-14 12:15:22", "Hello Saxon room member", 5);
insert into group_history values (6, "Saxon", "2020-12-14 12:15:11", "Hello CAPE Town", 5);