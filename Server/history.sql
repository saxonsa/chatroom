create table history(
`hid` int primary key not null auto_increment,
`user_name` varchar(30),
`create_time` DATETIME DEFAULT now(),
`content` text(1200)
)default charset=utf8;