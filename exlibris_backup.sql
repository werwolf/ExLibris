/*
SQLyog Ultimate v8.82 
MySQL - 5.0.51b-community-nt-log : Database - exlibris
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`exlibris` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `exlibris`;

/*Table structure for table `amort_log` */

DROP TABLE IF EXISTS `amort_log`;

CREATE TABLE `amort_log` (
  `id` int(11) NOT NULL auto_increment,
  `garage_id` int(11) NOT NULL,
  `sum` float(10,2) unsigned NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`garage_id`),
  KEY `fk_amortlog_garage_id` (`garage_id`),
  CONSTRAINT `fk_amortlog_garage_id` FOREIGN KEY (`garage_id`) REFERENCES `garage` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `amort_log` */

/*Table structure for table `authors` */

DROP TABLE IF EXISTS `authors`;

CREATE TABLE `authors` (
  `id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `dob` date NOT NULL,
  `sex` enum('M','W') NOT NULL,
  PRIMARY KEY  (`id`,`user_id`),
  KEY `fk_author_user_id` (`user_id`),
  CONSTRAINT `fk_author_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

/*Data for the table `authors` */

insert  into `authors`(`id`,`user_id`,`dob`,`sex`) values (4,6,'1967-02-04','M'),(5,27,'1974-01-01','M'),(6,28,'1979-11-10','M'),(7,30,'1979-05-12','W');

/*Table structure for table `authors_queries` */

DROP TABLE IF EXISTS `authors_queries`;

CREATE TABLE `authors_queries` (
  `author_id` int(11) NOT NULL,
  `query_id` varchar(17) NOT NULL,
  PRIMARY KEY  (`author_id`,`query_id`),
  KEY `fk_author_query_query_id` (`query_id`),
  KEY `fk_authors_queries_authors1` (`author_id`),
  CONSTRAINT `fk_authors_queries_authors1` FOREIGN KEY (`author_id`) REFERENCES `authors` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_author_query_query_id` FOREIGN KEY (`query_id`) REFERENCES `queries` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `authors_queries` */

insert  into `authors_queries`(`author_id`,`query_id`) values (4,'978-182-1-25-9'),(4,'978-551-1-16-6'),(4,'978-630-1-25-7');

/*Table structure for table `balance_log` */

DROP TABLE IF EXISTS `balance_log`;

CREATE TABLE `balance_log` (
  `id` int(11) NOT NULL auto_increment,
  `sum` float unsigned NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `balance_log` */

insert  into `balance_log`(`id`,`sum`,`date`) values (1,10000,'2011-04-02 23:17:19');

/*Table structure for table `book_sell_log` */

DROP TABLE IF EXISTS `book_sell_log`;

CREATE TABLE `book_sell_log` (
  `id` int(11) NOT NULL auto_increment,
  `client_id` int(11) NOT NULL,
  `query_id` varchar(17) NOT NULL,
  `sum` float unsigned NOT NULL,
  `number` int(11) NOT NULL,
  `income_date` datetime NOT NULL,
  `start_print_date` datetime NOT NULL,
  `end_print_date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`client_id`,`query_id`),
  KEY `fk_bookselllog_query_id` (`query_id`),
  KEY `fk_book_sell_log_clients1` (`client_id`),
  CONSTRAINT `fk_bookselllog_query_id` FOREIGN KEY (`query_id`) REFERENCES `queries` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_book_sell_log_clients1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

/*Data for the table `book_sell_log` */

/*Table structure for table `buy_log` */

DROP TABLE IF EXISTS `buy_log`;

CREATE TABLE `buy_log` (
  `id` int(11) NOT NULL auto_increment,
  `resource_id` int(11) NOT NULL,
  `supplier_id` int(11) NOT NULL,
  `garage_id` int(11) NOT NULL,
  `number` int(11) NOT NULL,
  `sum` float unsigned NOT NULL,
  `deal_date` datetime NOT NULL,
  `deliver_date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`resource_id`,`supplier_id`,`garage_id`),
  KEY `fk_buylog_resource_id` (`resource_id`),
  KEY `fk_buylog_garage_id` (`garage_id`),
  KEY `fk_buy_log_suppliers1` (`supplier_id`),
  CONSTRAINT `fk_buylog_garage_id` FOREIGN KEY (`garage_id`) REFERENCES `garage` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_buylog_resource_id` FOREIGN KEY (`resource_id`) REFERENCES `resources` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_buy_log_suppliers1` FOREIGN KEY (`supplier_id`) REFERENCES `suppliers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

/*Data for the table `buy_log` */

/*Table structure for table `clients` */

DROP TABLE IF EXISTS `clients`;

CREATE TABLE `clients` (
  `id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `company_name` varchar(45) NOT NULL,
  PRIMARY KEY  (`id`,`user_id`),
  KEY `fk_clients_users1` (`user_id`),
  CONSTRAINT `fk_clients_users1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `clients` */

insert  into `clients`(`id`,`user_id`,`company_name`) values (2,5,'KPI');

/*Table structure for table `garage` */

DROP TABLE IF EXISTS `garage`;

CREATE TABLE `garage` (
  `id` int(11) NOT NULL auto_increment,
  `model_id` int(11) NOT NULL,
  `driver` varchar(45) NOT NULL,
  `amort_quot` float NOT NULL,
  `last_amort_date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`model_id`),
  KEY `fk_garage_model_id` (`model_id`),
  CONSTRAINT `fk_garage_model_id` FOREIGN KEY (`model_id`) REFERENCES `models` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `garage` */

insert  into `garage`(`id`,`model_id`,`driver`,`amort_quot`,`last_amort_date`) values (1,1,'Petrov',0.999,'2011-03-29 11:43:01'),(2,2,'Pechkin',0.71,'2011-04-03 23:26:58');

/*Table structure for table `genres` */

DROP TABLE IF EXISTS `genres`;

CREATE TABLE `genres` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(45) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `title_UNIQUE` (`title`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

/*Data for the table `genres` */

insert  into `genres`(`id`,`title`) values (6,'Детектив'),(8,'Детское'),(2,'Криминал'),(9,'Математика'),(7,'Приключения'),(11,'Религия'),(3,'Роман'),(12,'Семья'),(10,'Эзотерика');

/*Table structure for table `limits` */

DROP TABLE IF EXISTS `limits`;

CREATE TABLE `limits` (
  `id` int(11) NOT NULL auto_increment,
  `quot_author` float unsigned NOT NULL,
  `quot_sell_book` float unsigned NOT NULL,
  `quot_sell_resource` float unsigned NOT NULL,
  `quot_sell_service` float unsigned NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

/*Data for the table `limits` */

insert  into `limits`(`id`,`quot_author`,`quot_sell_book`,`quot_sell_resource`,`quot_sell_service`,`date`) values (3,1.5,2,1.3,1.8,'2011-03-23 13:38:50');

/*Table structure for table `models` */

DROP TABLE IF EXISTS `models`;

CREATE TABLE `models` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(45) NOT NULL,
  `deltkm` float NOT NULL,
  `price` float NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `models` */

insert  into `models`(`id`,`title`,`deltkm`,`price`) values (1,'BMW',0.8,35000),(2,'Audi',0.85,28700);

/*Table structure for table `personnel` */

DROP TABLE IF EXISTS `personnel`;

CREATE TABLE `personnel` (
  `id` int(11) NOT NULL auto_increment,
  `post_id` int(11) NOT NULL,
  `lastname` varchar(45) NOT NULL,
  `name` varchar(45) NOT NULL,
  `salary_quot` float unsigned NOT NULL,
  `assign_date` date NOT NULL,
  PRIMARY KEY  (`id`,`post_id`),
  KEY `fk_personnel_post_id` (`post_id`),
  CONSTRAINT `fk_personnel_post_id` FOREIGN KEY (`post_id`) REFERENCES `posts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

/*Data for the table `personnel` */

insert  into `personnel`(`id`,`post_id`,`lastname`,`name`,`salary_quot`,`assign_date`) values (11,19,'Kostilskiy','Svirid',2,'2011-03-30'),(12,21,'Senkov','Semen',1.5,'2011-04-03');

/*Table structure for table `personnel_services` */

DROP TABLE IF EXISTS `personnel_services`;

CREATE TABLE `personnel_services` (
  `personnel_id` int(11) NOT NULL,
  `service_id` int(11) NOT NULL,
  PRIMARY KEY  (`personnel_id`,`service_id`),
  KEY `fk_personnel_service_service_id` (`service_id`),
  CONSTRAINT `fk_personnel_service_service_id` FOREIGN KEY (`service_id`) REFERENCES `services` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `personnel_services_ibfk_1` FOREIGN KEY (`personnel_id`) REFERENCES `personnel` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `personnel_services` */

insert  into `personnel_services`(`personnel_id`,`service_id`) values (11,4),(11,6),(12,8);

/*Table structure for table `posts` */

DROP TABLE IF EXISTS `posts`;

CREATE TABLE `posts` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(45) NOT NULL,
  `rate` float unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

/*Data for the table `posts` */

insert  into `posts`(`id`,`title`,`rate`) values (19,'дизайнер',1000),(20,'типограф',950),(21,'верстальщик',775);

/*Table structure for table `queries` */

DROP TABLE IF EXISTS `queries`;

CREATE TABLE `queries` (
  `id` varchar(17) NOT NULL,
  `genre_id` int(11) NOT NULL,
  `title` varchar(45) NOT NULL,
  `regdate` datetime NOT NULL,
  PRIMARY KEY  (`id`,`genre_id`),
  KEY `fk_query_genre_id` (`genre_id`),
  CONSTRAINT `fk_query_genre_id` FOREIGN KEY (`genre_id`) REFERENCES `genres` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `queries` */

insert  into `queries`(`id`,`genre_id`,`title`,`regdate`) values ('978-182-1-25-9',6,'Детективные истории','2011-03-30 00:00:00'),('978-296-1-29-4',2,'Криминальщина','2011-03-23 00:00:00'),('978-321-1-19-1',3,'Руслан и Людмила','2011-03-23 00:00:00'),('978-551-1-16-6',7,'За 80 дней вокруг света','2011-03-23 00:00:00'),('978-584-1-37-5',7,'Приключения Робинзона Крузо','2011-03-23 00:00:00'),('978-630-1-25-7',9,'Конспект по матанализу','2011-03-30 00:00:00'),('978-733-1-21-3',10,'БД та ІС','2011-03-23 00:00:00'),('978-906-1-40-2',12,'Моя дом - моя крепость','2011-03-23 00:00:00');

/*Table structure for table `queries_resources` */

DROP TABLE IF EXISTS `queries_resources`;

CREATE TABLE `queries_resources` (
  `query_id` varchar(17) NOT NULL,
  `resource_id` int(11) NOT NULL,
  `number` int(11) NOT NULL,
  PRIMARY KEY  (`query_id`,`resource_id`),
  KEY `fk_query_resource_query_id` (`query_id`),
  KEY `fk_query_resource_resource_id` (`resource_id`),
  CONSTRAINT `fk_query_resource_query_id` FOREIGN KEY (`query_id`) REFERENCES `queries` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_query_resource_resource_id` FOREIGN KEY (`resource_id`) REFERENCES `resources` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `queries_resources` */

insert  into `queries_resources`(`query_id`,`resource_id`,`number`) values ('978-182-1-25-9',9,1600),('978-182-1-25-9',10,1),('978-630-1-25-7',8,1600),('978-630-1-25-7',10,1),('978-630-1-25-7',12,1600);

/*Table structure for table `queries_services` */

DROP TABLE IF EXISTS `queries_services`;

CREATE TABLE `queries_services` (
  `query_id` varchar(17) NOT NULL,
  `service_id` int(11) NOT NULL,
  PRIMARY KEY  (`query_id`,`service_id`),
  KEY `fk_query_service_service_id` (`service_id`),
  KEY `fk_query_service_query_id` (`query_id`),
  CONSTRAINT `fk_query_service_query_id` FOREIGN KEY (`query_id`) REFERENCES `queries` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_query_service_service_id` FOREIGN KEY (`service_id`) REFERENCES `services` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `queries_services` */

/*Table structure for table `resource_sell_log` */

DROP TABLE IF EXISTS `resource_sell_log`;

CREATE TABLE `resource_sell_log` (
  `id` int(11) NOT NULL auto_increment,
  `client_id` int(11) NOT NULL,
  `resource_id` int(11) NOT NULL,
  `sum` float unsigned NOT NULL,
  `number` int(11) NOT NULL,
  `income_date` datetime NOT NULL,
  `deal_date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`client_id`,`resource_id`),
  KEY `fk_resourceselllog_resource_id` (`resource_id`),
  KEY `fk_resource_sell_log_clients1` (`client_id`),
  CONSTRAINT `fk_resourceselllog_resource_id` FOREIGN KEY (`resource_id`) REFERENCES `resources` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_resource_sell_log_clients1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `resource_sell_log` */

/*Table structure for table `resource_types` */

DROP TABLE IF EXISTS `resource_types`;

CREATE TABLE `resource_types` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(45) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `title_UNIQUE` (`title`)
) ENGINE=InnoDB AUTO_INCREMENT=65 DEFAULT CHARSET=utf8;

/*Data for the table `resource_types` */

insert  into `resource_types`(`id`,`title`) values (4,'Бумага'),(7,'Мягкий переплет'),(5,'Обложка'),(9,'Переплет на пружину'),(6,'Печать'),(8,'Твердый переплет');

/*Table structure for table `resources` */

DROP TABLE IF EXISTS `resources`;

CREATE TABLE `resources` (
  `id` int(11) NOT NULL auto_increment,
  `resource_type_id` int(11) NOT NULL,
  `title` varchar(45) NOT NULL,
  `price` float unsigned NOT NULL,
  PRIMARY KEY  (`id`,`resource_type_id`),
  UNIQUE KEY `title_UNIQUE` (`title`,`resource_type_id`),
  KEY `fk_resource_resourcetype_id` (`resource_type_id`),
  CONSTRAINT `fk_resource_resourcetype_id` FOREIGN KEY (`resource_type_id`) REFERENCES `resource_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=52 DEFAULT CHARSET=utf8;

/*Data for the table `resources` */

insert  into `resources`(`id`,`resource_type_id`,`title`,`price`) values (8,4,'белая бумага',1),(9,4,'глянцевая бумага',2),(10,5,'твердая обложка',1),(11,5,'мягкая обложка',0.5),(12,6,'лазерная печать',2),(43,7,'клеевое бесшвейное скрепление',3),(44,7,'скрепка',0.4),(45,7,'каналы металбинд',4),(46,8,'твердый переплет',2),(47,8,'металбинд',3),(48,8,'интегральный переплет',1),(49,9,'металлическая пружина',3.1),(50,9,'пластиковая пружина',2.7),(51,9,'канал металбинд',3);

/*Table structure for table `salary_log` */

DROP TABLE IF EXISTS `salary_log`;

CREATE TABLE `salary_log` (
  `id` int(11) NOT NULL auto_increment,
  `personnel_id` int(11) NOT NULL,
  `sum` float unsigned NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`personnel_id`),
  KEY `fk_salarylog_personnel_user_id` (`personnel_id`),
  CONSTRAINT `fk_salarylog_personnel_user_id` FOREIGN KEY (`personnel_id`) REFERENCES `personnel` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `salary_log` */

/*Table structure for table `service_sell_log` */

DROP TABLE IF EXISTS `service_sell_log`;

CREATE TABLE `service_sell_log` (
  `id` int(11) NOT NULL auto_increment,
  `client_id` int(11) NOT NULL,
  `service_id` int(11) NOT NULL,
  `sum` float unsigned NOT NULL,
  `income_date` datetime NOT NULL,
  `deal_date` datetime NOT NULL,
  PRIMARY KEY  (`id`,`client_id`,`service_id`),
  KEY `fk_serviceselllog_service_id` (`service_id`),
  KEY `fk_service_sell_log_clients1` (`client_id`),
  CONSTRAINT `fk_serviceselllog_service_id` FOREIGN KEY (`service_id`) REFERENCES `services` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_service_sell_log_clients1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `service_sell_log` */

/*Table structure for table `services` */

DROP TABLE IF EXISTS `services`;

CREATE TABLE `services` (
  `id` int(11) NOT NULL auto_increment,
  `title` varchar(45) NOT NULL,
  `price` float unsigned NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

/*Data for the table `services` */

insert  into `services`(`id`,`title`,`price`) values (4,'дизайн',500),(6,'переплёт',199.99),(7,'типография',600),(8,'верстка',1000);

/*Table structure for table `stock` */

DROP TABLE IF EXISTS `stock`;

CREATE TABLE `stock` (
  `resource_id` int(11) NOT NULL auto_increment,
  `number` int(11) NOT NULL,
  PRIMARY KEY  (`resource_id`),
  KEY `fk_stock_resource_id` (`resource_id`),
  CONSTRAINT `fk_stock_resource_id` FOREIGN KEY (`resource_id`) REFERENCES `resources` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `stock` */

/*Table structure for table `suppliers` */

DROP TABLE IF EXISTS `suppliers`;

CREATE TABLE `suppliers` (
  `id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `distance` float NOT NULL,
  `company_name` varchar(45) NOT NULL,
  PRIMARY KEY  (`id`,`user_id`),
  KEY `fk_supplier_supplier_user_id` (`user_id`),
  CONSTRAINT `suppliers_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

/*Data for the table `suppliers` */

insert  into `suppliers`(`id`,`user_id`,`distance`,`company_name`) values (1,19,10000,'SP_C(20)'),(3,29,280,'Osmos Inc.');

/*Table structure for table `suppliers_resources` */

DROP TABLE IF EXISTS `suppliers_resources`;

CREATE TABLE `suppliers_resources` (
  `supplier_id` int(11) NOT NULL,
  `resource_id` int(11) NOT NULL,
  `price` float unsigned NOT NULL,
  `number` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`supplier_id`,`resource_id`),
  KEY `fk_suppliers_resources_suppliers1` (`supplier_id`),
  KEY `fk_supplier_resource_resource_id` (`resource_id`),
  CONSTRAINT `fk_suppliers_resources_suppliers1` FOREIGN KEY (`supplier_id`) REFERENCES `suppliers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_supplier_resource_resource_id` FOREIGN KEY (`resource_id`) REFERENCES `resources` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `suppliers_resources` */

/*Table structure for table `users` */

DROP TABLE IF EXISTS `users`;

CREATE TABLE `users` (
  `id` int(11) NOT NULL auto_increment,
  `login` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `lastname` varchar(45) NOT NULL,
  `name` varchar(45) NOT NULL,
  `address` varchar(45) default NULL,
  `phone` varchar(45) default NULL,
  `email` varchar(45) default NULL,
  `type` enum('AUTHOR','CLIENT','SUPPLIER','CEO','OPERATOR','ADMIN') NOT NULL,
  `reg_date` datetime NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `login_unique` (`login`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;

/*Data for the table `users` */

insert  into `users`(`id`,`login`,`password`,`lastname`,`name`,`address`,`phone`,`email`,`type`,`reg_date`) values (5,'client','62608e08adc29a8d6dbc9754e659f125','lll','kjkljk','','','','CLIENT','2011-03-23 13:34:31'),(6,'author','02bd92faa38aaa6cc0ea75e59937a1ef','last','first','','','','AUTHOR','2011-03-23 13:46:29'),(19,'supplier','99b0e8da24e29e4ccb5d7d76e677c2ac','(Sklad #1)','Nikolay Petrovich','','','','SUPPLIER','2011-03-24 23:08:57'),(23,'operator','4b583376b2767b923c3e1da60d10de59','(Operator #1)','Ivan',NULL,NULL,NULL,'OPERATOR','2011-03-29 13:16:04'),(24,'admin','21232f297a57a5a743894a0e4a801fc3','lname','name',NULL,NULL,NULL,'ADMIN','2011-03-29 14:00:56'),(25,'manager','1d0258c2440a8d19e716292b231e3190','Trukhan','Svetlana',NULL,NULL,NULL,'CEO','2011-03-30 13:21:45'),(27,'author2','02bd92faa38aaa6cc0ea75e59937a1ef','Pis','David','','','','AUTHOR','2011-04-03 23:03:24'),(28,'author3','02bd92faa38aaa6cc0ea75e59937a1ef','Alexander','Dagan','','','','AUTHOR','2011-04-03 23:08:00'),(29,'supplier2','99b0e8da24e29e4ccb5d7d76e677c2ac','Denisenko','CH P','','','','SUPPLIER','2011-04-03 23:09:47'),(30,'author4','02bd92faa38aaa6cc0ea75e59937a1ef','Jackson','Natali','','','','AUTHOR','2011-04-03 23:21:36');

/* Trigger structure for table `authors_queries` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `changeBalance` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `changeBalance` AFTER INSERT ON `authors_queries` FOR EACH ROW BEGIN
    DECLARE resourcePrice FLOAT;
    DECLARE servicePrice FLOAT;
    DECLARE quot FLOAT;
    SET quot=(SELECT quot_author FROM limits WHERE id=3);
    SET resourcePrice = (SELECT (SELECT MAX(number)
                        FROM queries_resources JOIN resources
                        ON resource_id=id AND queries_resources.query_id=NEW.query_id)*SUM(price)
            FROM
                (SELECT resources.id, price
                 FROM resource_types JOIN resources
                    ON resource_type_id=resource_types.id
                        AND resource_types.title IN ('Paper', 'Print')
                ) titles
            JOIN queries_resources
                ON titles.id=resource_id
                    AND query_id=NEW.query_id);
    SET servicePrice=(SELECT SUM(price)
        FROM
            (SELECT price
            FROM
                (SELECT resources.id, price
                 FROM resource_types JOIN resources
                    ON resource_type_id=resource_types.id
                        AND resource_types.title=('Cover')
                ) covers
            JOIN queries_resources
                ON covers.id=resource_id
                    AND query_id=NEW.query_id
            UNION ALL
            SELECT price
            FROM services JOIN queries_services
                ON services.id=service_id
                    AND query_id=NEW.query_id) cov
        );
    SET servicePrice = servicePrice + quot*(resourcePrice+servicePrice);
    SET servicePrice = servicePrice +
        (SELECT sum FROM balance_log WHERE id=1);
    INSERT INTO balance_log VALUES ((SELECT COUNT(*)+1 FROM balance_log b),
        servicePrice,
        CURRENT_DATE);
END */$$


DELIMITER ;

/* Trigger structure for table `book_sell_log` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `book_sell_log_after_insert` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `book_sell_log_after_insert` AFTER INSERT ON `book_sell_log` FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance + new.sum*new.number), NOW());
    END */$$


DELIMITER ;

/* Trigger structure for table `buy_log` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `buy_log_after_insert` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `buy_log_after_insert` AFTER INSERT ON `buy_log` FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum*new.number), NOW());
	UPDATE garage SET amort_quot = amort_quot * 0.99 WHERE id = new.garage_id;
    END */$$


DELIMITER ;

/* Trigger structure for table `resource_sell_log` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `resource_sell_log_after_insert` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `resource_sell_log_after_insert` AFTER INSERT ON `resource_sell_log` FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance + new.sum*new.number), NOW());
    END */$$


DELIMITER ;

/* Trigger structure for table `salary_log` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `salary_log_after_insert` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `salary_log_after_insert` AFTER INSERT ON `salary_log` FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log WHERE id=(SELECT MAX(id) FROM balance_log));
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum), NOW());
    END */$$


DELIMITER ;

/* Trigger structure for table `service_sell_log` */

DELIMITER $$

/*!50003 DROP TRIGGER*//*!50032 IF EXISTS */ /*!50003 `service_sell_log_after_insert` */$$

/*!50003 CREATE */ /*!50017 DEFINER = 'root'@'localhost' */ /*!50003 TRIGGER `service_sell_log_after_insert` AFTER INSERT ON `service_sell_log` FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance + new.sum), NOW());
    END */$$


DELIMITER ;

/* Function  structure for function  `getBookCost` */

/*!50003 DROP FUNCTION IF EXISTS `getBookCost` */;
DELIMITER $$

/*!50003 CREATE DEFINER=`root`@`localhost` FUNCTION `getBookCost`(book_id VARCHAR (17)) RETURNS float
    DETERMINISTIC
BEGIN
  DECLARE cost INT ;
  SELECT
    SUM(qr.number * r.price) INTO cost
  FROM
    resources AS r
    INNER JOIN queries_resources AS qr
      ON qr.resource_id = r.id
    INNER JOIN queries AS q
      ON qr.query_id = q.id
  WHERE q.id = book_id
  GROUP BY q.id ;
  RETURN cost ;
END */$$
DELIMITER ;

/* Procedure structure for procedure `insertQuery` */

/*!50003 DROP PROCEDURE IF EXISTS  `insertQuery` */;

DELIMITER $$

/*!50003 CREATE DEFINER=`root`@`localhost` PROCEDURE `insertQuery`()
BEGIN
END */$$
DELIMITER ;

/* Procedure structure for procedure `modifyBalance` */

/*!50003 DROP PROCEDURE IF EXISTS  `modifyBalance` */;

DELIMITER $$

/*!50003 CREATE DEFINER=`root`@`localhost` PROCEDURE `modifyBalance`()
BEGIN
END */$$
DELIMITER ;

/* Procedure structure for procedure `paySalary` */

/*!50003 DROP PROCEDURE IF EXISTS  `paySalary` */;

DELIMITER $$

/*!50003 CREATE DEFINER=`root`@`localhost` PROCEDURE `paySalary`()
BEGIN
END */$$
DELIMITER ;

/*Table structure for table `allauthors` */

DROP TABLE IF EXISTS `allauthors`;

/*!50001 DROP VIEW IF EXISTS `allauthors` */;
/*!50001 DROP TABLE IF EXISTS `allauthors` */;

/*!50001 CREATE TABLE  `allauthors`(
 `id` int(11) ,
 `name` varchar(45) ,
 `lastname` varchar(45) 
)*/;

/*Table structure for table `allclients` */

DROP TABLE IF EXISTS `allclients`;

/*!50001 DROP VIEW IF EXISTS `allclients` */;
/*!50001 DROP TABLE IF EXISTS `allclients` */;

/*!50001 CREATE TABLE  `allclients`(
 `id` int(11) ,
 `company` varchar(45) 
)*/;

/*Table structure for table `allpersonnel` */

DROP TABLE IF EXISTS `allpersonnel`;

/*!50001 DROP VIEW IF EXISTS `allpersonnel` */;
/*!50001 DROP TABLE IF EXISTS `allpersonnel` */;

/*!50001 CREATE TABLE  `allpersonnel`(
 `id` int(11) ,
 `lastname` varchar(45) ,
 `name` varchar(45) ,
 `post` varchar(45) ,
 `rate` float unsigned ,
 `sum` double ,
 `date` date 
)*/;

/*Table structure for table `allsuppliers` */

DROP TABLE IF EXISTS `allsuppliers`;

/*!50001 DROP VIEW IF EXISTS `allsuppliers` */;
/*!50001 DROP TABLE IF EXISTS `allsuppliers` */;

/*!50001 CREATE TABLE  `allsuppliers`(
 `id` int(11) ,
 `company` varchar(45) 
)*/;

/*Table structure for table `covers` */

DROP TABLE IF EXISTS `covers`;

/*!50001 DROP VIEW IF EXISTS `covers` */;
/*!50001 DROP TABLE IF EXISTS `covers` */;

/*!50001 CREATE TABLE  `covers`(
 `title` varchar(45) 
)*/;

/*Table structure for table `genrequeries` */

DROP TABLE IF EXISTS `genrequeries`;

/*!50001 DROP VIEW IF EXISTS `genrequeries` */;
/*!50001 DROP TABLE IF EXISTS `genrequeries` */;

/*!50001 CREATE TABLE  `genrequeries`(
 `id` varchar(17) ,
 `genre` varchar(45) ,
 `title` varchar(45) ,
 `regdate` datetime 
)*/;

/*Table structure for table `op_resources_view` */

DROP TABLE IF EXISTS `op_resources_view`;

/*!50001 DROP VIEW IF EXISTS `op_resources_view` */;
/*!50001 DROP TABLE IF EXISTS `op_resources_view` */;

/*!50001 CREATE TABLE  `op_resources_view`(
 `id` int(11) ,
 `supplier_id` int(11) ,
 `TYPE` varchar(45) ,
 `title` varchar(45) ,
 `CONCAT(u.lastname, ' ', u.name)` varchar(91) ,
 `distance` float ,
 `number` int(10) unsigned ,
 `price` float unsigned ,
 `0` int(1) 
)*/;

/*Table structure for table `papers` */

DROP TABLE IF EXISTS `papers`;

/*!50001 DROP VIEW IF EXISTS `papers` */;
/*!50001 DROP TABLE IF EXISTS `papers` */;

/*!50001 CREATE TABLE  `papers`(
 `title` varchar(45) 
)*/;

/*Table structure for table `prints` */

DROP TABLE IF EXISTS `prints`;

/*!50001 DROP VIEW IF EXISTS `prints` */;
/*!50001 DROP TABLE IF EXISTS `prints` */;

/*!50001 CREATE TABLE  `prints`(
 `title` varchar(45) 
)*/;

/*Table structure for table `queries_view` */

DROP TABLE IF EXISTS `queries_view`;

/*!50001 DROP VIEW IF EXISTS `queries_view` */;
/*!50001 DROP TABLE IF EXISTS `queries_view` */;

/*!50001 CREATE TABLE  `queries_view`(
 `id` varchar(17) ,
 `title` varchar(45) ,
 `authors` varchar(341) ,
 `genre` varchar(45) ,
 `price` double ,
 `count` int(1) 
)*/;

/*Table structure for table `resources_view` */

DROP TABLE IF EXISTS `resources_view`;

/*!50001 DROP VIEW IF EXISTS `resources_view` */;
/*!50001 DROP TABLE IF EXISTS `resources_view` */;

/*!50001 CREATE TABLE  `resources_view`(
 `id` int(11) ,
 `type` varchar(45) ,
 `title` varchar(45) ,
 `number` int(11) ,
 `price` float unsigned ,
 `0` int(1) 
)*/;

/*Table structure for table `services_view` */

DROP TABLE IF EXISTS `services_view`;

/*!50001 DROP VIEW IF EXISTS `services_view` */;
/*!50001 DROP TABLE IF EXISTS `services_view` */;

/*!50001 CREATE TABLE  `services_view`(
 `id` int(11) ,
 `title` varchar(45) ,
 `price` float unsigned 
)*/;

/*Table structure for table `suppliers_resources_view` */

DROP TABLE IF EXISTS `suppliers_resources_view`;

/*!50001 DROP VIEW IF EXISTS `suppliers_resources_view` */;
/*!50001 DROP TABLE IF EXISTS `suppliers_resources_view` */;

/*!50001 CREATE TABLE  `suppliers_resources_view`(
 `type` varchar(45) ,
 `title` varchar(45) ,
 `price` float unsigned ,
 `number` int(10) unsigned ,
 `res_id` int(11) ,
 `supplier_id` int(11) 
)*/;

/*View structure for view allauthors */

/*!50001 DROP TABLE IF EXISTS `allauthors` */;
/*!50001 DROP VIEW IF EXISTS `allauthors` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `allauthors` AS select `authors`.`id` AS `id`,`users`.`name` AS `name`,`users`.`lastname` AS `lastname` from (`authors` join `users` on((`authors`.`user_id` = `users`.`id`))) */;

/*View structure for view allclients */

/*!50001 DROP TABLE IF EXISTS `allclients` */;
/*!50001 DROP VIEW IF EXISTS `allclients` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `allclients` AS select `clients`.`id` AS `id`,`clients`.`company_name` AS `company` from (`clients` join `users` on((`clients`.`user_id` = `users`.`id`))) */;

/*View structure for view allpersonnel */

/*!50001 DROP TABLE IF EXISTS `allpersonnel` */;
/*!50001 DROP VIEW IF EXISTS `allpersonnel` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `allpersonnel` AS select `personnel`.`id` AS `id`,`personnel`.`lastname` AS `lastname`,`personnel`.`name` AS `name`,`posts`.`title` AS `post`,`posts`.`rate` AS `rate`,(`posts`.`rate` * `personnel`.`salary_quot`) AS `sum`,`personnel`.`assign_date` AS `date` from (`personnel` join `posts` on((`personnel`.`post_id` = `posts`.`id`))) */;

/*View structure for view allsuppliers */

/*!50001 DROP TABLE IF EXISTS `allsuppliers` */;
/*!50001 DROP VIEW IF EXISTS `allsuppliers` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `allsuppliers` AS select `suppliers`.`id` AS `id`,`suppliers`.`company_name` AS `company` from (`suppliers` join `users` on((`suppliers`.`user_id` = `users`.`id`))) */;

/*View structure for view covers */

/*!50001 DROP TABLE IF EXISTS `covers` */;
/*!50001 DROP VIEW IF EXISTS `covers` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `covers` AS select `resources`.`title` AS `title` from (`resource_types` join `resources` on(((`resources`.`resource_type_id` = `resource_types`.`id`) and (`resource_types`.`title` = _utf8'Cover')))) */;

/*View structure for view genrequeries */

/*!50001 DROP TABLE IF EXISTS `genrequeries` */;
/*!50001 DROP VIEW IF EXISTS `genrequeries` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `genrequeries` AS select `queries`.`id` AS `id`,`genres`.`title` AS `genre`,`queries`.`title` AS `title`,`queries`.`regdate` AS `regdate` from (`queries` join `genres` on((`queries`.`genre_id` = `genres`.`id`))) */;

/*View structure for view op_resources_view */

/*!50001 DROP TABLE IF EXISTS `op_resources_view` */;
/*!50001 DROP VIEW IF EXISTS `op_resources_view` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `op_resources_view` AS select `r`.`id` AS `id`,`s`.`id` AS `supplier_id`,`rt`.`title` AS `TYPE`,`r`.`title` AS `title`,concat(`u`.`lastname`,_utf8' ',`u`.`name`) AS `CONCAT(u.lastname, ' ', u.name)`,`s`.`distance` AS `distance`,`sr`.`number` AS `number`,`sr`.`price` AS `price`,0 AS `0` from ((((`resources` `r` join `suppliers_resources` `sr` on((`sr`.`resource_id` = `r`.`id`))) join `suppliers` `s` on((`s`.`id` = `sr`.`supplier_id`))) join `users` `u` on((`u`.`id` = `s`.`user_id`))) join `resource_types` `rt` on((`rt`.`id` = `r`.`resource_type_id`))) */;

/*View structure for view papers */

/*!50001 DROP TABLE IF EXISTS `papers` */;
/*!50001 DROP VIEW IF EXISTS `papers` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `papers` AS select `resources`.`title` AS `title` from (`resource_types` join `resources` on(((`resources`.`resource_type_id` = `resource_types`.`id`) and (`resource_types`.`title` = _utf8'Paper')))) */;

/*View structure for view prints */

/*!50001 DROP TABLE IF EXISTS `prints` */;
/*!50001 DROP VIEW IF EXISTS `prints` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `prints` AS select `resources`.`title` AS `title` from (`resource_types` join `resources` on(((`resources`.`resource_type_id` = `resource_types`.`id`) and (`resource_types`.`title` = _utf8'Print')))) */;

/*View structure for view queries_view */

/*!50001 DROP TABLE IF EXISTS `queries_view` */;
/*!50001 DROP VIEW IF EXISTS `queries_view` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `queries_view` AS select `q`.`id` AS `id`,`q`.`title` AS `title`,group_concat(distinct concat(`u`.`lastname`,_utf8' ',`u`.`name`) order by `u`.`lastname` ASC separator ', ') AS `authors`,`g`.`title` AS `genre`,`getBookCost`(`q`.`id`) AS `price`,0 AS `count` from ((((`queries` `q` join `genres` `g` on((`g`.`id` = `q`.`genre_id`))) join `authors_queries` `aq` on((`q`.`id` = `aq`.`query_id`))) join `authors` `a` on((`aq`.`author_id` = `a`.`id`))) join `users` `u` on((`u`.`id` = `a`.`user_id`))) group by `q`.`id` */;

/*View structure for view resources_view */

/*!50001 DROP TABLE IF EXISTS `resources_view` */;
/*!50001 DROP VIEW IF EXISTS `resources_view` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `resources_view` AS select `r`.`id` AS `id`,`rt`.`title` AS `type`,`r`.`title` AS `title`,`s`.`number` AS `number`,`r`.`price` AS `price`,0 AS `0` from ((`resources` `r` join `stock` `s` on((`s`.`resource_id` = `r`.`id`))) join `resource_types` `rt` on((`rt`.`id` = `r`.`resource_type_id`))) */;

/*View structure for view services_view */

/*!50001 DROP TABLE IF EXISTS `services_view` */;
/*!50001 DROP VIEW IF EXISTS `services_view` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `services_view` AS select `s`.`id` AS `id`,`s`.`title` AS `title`,`s`.`price` AS `price` from ((`services` `s` join `personnel_services` `ps` on((`ps`.`service_id` = `s`.`id`))) join `personnel` `p` on((`ps`.`personnel_id` = `p`.`id`))) */;

/*View structure for view suppliers_resources_view */

/*!50001 DROP TABLE IF EXISTS `suppliers_resources_view` */;
/*!50001 DROP VIEW IF EXISTS `suppliers_resources_view` */;

/*!50001 CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `suppliers_resources_view` AS select `rt`.`title` AS `type`,`r`.`title` AS `title`,`sr`.`price` AS `price`,`sr`.`number` AS `number`,`r`.`id` AS `res_id`,`sr`.`supplier_id` AS `supplier_id` from ((`suppliers_resources` `sr` join `resources` `r` on((`sr`.`resource_id` = `r`.`id`))) join `resource_types` `rt` on((`r`.`resource_type_id` = `rt`.`id`))) order by `rt`.`title`,`r`.`title` */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
