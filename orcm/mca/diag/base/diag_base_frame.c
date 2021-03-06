/*
 * Copyright (c) 2013-2016 Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */


#include "orcm_config.h"
#include "orcm/constants.h"
#include "orcm/types.h"

#include "opal/mca/mca.h"
#include "opal/mca/base/base.h"
#include "opal/mca/event/event.h"
#include "opal/dss/dss.h"
#include "opal/util/output.h"
#include "opal/runtime/opal_progress_threads.h"

#include "orte/mca/errmgr/errmgr.h"

#include "orcm/mca/db/db.h"
#include "orcm/mca/diag/base/base.h"


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * component's public mca_base_component_t struct.
 */

#include "orcm/mca/diag/base/static-components.h"

/*
 * Global variables
 */
orcm_diag_API_module_t orcm_diag = {
    orcm_diag_base_calibrate
};
orcm_diag_base_t orcm_diag_base;

static void db_open_cb(int handle, int status, opal_list_t *kvs,
                       opal_list_t *ret, void *cbdata)
{
    if (ORCM_SUCCESS == status) {
        orcm_diag_base.dbhandle = handle;
    }
}

static int orcm_diag_base_close(void)
{
    int rc;
    orcm_diag_active_module_t* mod = NULL;

    if (ORCM_SUCCESS != (rc = orcm_diag_base_comm_stop())) {
        ORTE_ERROR_LOG(rc);
    }

    /* stop the thread */
    opal_progress_thread_finalize("diag");

    /* deconstruct the base objects */
    OPAL_LIST_FOREACH(mod, &orcm_diag_base.modules, orcm_diag_active_module_t) {
        if(NULL != mod->module && NULL != mod->module->finalize) {
            mod->module->finalize();
        }
    }
    OPAL_LIST_DESTRUCT(&orcm_diag_base.modules);

    return mca_base_framework_components_close(&orcm_diag_base_framework, NULL);
}

/**
 * Function for finding and opening either all MCA components, or the one
 * that was specifically requested via a MCA parameter.
 */
static int orcm_diag_base_open(mca_base_open_flag_t flags)
{
    int rc;

    /* setup the base objects */
    orcm_diag_base.dbhandle = -1;
    orcm_diag_base.dbhandle_requested = false;
    OBJ_CONSTRUCT(&orcm_diag_base.modules, opal_list_t);

    if (OPAL_SUCCESS != (rc = mca_base_framework_components_open(&orcm_diag_base_framework, flags))) {
        return rc;
    }

    /* create the event base and start the progress thread */
    if (NULL == (orcm_diag_base.ev_base = opal_progress_thread_init("diag"))) {
        return ORCM_ERR_OUT_OF_RESOURCE;
    }

    if (ORCM_SUCCESS != (rc = orcm_diag_base_comm_start())) {
        ORTE_ERROR_LOG(rc);
    }

    if (!orcm_diag_base.dbhandle_requested && ORCM_PROC_IS_AGGREGATOR) {
        orcm_db.open("diag", NULL, db_open_cb, NULL);
        orcm_diag_base.dbhandle_requested = true;
    }

    return rc;
}

MCA_BASE_FRAMEWORK_DECLARE(orcm, diag, NULL, NULL,
                           orcm_diag_base_open, orcm_diag_base_close,
                           mca_diag_base_static_components, 0);

OBJ_CLASS_INSTANCE(orcm_diag_active_module_t,
                   opal_list_item_t,
                   NULL, NULL);

static void caddy_con(orcm_diag_caddy_t *p)
{
    p->info = NULL;
}
static void caddy_des(orcm_diag_caddy_t *p)
{
    if(p->info) {
        OBJ_RELEASE(p->info);
    }
}
OBJ_CLASS_INSTANCE(orcm_diag_caddy_t,
                   opal_object_t,
                   caddy_con, caddy_des);

static void info_con(orcm_diag_info_t *p)
{
    p->component = NULL;
    p->want_result = false;
    p->requester = NULL;
    OBJ_CONSTRUCT(&p->options, opal_list_t);
}
static void info_des(orcm_diag_info_t *p)
{
    free(p->component);
    OPAL_LIST_DESTRUCT(&p->options);
}
OBJ_CLASS_INSTANCE(orcm_diag_info_t,
                   opal_object_t,
                   info_con, info_des);
