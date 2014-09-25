/*
Copyright (C) 2013-2014 Draios inc.

This file is part of sysdig.

sysdig is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

sysdig is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sysdig.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef VISIBILITY_PRIVATE
#define VISIBILITY_PRIVATE private:
#endif

class sinsp_delays_info;
class sinsp_threadtable_listener;
class thread_analyzer_info;

typedef struct erase_fd_params
{
	bool m_remove_from_table;
	sinsp* m_inspector;
	int64_t m_fd;
	sinsp_threadinfo* m_tinfo;
	sinsp_fdinfo_t* m_fdinfo;
	uint64_t m_ts;
}erase_fd_params;

/** @defgroup state State management 
 *  @{
 */

/*!
  \brief Thread/process information class.
  This class contains the full state for a thread, and a bunch of functions to
  manipulate threads and retrieve thread information.

  \note As a library user, you won't need to construct thread objects. Rather,
   you get them by calling \ref sinsp_evt::get_thread_info or 
   \ref sinsp::get_thread.
  \note sinsp_threadinfo is also used to keep process state. For the sinsp
   library, a process is just a thread with TID=PID.
*/
class SINSP_PUBLIC sinsp_threadinfo
{
public:
	sinsp_threadinfo();
	sinsp_threadinfo(sinsp *inspector);
	~sinsp_threadinfo();

	/*!
	  \brief Return the name of the process containing this thread, e.g. "top".
	*/
	string get_comm();

	/*!
	  \brief Return the full name of the process containing this thread, e.g. "/bin/top".
	*/
	string get_exe();

	/*!
	  \brief Return the working directory of the process containing this thread.
	*/
	string get_cwd();

	/*!
	  \brief Return true if this is a process' main thread.
	*/
	bool is_main_thread();

	/*!
	  \brief Get the main thread of the process containing this thread.
	*/
	sinsp_threadinfo* get_main_thread();

	/*!
	  \brief Get the process that launched this thread's process.
	*/
	sinsp_threadinfo* get_parent_thread();

	/*!
	  \brief Retrive information about one of this thread/process FDs.

	  \param fd The file descriptor number, e.g. 0 for stdin.

	  \return Pointer to the FD information, or NULL if the given FD doesn't
	   exist
	*/
	sinsp_fdinfo_t* get_fd(int64_t fd);

	/*!
	  \brief Return true if this thread is bound to the given server port.
	*/
	bool is_bound_to_port(uint16_t number);

	/*!
	  \brief Return true if this thread has a client socket open on the given port.
	*/
	bool uses_client_port(uint16_t number);

	void* get_private_state(uint32_t id);

	/*!
	  \brief Return the ratio between open FDs and maximum available FDs for this thread.
	*/
	uint64_t get_fd_usage_pct();

	/*!
	  \brief Return the number of open FDs for this thread.
	*/
	uint64_t get_fd_opencount();

	/*!
	  \brief Return the maximum number of FDs this thread can open.
	*/
	uint64_t get_fd_limit();

	//
	// Core state
	//
	int64_t m_tid;  ///< The id of this thread
	int64_t m_pid; ///< The id of the process containing this thread. In single thread threads, this is equal to tid.
	int64_t m_ptid; ///< The id of the process that started this thread.
	int64_t m_progid; ///< Main program id. If this process is part of a logical group of processes (e.g. it's one of the apache processes), the tid of the process that is the head of this group.
	string m_comm; ///< Command name (e.g. "top")
	string m_exe; ///< Full command name (e.g. "/bin/top")
	vector<string> m_args; ///< Command line arguments (e.g. "-d1")
	vector<string> m_env; ///< Environment variables
	uint32_t m_flags; ///< The thread flags. See the PPM_CL_* declarations in ppm_events_public.h.
	int64_t m_fdlimit;  ///< The maximum number of FDs this thread can open
	uint32_t m_uid; ///< user id
	uint32_t m_gid; ///< group id
	uint64_t m_nchilds; ///< When this is 0 the process can be deleted
	uint32_t m_vmsize_kb; ///< total virtual memory (as kb).
	uint32_t m_vmrss_kb; ///< resident non-swapped memory (as kb).
	uint32_t m_vmswap_kb; ///< swapped memory (as kb).
	uint64_t m_pfmajor; ///< number of major page faults since start.
	uint64_t m_pfminor; ///< number of minor page faults since start.

	//
	// State for multi-event processing
	//
	int64_t m_lastevent_fd; ///< The FD os the last event used by this thread.
	uint64_t m_lastevent_ts; ///< timestamp of the last event for this thread.
	uint64_t m_prevevent_ts; ///< timestamp of the event before the last for this thread.
	uint64_t m_lastaccess_ts; ///< The last time this thread was looked up. Used when cleaning up the table. 
	uint64_t m_clone_ts; ///< When the clone that started this process happened.

	thread_analyzer_info* m_ainfo;

#ifdef HAS_FILTERING
	//
	// State for filtering
	//
	uint64_t m_last_latency_entertime;
	uint64_t m_latency;
#endif

	//
	// Global state
	//
	sinsp *m_inspector;

VISIBILITY_PRIVATE
	void init();
	void init(const scap_threadinfo* pi);
	void fix_sockets_coming_from_proc();
	sinsp_fdinfo_t* add_fd(int64_t fd, sinsp_fdinfo_t *fdinfo);
	void remove_fd(int64_t fd);
	sinsp_fdtable* get_fd_table();
	void set_cwd(const char *cwd, uint32_t cwdlen);
	sinsp_threadinfo* get_cwd_root();
	void set_args(const char* args, size_t len);
	void set_env(const char* env, size_t len);
	void store_event(sinsp_evt *evt);
	bool is_lastevent_data_valid();
	void set_lastevent_data_validity(bool isvalid);
	void allocate_private_state();

	//  void push_fdop(sinsp_fdop* op);
	// the queue of recent fd operations
	//  std::deque<sinsp_fdop> m_last_fdop;

	//
	// Parameters that can't be accessed directly because they could be in the
	// parent thread info
	//
	sinsp_fdtable m_fdtable; // The fd table of this thread
	string m_cwd; // current working directory
	sinsp_threadinfo* m_main_thread;
	sinsp_threadinfo* m_main_program_thread;
	uint8_t m_lastevent_data[SP_EVT_BUF_SIZE]; // Used by some event parsers to store the last enter event
	vector<void*> m_private_state;

	uint16_t m_lastevent_type;
	uint16_t m_lastevent_cpuid;
	sinsp_evt::category m_lastevent_category;

	friend class sinsp;
	friend class sinsp_parser;
	friend class sinsp_analyzer;
	friend class sinsp_analyzer_parsers;
	friend class sinsp_evt;
	friend class sinsp_thread_manager;
	friend class sinsp_transaction_table;
	friend class thread_analyzer_info;
	friend class lua_cbacks;
};

/*@}*/

typedef unordered_map<int64_t, sinsp_threadinfo> threadinfo_map_t;
typedef threadinfo_map_t::iterator threadinfo_map_iterator_t;


///////////////////////////////////////////////////////////////////////////////
// Little class that manages the allocation of private state in the thread info class
///////////////////////////////////////////////////////////////////////////////
class sinsp_thread_privatestate_manager
{
public:
	//
	// The return value is the ID of the newly reserved memory area
	//
	uint32_t reserve(uint32_t size)
	{
		m_memory_sizes.push_back(size);
		return (uint32_t)m_memory_sizes.size() - 1;
	}

	uint32_t get_size()
	{
		return (uint32_t)m_memory_sizes.size();
	}

private:
	vector<uint32_t> m_memory_sizes;

	friend class sinsp_threadinfo;
};

///////////////////////////////////////////////////////////////////////////////
// This class manages the thread table
///////////////////////////////////////////////////////////////////////////////
class SINSP_PUBLIC sinsp_thread_manager
{
public:
	sinsp_thread_manager(sinsp* inspector);
	void clear();

	void set_listener(sinsp_threadtable_listener* listener);
	//
	// Note: lookup_only should be used when the query for the thread is made
	//       not as a consequence of an event for that thread arriving, but for
	//       just for lookup reason. In that case, m_lastaccess_ts is not updated
	//       and m_last_tinfo is not set.
	//
	sinsp_threadinfo* get_thread(int64_t tid, bool lookup_only);
	void add_thread(sinsp_threadinfo& threadinfo, bool from_scap_proctable);
	void remove_thread(int64_t tid, bool force);
	void remove_thread(threadinfo_map_iterator_t it, bool force);
	// Returns true if the table is actually scanned
	bool remove_inactive_threads();
	void fix_sockets_coming_from_proc();
	void reset_child_dependencies();
	void create_child_dependencies();
	void recreate_child_dependencies();

	uint32_t get_thread_count()
	{
		return (uint32_t)m_threadtable.size();
	}

	void update_statistics();

	threadinfo_map_t* get_threads()
	{
		return &m_threadtable;
	}

	set<uint16_t> m_server_ports;

private:
	void increment_mainthread_childcount(sinsp_threadinfo* threadinfo);
	void increment_program_childcount(sinsp_threadinfo* threadinfo, uint32_t level, uint32_t notclosed_level);
	// Don't set level, it's for internal use
	void decrement_program_childcount(sinsp_threadinfo* threadinfo, uint32_t level = 0);
	inline void clear_thread_pointers(threadinfo_map_iterator_t it);

	sinsp* m_inspector;
	threadinfo_map_t m_threadtable;
	int64_t m_last_tid;
	sinsp_threadinfo* m_last_tinfo;
	uint64_t m_last_flush_time_ns;
	uint32_t m_n_drops;
	uint32_t m_n_proc_lookups;

	sinsp_threadtable_listener* m_listener;

	INTERNAL_COUNTER(m_failed_lookups);
	INTERNAL_COUNTER(m_cached_lookups);
	INTERNAL_COUNTER(m_non_cached_lookups);
	INTERNAL_COUNTER(m_added_threads);
	INTERNAL_COUNTER(m_removed_threads);

	friend class sinsp_parser;
	friend class sinsp_analyzer;
	friend class sinsp;
	friend class sinsp_threadinfo;
};
