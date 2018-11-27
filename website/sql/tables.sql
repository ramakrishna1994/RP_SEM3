create table running(
   pid INT NOT NULL,
   status int NOT NULL,
   PRIMARY KEY ( pid )
);

create table strace(
   id INT NOT NULL AUTO_INCREMENT,
   pid INT NOT NULL,
   val TEXT ,
   PRIMARY KEY ( id )
);


create table systemcalls(
   id INT NOT NULL AUTO_INCREMENT,
   scall text NOT NULL,
   cnt int ,
   PRIMARY KEY ( id )
);

create table top5pidswithtop5systemcalls(
   pid INT NOT NULL,
   scall text NOT NULL,
   cnt int ,
   PRIMARY KEY ( pid )
);
