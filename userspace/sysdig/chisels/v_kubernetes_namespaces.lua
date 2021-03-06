--[[
Copyright (C) 2013-2014 Draios inc.
 
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
	id = "kubernetes_namespaces",
	name = "K8s Namespaces",
	description = "List all Kubernetes namespaces running on this machine, and the resources that each of them uses.",
	tips = {"Select a namespace and click enter to drill down into it. At that point, you will be able to access several views that will show you the details of the selected namespace."},
	view_type = "table",
	applies_to = {"", "evt.res", "k8s.pod.id", "k8s.rc.id", "k8s.svc.id"},
	filter = "k8s.ns.id != ''",
	use_defaults = true,
	drilldown_target = "kubernetes_pods",
	columns = 
	{
		{
			name = "NA",
			field = "thread.tid",
			is_key = true
		},
		{
			name = "CPU",
			field = "thread.cpu",
			description = "Amount of CPU used by the namespace.",
			colsize = 8,
			aggregation = "AVG",
			groupby_aggregation = "SUM",
			is_sorting = true
		},
		{
			name = "VIRT",
			field = "thread.vmsize.b",
			description = "total virtual memory for the namespace (as kb).",
			aggregation = "MAX",
			groupby_aggregation = "SUM",
			colsize = 9
		},
		{
			name = "RES",
			field = "thread.vmrss.b",
			description = "resident non-swapped memory for the namespace (as kb).",
			aggregation = "MAX",
			groupby_aggregation = "SUM",
			colsize = 9
		},
		{
			name = "FILE",
			field = "evt.buflen.file",
			description = "Total (input+output) file I/O bandwidth generated by the namespace, in bytes per second.",
			colsize = 8,
			aggregation = "TIME_AVG",
			groupby_aggregation = "SUM"
		},
		{
			name = "NET",
			field = "evt.buflen.net",
			description = "Total (input+output) network bandwidth generated by the namespace, in bytes per second.",
			colsize = 8,
			aggregation = "TIME_AVG",
			groupby_aggregation = "SUM"
		},
		{
			name = "NA",
			field = "k8s.ns.id",
			is_groupby_key = true
		},
		{
			name = "ID",
			field = "k8s.ns.id",
			description = "Namespace id.",
			colsize = 38
		},
		{
			name = "NAME",
			field = "k8s.ns.name",
			description = "Namespace name.",
			colsize = 25
		},
		{
			name = "LABELS",
			field = "k8s.ns.labels",
			description = "Namespace labels.",
			colsize = 0
		},
	},
	actions = 
	{
		{
			hotkey = "d",
			command = "kubectl --namespace=%k8s.ns.name describe namespaces %k8s.ns.name",
			description = "describe"
		},
		{
			hotkey = "x",
			command = "kubectl --namespace=%k8s.ns.name delete namespaces %k8s.ns.name",
			description = "delete"
		}
	}
}
