-- Active: 1700846483389@@sql3.freemysqlhosting.net@3306@sql3664795

CREATE DATABASE  IF NOT EXISTS `sql3664795`;
USE `sql3664795`;


DROP TABLE IF EXISTS `dht_sensor_data`;

CREATE TABLE `dht_sensor_data` (
  `id` int NOT NULL AUTO_INCREMENT,
  `gas` float DEFAULT NULL,
  `temperature` float DEFAULT NULL,
  `humidity` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3;
