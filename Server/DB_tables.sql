drop table if exists private_history;	
create table private_history(
`user_name` varchar(100),
`create_time` varchar (50),
`content` text(1200),
`recevie_name` varchar(30),
primary key(user_name, recevie_name)
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
`password` varchar(100)
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

