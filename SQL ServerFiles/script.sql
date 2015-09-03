-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema TSMARTPad
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema TSMARTPad
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `TSMARTPad` DEFAULT CHARACTER SET latin1 ;
USE `TSMARTPad` ;

-- -----------------------------------------------------
-- Table `TSMARTPad`.`readings`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `TSMARTPad`.`readings` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '',
  `sensor` VARCHAR(255) NOT NULL COMMENT '',
  `time_stamp` INT(10) UNSIGNED NOT NULL COMMENT '',
  `value` VARCHAR(255) NOT NULL COMMENT '',
  `stddev` DECIMAL NULL COMMENT '',
  `samplesize` VARCHAR(45) NOT NULL COMMENT '',
  PRIMARY KEY (`id`)  COMMENT '')
ENGINE = InnoDB
AUTO_INCREMENT = 875
DEFAULT CHARACTER SET = latin1
INSERT_METHOD = FIRST;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
