/*
 Navicat Premium Data Transfer

 Source Server         : localhost
 Source Server Type    : MySQL
 Source Server Version : 100414
 Source Host           : localhost:3306
 Source Schema         : player

 Target Server Type    : MySQL
 Target Server Version : 100414
 File Encoding         : 65001

 Date: 00/00/0000 00:00:00
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for event
-- ----------------------------
DROP TABLE IF EXISTS `event`;
CREATE TABLE `event`  (
  `id` int(5) NOT NULL AUTO_INCREMENT,
  `type` enum('EVENT_TYPE_NONE','EVENT_TYPE_EXPERIENCE','EVENT_TYPE_ITEM_DROP','EVENT_TYPE_BOSS','EVENT_TYPE_METIN','EVENT_TYPE_MINING','EVENT_TYPE_GOLD_FROG','EVENT_TYPE_MOONLIGHT','EVENT_TYPE_HEXEGONAL_CHEST','EVENT_TYPE_FISHING','EVENT_TYPE_HIDE_AND_SEEK','EVENT_TYPE_OX','EVENT_TYPE_TANAKA') CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT 'EVENT_TYPE_NONE',
  `start` datetime(0) NOT NULL,
  `end` datetime(0) NOT NULL,
  `value0` int(4) DEFAULT 0,
  `value1` int(4) DEFAULT 0,
  `completed` tinyint(1) DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
