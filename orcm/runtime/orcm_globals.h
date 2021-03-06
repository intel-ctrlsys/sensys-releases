/*
 * Copyright (c) 2009-2010 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2014-2016 Intel, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

/**
 * @file
 *
 * Interface into the ORCM Library
 */
#ifndef ORCM_GLOBALS_H
#define ORCM_GLOBALS_H

#include "orcm_config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "opal/mca/event/event.h"
#include "opal/class/opal_list.h"
#include "opal/dss/dss_types.h"

#include "orte/util/proc_info.h"

BEGIN_C_DECLS

/* define some process types */
typedef orte_proc_type_t orcm_proc_type_t;
#define ORCM_TOOL           (ORTE_PROC_TOOL | ORTE_PROC_CM)
#define ORCM_DAEMON         (ORTE_PROC_DAEMON | ORTE_PROC_CM)
#define ORCM_AGGREGATOR     (ORTE_PROC_AGGREGATOR | ORTE_PROC_CM)
#define ORCM_IOF_ENDPT      (ORTE_PROC_IOF_ENDPT | ORTE_PROC_CM)
#define ORCM_SCHED          (ORTE_PROC_SCHEDULER | ORTE_PROC_CM)
#define ORCM_MASTER         (ORTE_PROC_MASTER | ORTE_PROC_CM)
#define ORCM_EMULATOR       (ORTE_PROC_EMULATOR | ORTE_PROC_CM)
/* reuse ORTE_PROC_NON_MPI as ORCM_STED here.*/
#define ORCM_STEPD          (0x0010 | ORTE_PROC_CM)
#define ORCM_HNP            (ORTE_PROC_HNP | ORTE_PROC_CM)

/* just define these tests to their ORTE equivalent
 * as otherwise they will always be true since anything
 * in ORCM will have ORTE_PROC_CM set
 */
#define ORCM_PROC_IS_DAEMON      ORTE_PROC_IS_DAEMON
#define ORCM_PROC_IS_AGGREGATOR  ORTE_PROC_IS_AGGREGATOR
#define ORCM_PROC_IS_SCHED       ORTE_PROC_IS_SCHEDULER
#define ORCM_PROC_IS_TOOL        ORTE_PROC_IS_TOOL
#define ORCM_PROC_IS_IOF_ENDPT   ORTE_PROC_IS_IOF_ENDPT
#define ORCM_PROC_IS_MASTER      ORTE_PROC_IS_MASTER
#define ORCM_PROC_IS_EMULATOR    ORTE_PROC_IS_EMULATOR
#define ORCM_PROC_IS_HNP         ORTE_PROC_IS_HNP
#define ORCM_PROC_IS_STEPD       (0x0010 & orte_process_info.proc_type)

/****    NODESTATE TYPE    ****/
typedef int8_t orcm_node_state_t;
#define ORCM_NODE_STATE_T OPAL_INT8

#define ORCM_NODE_STATE_UNDEF         0  // Node is undefined
#define ORCM_NODE_STATE_UNKNOWN       1  // Node is in unknown state
#define ORCM_NODE_STATE_UP            2  // Node is up
#define ORCM_NODE_STATE_DOWN          3  // Node is down
#define ORCM_NODE_STATE_SESTERM       4  // Node is terminating session
#define ORCM_NODE_STATE_DRAIN         5  // Node is draining/drained
#define ORCM_NODE_STATE_RESUME        6  // Node is resuming operation

/* define a few commands for sending between orcmd's and orcmsched */
typedef uint8_t orcm_rm_cmd_flag_t;
#define ORCM_RM_CMD_T OPAL_UINT8

#define ORCM_NODESTATE_REQ_COMMAND                1
#define ORCM_RESOURCE_REQ_COMMAND                 2
#define ORCM_NODESTATE_UPDATE_COMMAND             3
#define ORCM_VM_READY_COMMAND                     4
#define ORCM_LAUNCH_STEPD_COMMAND                 5
#define ORCM_CANCEL_STEPD_COMMAND                 6
#define ORCM_STEPD_COMPLETE_COMMAND               7
#define ORCM_CALIBRATE_COMMAND                    8
#define ORCM_SET_POWER_COMMAND                    9
#define ORCM_GET_POWER_COMMAND                    10
#define ORCM_SET_POWER_BUDGET_COMMAND             11
#define ORCM_SET_POWER_MODE_COMMAND               12
#define ORCM_SET_POWER_WINDOW_COMMAND             13
#define ORCM_SET_POWER_OVERAGE_COMMAND            14
#define ORCM_SET_POWER_UNDERAGE_COMMAND           15
#define ORCM_SET_POWER_OVERAGE_TIME_COMMAND       16
#define ORCM_SET_POWER_UNDERAGE_TIME_COMMAND      17
#define ORCM_SET_POWER_FREQUENCY_COMMAND          18
#define ORCM_SET_POWER_STRICT_COMMAND             19
#define ORCM_GET_POWER_BUDGET_COMMAND             20
#define ORCM_GET_POWER_MODE_COMMAND               21
#define ORCM_GET_POWER_WINDOW_COMMAND             22
#define ORCM_GET_POWER_OVERAGE_COMMAND            23
#define ORCM_GET_POWER_UNDERAGE_COMMAND           24
#define ORCM_GET_POWER_OVERAGE_TIME_COMMAND       25
#define ORCM_GET_POWER_UNDERAGE_TIME_COMMAND      26
#define ORCM_GET_POWER_FREQUENCY_COMMAND          27
#define ORCM_GET_POWER_MODES_COMMAND              28
#define ORCM_GET_POWER_STRICT_COMMAND             29
#define ORCM_GET_DB_SENSOR_INVENTORY_COMMAND      30
#define ORCM_GET_DB_QUERY_HISTORY_COMMAND         31
#define ORCM_GET_DB_QUERY_SENSOR_COMMAND          32
#define ORCM_GET_DB_QUERY_LOG_COMMAND             33
#define ORCM_GET_DB_QUERY_IDLE_COMMAND            34
#define ORCM_GET_DB_QUERY_NODE_COMMAND            35
#define ORCM_GET_DB_QUERY_EVENT_DATA_COMMAND      36
#define ORCM_GET_DB_QUERY_EVENT_SNSR_DATA_COMMAND 37
#define ORCM_GET_DB_QUERY_EVENT_DATE_COMMAND      38
#define ORCM_GET_DB_STREAM                        39
#define ORCM_GET_CHASSIS_ID                       40
#define ORCM_GET_CHASSIS_ID_STATE                 41
#define ORCM_SET_CHASSIS_ID                       42
#define ORCM_SET_CHASSIS_ID_OFF                   43
#define ORCM_SET_CHASSIS_ID_ON                    44
#define ORCM_SET_CHASSIS_ID_TEMPORARY_ON          45
#define ORCM_DISPATCH_LAUNCH_EXEC_COMMAND         46

/* define diagnostic commands */
typedef uint8_t orcm_diag_cmd_flag_t;
#define ORCM_DIAG_CMD_T OPAL_UINT8

#define ORCM_DIAG_START_COMMAND       1
#define ORCM_DIAG_AGG_COMMAND         2

/* define sensor commands */
typedef uint8_t orcm_sensor_cmd_flag_t;
#define ORCM_SENSOR_CMD_T OPAL_UINT8

#define ORCM_SET_SENSOR_COMMAND               1
#define ORCM_GET_SENSOR_COMMAND               2
#define ORCM_SET_SENSOR_SAMPLE_RATE_COMMAND   3
#define ORCM_GET_SENSOR_SAMPLE_RATE_COMMAND   4
#define ORCM_SET_SENSOR_POLICY_COMMAND        5
#define ORCM_GET_SENSOR_POLICY_COMMAND        6
#define ORCM_ENABLE_SENSOR_SAMPLING_COMMAND   7
#define ORCM_DISABLE_SENSOR_SAMPLING_COMMAND  8
#define ORCM_RESET_SENSOR_SAMPLING_COMMAND    9


/* define notifier commands */
typedef uint8_t orcm_cmd_server_flag_t;
#define ORCM_CMD_SERVER_T OPAL_UINT8

#define ORCM_SET_NOTIFIER_COMMAND               1
#define ORCM_GET_NOTIFIER_COMMAND               2
#define ORCM_SET_NOTIFIER_POLICY_COMMAND        3
#define ORCM_GET_NOTIFIER_POLICY_COMMAND        4
#define ORCM_SET_NOTIFIER_SMTP_COMMAND          5
#define ORCM_GET_NOTIFIER_SMTP_COMMAND          6


/** version string of ORCM */
ORCM_DECLSPEC extern const char openrcm_version_string[];

/**
 * Whether ORCM is initialized or we are in openrcm_finalize
 */
ORCM_DECLSPEC extern int orcm_initialized;
ORCM_DECLSPEC extern bool orcm_finalizing;

/* debugger output control */
ORCM_DECLSPEC extern int orcm_debug_output;
ORCM_DECLSPEC extern int orcm_debug_verbosity;

ORCM_DECLSPEC extern char *orcm_event_exec_path;

/* the logical hostname where the daemon is running */
ORCM_DECLSPEC extern char *orcm_proc_hostname;

/* extend the ORTE RML tags to add ORCM DAEMONS tags */
/* scheduler */
#define ORCM_RML_TAG_SCD           (ORTE_RML_TAG_MAX + 1)
#define ORCM_RML_TAG_RM            (ORTE_RML_TAG_MAX + 2)
/* session daemons */
#define ORCM_RML_TAG_HNP           (ORTE_RML_TAG_MAX + 3)
#define ORCM_RML_TAG_DAEMON        (ORTE_RML_TAG_MAX + 4)
/* analytics */
#define ORCM_RML_TAG_ANALYTICS     (ORTE_RML_TAG_MAX + 5)
/* tools */
#define ORCM_RML_TAG_VM_READY      (ORTE_RML_TAG_MAX + 6)
/* diagnostics */
#define ORCM_RML_TAG_DIAG          (ORTE_RML_TAG_MAX + 7)
/* Inventory */
#define ORCM_RML_TAG_INVENTORY     (ORTE_RML_TAG_MAX + 8)
/* pwrmgmt base */
#define ORCM_RML_TAG_PWRMGMT_BASE  (ORTE_RML_TAG_MAX + 9)
/* autotuner */
#define ORCM_RML_TAG_AT            (ORTE_RML_TAG_MAX + 10)
/* sensor */
#define ORCM_RML_TAG_SENSOR        (ORTE_RML_TAG_MAX + 11)
/* db fetch */
#define ORCM_RML_TAG_ORCMD_FETCH   (ORTE_RML_TAG_MAX + 12)
/* cmd server */
#define ORCM_RML_TAG_CMD_SERVER    (ORTE_RML_TAG_MAX + 13)


/* define event base priorities */
#define ORCM_SCHED_PRI OPAL_EV_MSG_HI_PRI
#define ORCM_INFO_PRI  OPAL_EV_INFO_HI_PRI


/* system descriptions */
ORCM_DECLSPEC extern opal_list_t *orcm_clusters;
ORCM_DECLSPEC extern opal_list_t *orcm_schedulers;

/**
 * Init the ORCM datatype support
 */
ORCM_DECLSPEC int orcm_dt_init(void);

/* APIs to set/get the hostname where the daemon is running
 * */
ORCM_DECLSPEC int orcm_set_proc_hostname(char *hostname);
ORCM_DECLSPEC char *orcm_get_proc_hostname(void);

const char *orcm_node_state_to_str(orcm_node_state_t state);
const char *orcm_node_state_to_char(orcm_node_state_t state);

typedef struct {
    opal_value_t value;
    char *units;
} orcm_value_t;
ORCM_DECLSPEC OBJ_CLASS_DECLARATION(orcm_value_t);

END_C_DECLS

#endif /* ORCM_GLOBALS_H */
