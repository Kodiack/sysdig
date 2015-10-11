--[[
Copyright (C) 2013-2015 Draios inc.
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
--]]

view_info = 
{
	id = "procs_errors",
	name = "Processes Errors",
	description = "This view shows system error counters for processes. Errors are grouped into 4 categories: file I/O, network I/O, memory allocation and 'other'.",
	tips = {
		"If you click 'enter' on a selection in this chart, you will be able to see the specific errors that the process is generating.",
		"Digging into a process by clicking on F6 will let you explore the system calls for that specific process and see the full details about what's causing the errors."
	},
	tags = {"Default"},
	filter = "evt.type!=switch",
	view_type = "table",
	applies_to = {"", "container.id", "fd.name", "fd.sport", "evt.type", "fd.directory"},
	drilldown_target = "errors",
	use_defaults = true,
	columns = 
	{
		{
			name = "NA",
			field = "proc.pid",
			is_key = true
		},
		{
			name = "FILE",
			field = "evt.count.error.file",
			description = "Number of file I/O errors generated by the process during the sample interval. On trace files, this is the total for the whole file.",
			colsize = 8,
			aggregation = "SUM"
		},
		{
			name = "NET",
			field = "evt.count.error.net",
			description = "Number of network I/O errors generated by the process during the sample interval. On trace files, this is the total for the whole file.",
			colsize = 8,
			aggregation = "SUM"
		},
		{
			name = "MEMORY",
			field = "evt.count.error.memory",
			description = "Number of memory allocation/release related errors generated by the process during the sample interval. On trace files, this is the total for the whole file.",
			colsize = 8,
			aggregation = "SUM"
		},
		{
			name = "OTHER",
			field = "evt.count.error.other",
			description = "Number of errors generated by the process that don't fall in any of the previous categories. E.g. signal or event related errors. On trace files, this is the total for the whole file.",
			colsize = 8,
			aggregation = "SUM"
		},
		{
			name = "PID",
			description = "Process PID.",
			field = "proc.pid",
			colsize = 8,
		},
		{
			tags = {"containers"},
			name = "Container",
			field = "container.name",
			description = "Name of the container. What this field contains depends on the containerization technology. For example, for docker this is the content of the 'NAMES' column in 'docker ps'",
			colsize = 20
		},
		{
			name = "Command",
			description = "Full command line of the process.",
			field = "proc.exeline",
			aggregation = "MAX",
			colsize = 0
		}
	}
}
