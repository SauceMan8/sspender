/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Manager.cpp: Part of sspender
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

#include "Manager.h"

void Manager::setIpsToWatch(const vector<string> &ipToWatch)
{
	for(size_t i = 0, size = ipToWatch.size(); i < size; ++i)
	{
		m_ipsToWatch.push_back(ipToWatch[i]);
	}
}

void Manager::setDisksToMonitor(const vector<DiskCfg> &disksToMonitor)
{
	for(size_t i = 0, size = disksToMonitor.size(); i < size; ++i)
	{
		m_disksToMonitor.push_back(disksToMonitor[i]);
	}
}

void Manager::setCpusToMonitor(const CpuCfg &couConfig)
{
	m_cpusToMonitor.push_back(couConfig);
}

void Manager::setSleepMode(SLEEP_MODE sleepMode)
{
	m_sleepMode = sleepMode;
}

void Manager::setTimers(int check_if_idle_every,
				   	    int stop_monitoring_for,
				   	    int suspend_after)
{
	m_checkIfIdleEvery     = check_if_idle_every;
	m_stopMonitoringFor    = stop_monitoring_for;
	m_suspendAfter         = suspend_after;
}

void Manager::monitorSystemUsage()
{
	//start monitoring the usage of the passed in devices
	m_monitor.monitorSystemUsage(m_disksToMonitor, m_cpusToMonitor);

	auto idleStartTime = Clock::now();
	auto notIdleStartTime = Clock::now();

	while(true)
	{
		//check if the machine is idle every # minutes
		std::this_thread::sleep_for(std::chrono::minutes(m_checkIfIdleEvery));
		
		printHeaderMessage("Checking if clients are online", true);

		bool stayOnline = m_monitor.areClientsConnected(m_ipsToWatch);

		if(stayOnline)
		{
			cout << "Found clients online, will stop monitoring for "
				 << m_stopMonitoringFor
				 << " mins." << endl;

			//if any of the specified IPs is online, reset the counters and
			//stop checking if the machine is idle, note that the usage of
			//the devices will still be monitored by the detached threads
			std::this_thread::sleep_for(std::chrono::minutes(m_stopMonitoringFor));

			idleStartTime = Clock::now();
			notIdleStartTime = Clock::now();

			continue;
		}

		printHeaderMessage("Checking if system is idle", true);

		bool isIdle = isTheMachineIdle();

		double minutesTheMachineBeenBusyFor = getMinutesDuration(notIdleStartTime);
		double minutesTheMachineBeenIdleFor = getMinutesDuration(idleStartTime);

		if(isIdle)
		{
			cout << "System is idle (" << minutesTheMachineBeenIdleFor << " minutes).\n";

			notIdleStartTime = Clock::now();
		}
		else
		{
			cout << "System is not idle (" << minutesTheMachineBeenBusyFor << " minutes).\n";
		}

		printTheMachineUsage();

		if(isIdle && m_suspendAfter > 0)
		{
			//if idle for # minutes
			if(minutesTheMachineBeenIdleFor >= m_suspendAfter)
			{
				cout << "System was idle for more than " << m_suspendAfter << " mins, will suspend the machine.\n";

				printHeaderMessage("Suspending the machine", true);

				suspendTheMachine();

				printHeaderMessage("Waking up", true);

				resetTheMachine();
				
				idleStartTime = Clock::now();
				notIdleStartTime = Clock::now();
			}
		}
	}
}


void Manager::printTheMachineUsage()
{
	m_monitor.printTheMachineUsage();
}

bool Manager::isTheMachineIdle()
{
	return m_monitor.isTheMachineIdle();
}

void Manager::resetTheMachine()
{
	m_monitor.resetTheMachine();
}


void Manager::suspendTheMachine()
{
	vector<string> output;

	pmUtilSuspend(&output);

	printHeaderMessage("System returned from suspend", true);
	cout << "Suspend output: ";

	for(size_t i = 0, len = output.size(); i < len; ++i)
	{
		cout << output[i] << " , ";
	}

	cout << endl;
}


void Manager::pmUtilSuspend(vector<string> *output)
{
	//After setting the time, PC can be turned off with this command
	runSystemCommand(getPmUtilCommand(), output);
}


string Manager::getPmUtilCommand()
{
	switch(m_sleepMode)
	{
		case STAND_BY: { return string("/usr/sbin/pm-suspend");}
		case MEM:      { return string("/usr/sbin/pm-suspend");}
		case DISK:     { return string("/usr/sbin/pm-hibernate");}
		default:       { return string("/usr/sbin/pm-suspend");}
	}
}
