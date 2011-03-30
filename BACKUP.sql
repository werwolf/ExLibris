/* ZERO LEVEL */
alter table `exlibris`.`amort_log` 
   change `sum` `sum` float(10,2) UNSIGNED ZEROFILL NOT NULL;
   
INSERT INTO balance_log(`sum`, `date`) VALUES('10000', DATE_SUB(NOW(), INTERVAL 1 DAY));






/* FUNCTIONS */
DELIMITER $$

CREATE FUNCTION `exlibris`.`getBookCost` (book_id VARCHAR (17)) RETURNS FLOAT DETERMINISTIC 
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
END $$

DELIMITER ;






/* PROCEDURES (not used, but it's so cool) */
DELIMITER $$

USE `exlibris`$$

DROP PROCEDURE IF EXISTS `getBookCost`$$

CREATE DEFINER = `root` @`localhost` PROCEDURE `getBookCost` (IN id VARCHAR (17)) READS SQL DATA COMMENT 'Calculates total cost of the specified book by given query_id' 
BEGIN
  SET @s = CONCAT(
    "SELECT 
		  SUM(qr.number * r.price) AS price 
		FROM
		  resources AS r 
		  INNER JOIN queries_resources AS qr 
		    ON qr.resource_id = r.id 
		  INNER JOIN queries AS q 
		    ON qr.query_id = q.id 
		WHERE q.id = '",
    id,
    "' 
		GROUP BY q.id"
  ) ;
  PREPARE smth FROM @s ;
  EXECUTE smth ;
END $$



DELIMITER ;







/* VIEWS */
DELIMITER $$

USE `exlibris`$$

DROP VIEW IF EXISTS `queries_view`$$

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `queries_view` AS 
SELECT
  `q`.`id`    AS `id`,
  `q`.`title` AS `title`,
  GROUP_CONCAT(DISTINCT CONCAT(`u`.`lastname`,_utf8' ',`u`.`name`) ORDER BY `u`.`lastname` ASC SEPARATOR ', ') AS `authors`,
  `g`.`title` AS `genre`,
  `getBookCost`(
`q`.`id`)  AS `price`,
  0           AS `count`
FROM ((((`queries` `q`
      JOIN `genres` `g`
        ON ((`g`.`id` = `q`.`genre_id`)))
     JOIN `authors_queries` `aq`
       ON ((`q`.`id` = `aq`.`query_id`)))
    JOIN `authors` `a`
      ON ((`aq`.`author_id` = `a`.`id`)))
   JOIN `users` `u`
     ON ((`u`.`id` = `a`.`user_id`)))
GROUP BY `q`.`id`$$


DROP VIEW IF EXISTS `resources_view`$$

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `resources_view` AS 
SELECT
  `r`.`id`     AS `id`,
  `rt`.`title` AS `type`,
  `r`.`title`  AS `title`,
  `s`.`number` AS `number`,
  `r`.`price`  AS `price`,
  0            AS `0`
FROM ((`resources` `r`
    JOIN `stock` `s`
      ON ((`s`.`resource_id` = `r`.`id`)))
   JOIN `resource_types` `rt`
     ON ((`rt`.`id` = `r`.`resource_type_id`)))$$
     
     
DROP VIEW IF EXISTS `services_view`$$

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `services_view` AS 
SELECT
  `s`.`id`    AS `id`,
  `s`.`title` AS `title`,
  `s`.`price` AS `price`
FROM ((`services` `s`
    JOIN `personnel_services` `ps`
      ON ((`ps`.`service_id` = `s`.`id`)))
   JOIN `personnel` `p`
     ON ((`ps`.`personnel_id` = `p`.`id`)))$$
     
     
DROP VIEW IF EXISTS `op_resources_view`$$

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `op_resources_view` AS 
SELECT
  `r`.`id`       AS `id`,
  `s`.`id`       AS `supplier_id`,
  `rt`.`title`   AS `TYPE`,
  `r`.`title`    AS `title`,
  CONCAT(`u`.`lastname`,_utf8' ',`u`.`name`) AS `CONCAT(u.lastname, ' ', u.name)`,
  `s`.`distance` AS `distance`,
  `sr`.`number`  AS `number`,
  `sr`.`price`   AS `price`,
  0              AS `0`
FROM ((((`resources` `r`
      JOIN `suppliers_resources` `sr`
        ON ((`sr`.`resource_id` = `r`.`id`)))
     JOIN `suppliers` `s`
       ON ((`s`.`id` = `sr`.`supplier_id`)))
    JOIN `users` `u`
      ON ((`u`.`id` = `s`.`user_id`)))
   JOIN `resource_types` `rt`
     ON ((`rt`.`id` = `r`.`resource_type_id`)))$$

DELIMITER ;


/*
CREATE VIEW queries_view AS 
SELECT 
  q.id,
  q.title,
  GROUP_CONCAT(
    DISTINCT CONCAT(u.lastname, ' ', u.name) 
    ORDER BY u.lastname SEPARATOR ', '
  ) as authors,
  g.title AS genre,
  getBookCost(q.id) as price,
  0 as count
FROM
  queries AS q 
  INNER JOIN genres AS g 
    ON g.id = q.genre_id 
  INNER JOIN authors_queries AS aq 
    ON q.id = aq.query_id 
  INNER JOIN AUTHORS AS a 
    ON aq.author_id = a.id 
  INNER JOIN users AS u 
    ON u.id = a.user_id 
GROUP BY q.id 


CREATE VIEW resources_view AS 
SELECT 
  r.id,
  rt.title AS TYPE,
  r.title,
  s.number,
  r.price,
  0 
FROM
  resources AS r 
  INNER JOIN stock AS s 
    ON s.resource_id = r.id 
  INNER JOIN resource_types AS rt 
    ON rt.id = r.resource_type_id 


CREATE VIEW services_view AS 
SELECT 
  s.id,
  s.title,
  s.price 
FROM
  services AS s 
  INNER JOIN personnel_services AS ps 
    ON ps.service_id = s.id 
  INNER JOIN personnel AS p 
    ON ps.personnel_id = p.id 
    
    
CREATE VIEW `op_resources_view` AS 
SELECT 
  r.id,
  s.id AS supplier_id,
  rt.title AS TYPE,
  r.title,
  CONCAT(u.lastname, ' ', u.name),
  s.distance,
  sr.number,
  sr.price,
  0 
FROM
  resources AS r 
  INNER JOIN suppliers_resources AS sr 
    ON sr.resource_id = r.id 
  INNER JOIN suppliers AS s 
    ON s.id = sr.supplier_id 
  INNER JOIN users AS u 
    ON u.id = s.user_id 
  INNER JOIN resource_types AS rt 
    ON rt.id = r.resource_type_id 
*/    






/* TRIGGERS */
DELIMITER $$

USE `exlibris`$$

DROP TRIGGER /*!50032 IF EXISTS */ `book_sell_log_after_insert`$$

CREATE
    /*!50017 DEFINER = 'root'@'localhost' */
    TRIGGER `book_sell_log_after_insert` AFTER INSERT ON `book_sell_log` 
    FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum*new.number), NOW());
    END;
$$


DROP TRIGGER /*!50032 IF EXISTS */ `resource_sell_log_after_insert`$$

CREATE
    /*!50017 DEFINER = 'root'@'localhost' */
    TRIGGER `resource_sell_log_after_insert` AFTER INSERT ON `resource_sell_log` 
    FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum*new.number), NOW());
    END;
$$


DROP TRIGGER /*!50032 IF EXISTS */ `service_sell_log_after_insert`$$

CREATE
    /*!50017 DEFINER = 'root'@'localhost' */
    TRIGGER `service_sell_log_after_insert` AFTER INSERT ON `service_sell_log` 
    FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum), NOW());
    END;
$$


DROP TRIGGER /*!50032 IF EXISTS */ `buy_log_after_insert`$$

CREATE
    /*!50017 DEFINER = 'root'@'localhost' */
    TRIGGER `buy_log_after_insert` AFTER INSERT ON `buy_log` 
    FOR EACH ROW BEGIN
    SET @old_balance = (SELECT SUM FROM balance_log ORDER BY DATE DESC LIMIT 1);
	INSERT INTO balance_log(SUM, DATE) VALUES((@old_balance - new.sum*new.number), NOW());
	UPDATE garage SET amort_quot = amort_quot * 0.99 WHERE id = new.garage_id;
    END;
$$

DELIMITER ;


