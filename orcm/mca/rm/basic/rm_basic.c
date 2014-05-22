/*
 * Copyright (c) 2014      Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "orcm_config.h"
#include "orcm/constants.h"

#include "opal/util/output.h"

#include "orte/mca/errmgr/errmgr.h"
#include "orte/util/regex.h"
#include "orte/mca/rml/rml.h"

#include "orcm/runtime/orcm_globals.h"
#include "orcm/mca/cfgi/base/base.h"
#include "orcm/mca/scd/base/base.h"
#include "orcm/mca/rm/base/base.h"
#include "rm_basic.h"

static int init(void);
static void finalize(void);


orcm_rm_base_module_t orcm_rm_basic_module = {
    init,
    finalize
};

static void basic_undef(int sd, short args, void *cbdata);
static void basic_request(int sd, short args, void *cbdata);
static void basic_active(int sd, short args, void *cbdata);

static orcm_rm_session_state_t states[] = {
    ORCM_SESSION_STATE_UNDEF,
    ORCM_SESSION_STATE_REQ,
    ORCM_SESSION_STATE_ACTIVE
};
static orcm_rm_state_cbfunc_t callbacks[] = {
    basic_undef,
    basic_request,
    basic_active
};

static int init(void)
{
    int i, rc, num_states;

    OPAL_OUTPUT_VERBOSE((5, orcm_rm_base_framework.framework_output,
                         "%s rm:basic:init",
                         ORTE_NAME_PRINT(ORTE_PROC_MY_NAME)));
    
    /* start the receive */
    if (ORCM_SUCCESS != (rc = orcm_rm_base_comm_start())) {
        ORTE_ERROR_LOG(rc);
        return rc;
    }

    /* define our state machine */
    num_states = sizeof(states) / sizeof(orcm_rm_session_state_t);
    for (i=0; i < num_states; i++) {
        if (ORCM_SUCCESS != (rc = orcm_rm_base_add_session_state(states[i],
                                                                 callbacks[i],
                                                                 ORTE_SYS_PRI))) {
            ORTE_ERROR_LOG(rc);
            return rc;
        }
    }

    return ORCM_SUCCESS;
}

static void finalize(void)
{
    OPAL_OUTPUT_VERBOSE((5, orcm_rm_base_framework.framework_output,
                         "%s rm:basic:finalize",
                         ORTE_NAME_PRINT(ORTE_PROC_MY_NAME)));
    
    orcm_rm_base_comm_stop();
}

static void basic_undef(int sd, short args, void *cbdata)
{
    orcm_session_caddy_t *caddy = (orcm_session_caddy_t*)cbdata;
    /* this isn't defined - so just report the error */
    opal_output(0, "%s UNDEF RM STATE CALLED",
                ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));
    OBJ_RELEASE(caddy);
}

static void basic_request(int sd, short args, void *cbdata)
{
    orcm_node_t *nodeptr;
    orcm_session_caddy_t *caddy = (orcm_session_caddy_t*)cbdata;
    int i, rc, num_nodes;
    char *nodelist = NULL;
    char *noderegex;

    num_nodes = caddy->session->alloc->min_nodes;

    if (0 < num_nodes) {
        for (i = 0; i < orcm_rm_base.nodes.size; i++) {
            if (NULL == (nodeptr =
                         (orcm_node_t*)opal_pointer_array_get_item(&orcm_rm_base.nodes, i))) {
                continue;
            }
            if (ORCM_SCD_NODE_STATE_UNALLOC == nodeptr->scd_state && ORCM_NODE_STATE_UP == nodeptr->state) {
                OPAL_OUTPUT_VERBOSE((5, orcm_rm_base_framework.framework_output,
                                     "%s rm:basic:request adding node %s to list",
                                     ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                                     nodeptr->name));
                if (NULL != nodelist) {
                    asprintf(&nodelist, "%s,%s", nodelist, nodeptr->name);
                } else {
                    asprintf(&nodelist, "%s", nodeptr->name);
                }
                num_nodes--;
                if (0 == num_nodes) {
                    break;
                }
            }
        }

        OPAL_OUTPUT_VERBOSE((5, orcm_rm_base_framework.framework_output,
                             "%s rm:basic:request giving allocation %i nodelist %s",
                             ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                             (int)caddy->session->alloc->id,
                             nodelist));

        if (0 == num_nodes) {
            if (ORTE_SUCCESS != (rc = orte_regex_create(nodelist, &noderegex))) {
                ORTE_ERROR_LOG(rc);
                caddy->session->alloc->nodes = strdup("ERROR");
            } else {
                caddy->session->alloc->nodes = noderegex;
            }
        } /* else, error? not enough nodes found? */

        OPAL_OUTPUT_VERBOSE((5, orcm_rm_base_framework.framework_output,
                             "%s rm:basic:request giving allocation %i noderegex %s",
                             ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                             (int)caddy->session->alloc->id,
                             noderegex));

        ORCM_ACTIVATE_SCD_STATE(caddy->session, ORCM_SESSION_STATE_ALLOCD);

        if(nodelist != NULL) {
            free(nodelist);
        }
    } /* else, error? no nodes requested */

    OBJ_RELEASE(caddy);
}

static void basic_active(int sd, short args, void *cbdata)
{
    orcm_session_caddy_t *caddy = (orcm_session_caddy_t*)cbdata;
    char **nodenames = NULL;
    int rc, i, j;
    orcm_node_t *nodeptr;
    opal_buffer_t *buf;
    orcm_rm_cmd_flag_t command = ORCM_LAUNCH_STEPD_COMMAND;

    if (ORTE_SUCCESS != (rc = orte_regex_extract_node_names(caddy->session->alloc->nodes, &nodenames))) {
        ORTE_ERROR_LOG(rc);
        OPAL_OUTPUT_VERBOSE((5, orcm_scd_base_framework.framework_output,
                             "%s rm:basic:active - (session: %d) could not extract nodelist\n",
                             ORTE_NAME_PRINT(ORTE_PROC_MY_NAME), caddy->session->id));
        if (NULL != nodenames) {
            opal_argv_free(nodenames);
        }
        return;
    }

    /* set hnp name to first in the list */
    caddy->session->alloc->hnpname = strdup(nodenames[0]);

    buf = OBJ_NEW(opal_buffer_t);
    /* pack the command */
    if (OPAL_SUCCESS != (rc = opal_dss.pack(buf, &command, 1, ORCM_RM_CMD_T))) {
        ORTE_ERROR_LOG(rc);
        return;
    }

    /* node array should be indexed by node num, if we change to lookup by index that would be faster */
    for (i = 0; i < caddy->session->alloc->min_nodes; i++) {
        for (j = 0; j < orcm_rm_base.nodes.size; j++) {
            if (NULL == (nodeptr =
                         (orcm_node_t*)opal_pointer_array_get_item(&orcm_rm_base.nodes, j))) {
                continue;
            }
            if (0 == strcmp(nodeptr->name, nodenames[i])) {
                if (0 == i) {
                    /* if this is the first node in the list, then set the hnp daemon info */
                    caddy->session->alloc->hnp.jobid = nodeptr->daemon.jobid;
                    caddy->session->alloc->hnp.vpid = nodeptr->daemon.vpid;
                    /* pack the alloc */
                    if (OPAL_SUCCESS != (rc = opal_dss.pack(buf, &caddy->session->alloc, 1, ORCM_ALLOC))) {
                        ORTE_ERROR_LOG(rc);
                        return;
                    }
                }
                /* SEND ALLOC TO NODE */
                if (ORTE_SUCCESS != (rc = orte_rml.send_buffer_nb(&nodeptr->daemon, buf,
                                                                  ORCM_RML_TAG_RM,
                                                                  orte_rml_send_callback, NULL))) {
                    ORTE_ERROR_LOG(rc);
                    OBJ_RELEASE(buf);
                    return;
                }
            }
        }
    }

    if (NULL != nodenames) {
        opal_argv_free(nodenames);
    }
    OBJ_RELEASE(caddy);
}
