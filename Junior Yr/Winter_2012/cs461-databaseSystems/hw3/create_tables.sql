CREATE TABLE Photographers (
	ID_PHOTOGRAPHER SERIAL,
	FIRST_NAME VARCHAR(20),
	LAST_NAME VARCHAR(20),
	ORDER_FORM VARCHAR(30),
	PRIMARY KEY(ID_PHOTOGRAPHER)
);

CREATE TABLE Photographs (
	Id_photo SERIAL,
	Filename VARCHAR(20),
	Title VARCHAR(20),
	Description VARCHAR(100),
	Id_photographer INT,
	Photo_order INT,
	Height INT,
	Width INT,
	City VARCHAR(20),
	State VARCHAR(20),
	Country VARCHAR(20),
	Date_taken DATE,
	PRIMARY KEY(ID_PHOTO),
	FOREIGN KEY(Id_photographer) REFERENCES Photographers(Id_photographer)
);

CREATE TABLE PhotoSales (
	id_sale SERIAL,
	id_photo INT,
	sale_price INT,
	tax INT,
	sale_date DATE,
	Primary key (id_sale),
	foreign key (id_photo) references Photographs(id_photo)
);

CREATE TABLE RefPhotoAttributes (
	id_attribute serial,
	attribute_desc varchar(20),
	primary key(id_attribute)
);

CREATE TABLE PhotoAttributes (
	id_photo int,
	id_attribute int,
	primary key(id_photo, id_attribute),
	foreign key(id_photo) references photographs(id_photo),
	foreign key(id_attribute) references RefPhotoAttributes(id_attribute)
);

CREATE TABLE RefSalescategory (
	id_sales_category int,
	sales_category varchar(20),
	primary key(id_sales_category)
);

CREATE TABLE SalesCategories (
	id_sales_category int,
	id_photo int,
	primary key (id_photo,id_sales_category),
	foreign key (id_sales_category) references RefSalesCategory(id_sales_category),
	foreign key (id_photo) references photographs(id_photo)
);

CREATE TABLE UpdatedPhotographs (
	filename varchar(20),
	title varchar(20),
	primary key (filename)
);

CREATE TABLE RefEquipmentUsed (
	id_equipment INT,
	equipmentdesc varchar(30),
	primary key (id_equipment)
);

CREATE TABLE PhotoEquipment (
	id_photo INT,
	id_equipment INT,
	primary key (id_photo,id_equipment),
	foreign key (id_photo) REFERENCES Photographs(id_photo),
	foreign key (id_equipment) REFERENCES RefEquipmentUsed(id_equipment)
);

/** FOLLOWING IS FOR #9 OF THE HW notice: sample data included */

CREATE TABLE Students (
	id_student int,
	id_class int,
	grade int
);

INSERT INTO Students (id_student,id_class,grade) values (1,1,90);
INSERT INTO Students (id_student,id_class,grade) values (1,2,80);
INSERT INTO Students (id_student,id_class,grade) values (2,1,99);
INSERT INTO Students (id_student,id_class,grade) values (3,1,80);
INSERT INTO Students (id_student,id_class,grade) values (4,1,70);
INSERT INTO Students (id_student,id_class,grade) values (5,2,78);
INSERT INTO Students (id_student,id_class,grade) values (6,2,90);
INSERT INTO Students (id_student,id_class,grade) values (6,3,50);
INSERT INTO Students (id_student,id_class,grade) values (7,3,90);
