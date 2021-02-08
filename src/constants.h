/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * constants.h: Part of sspender
 *
 * sspender is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * sspender is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with sspender.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

using namespace std;

enum SLEEP_MODE {STAND_BY, MEM, DISK};

const int STORAGE_READ_WRITE_LIMIT    = 400;    //#KB/s across all drives
const int MONITORING_THREAD_FREQUENCY = 5000;   //The frequency at which the threads will monitor usage (ms)

const int DISK_STATS_LINE_INDEX = 0;
const int DISK_LINE_NUM_OF_STATS = 11;
const int NUM_OF_CPU_WORK_JIFFIES = 3;
const string PING_IDENTIFIER = "from";    //string to look for in ping output if client is connected
const string CONFIG_DELIMITER = ",";      //delimiter used in the config file

//default settings
const int SUSPEND_AFTER       = 240;         //suspend system if idle for more than (x minute)
const int CHECK_IF_IDLE_EVERY = 10;          //check if system is idle every (x minute)
const int STOP_MONITORING_FOR = 20;          //stop monitoring system for (x minute)
const int DISK_IDLE_LOAD_THRESHOLD = 6;      //Idle load threshold (y%)
const int DISK_IDLE_TIME_THRESHOLD = 120;    //Idle time threshold (x min under y%)
const int CPU_IDLE_LOAD_THRESHOLD = 6;       //Idle load threshold (y%)
const int CPU_IDLE_TIME_THRESHOLD = 10;      //Idle time threshold (x min under y%)
const bool SPIN_DOWN_DISK_IF_IDLE  = true; //spin down the disks when idle
const string  DEFAULT_SLEEP_MODE   = "mem"; //Default sleep mode

#endif
