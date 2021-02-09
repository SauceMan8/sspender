/*
 * Copyright (C) 2016 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Monitor.cpp: Part of sspender
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

#include "Monitor.h"

Monitor::Monitor()
{

}

Monitor::~Monitor()
{
	if(m_disksToMonitor.size() > 0)
	{
		delete[] &m_disksToMonitor;
	}

	if(m_cpusToMonitor.size() > 0)
	{
		delete[] &m_cpusToMonitor;
	}
}

void Monitor::monitorSystemUsage(const vector<DiskCfg> &disks,
		                             const vector<CpuCfg> &cpus)
{
	for(size_t i = 0, len = disks.size(); i < len; ++i)
	{
		Disk *newDisk= new Disk(disks[i].diskName,
								disks[i].diskUUID,
								disks[i].disk_idle_time_threshold,
								disks[i].disk_idle_load_threshold,
								disks[i].spinDown);

		newDisk->monitorUsage();

		m_disksToMonitor.push_back(newDisk);
	}

	for(size_t i = 0, len = cpus.size(); i < len; ++i)
	{
		Cpu *newCpu= new Cpu(cpus[i].cpuName,
				             cpus[i].cpu_idle_time_threshold,
							 cpus[i].cpu_idle_load_threshold);

		newCpu->monitorUsage();

		m_cpusToMonitor.push_back(newCpu);
	}
}


void Monitor::printTheMachineUsage()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		cout << *(m_cpusToMonitor[i]) << endl;
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		cout << *(m_disksToMonitor[i]) << endl;
	}
}

void Monitor::resetTheMachine()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		m_cpusToMonitor[i]->resetUsage();
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		m_disksToMonitor[i]->resetUsage();
	}
}

bool Monitor::isTheMachineIdle()
{
	for(size_t i = 0, len = m_cpusToMonitor.size(); i < len; ++i)
	{
		if(m_cpusToMonitor[i]->getIdleState() == false)
		{
			return false;
		}
	}

	for(size_t i = 0, len = m_disksToMonitor.size(); i < len; ++i)
	{
		if(m_disksToMonitor[i]->getIdleState() == false)
		{
			return false;
		}
	}

	return true;
}

bool Monitor::areClientsConnected(const vector<string> &clients)
{
	bool isAnyClientOnline = false;

	if(clients.size() > 0){
		printHeaderMessage("Checking if clients are online", true);
		
		for(size_t i = 0, len = clients.size(); i < len; ++i)
		{
			if(isIpAddressOnline(clients[i]))
			{
				cout << "IP " << clients[i] << " is online." << endl;

				isAnyClientOnline = true;
			}
		}
	}
	return isAnyClientOnline;
}

bool Monitor::isIpAddressOnline(const string &ipAddress)
{
	vector<string> pingOutput;
	runSystemCommand("ping -c 1 -W 1 " + ipAddress, &pingOutput);

	for(size_t i = 0, len = pingOutput.size(); i < len; ++i)
	{
		size_t found = pingOutput[i].find(PING_IDENTIFIER, 0);

		if (found != string::npos)
		{
			cout << ipAddress << " is online." << endl;
			return true;
		}
	}

	cout << ipAddress << " is not online." << endl;

	return false;
}
