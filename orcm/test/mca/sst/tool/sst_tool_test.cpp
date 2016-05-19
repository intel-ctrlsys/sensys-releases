/*
 * Copyright (c) 2016  Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "sst_tool_test.h"
#include "orcm/mca/sst/base/base.h"
#include "orcm/mca/sst/tool/sst_tool.h"
#include "orcm/runtime/orcm_globals.h"
#include "orcm/mca/cfgi/cfgi.h"
#include "sst_tool_mocking.h"

// C++ includes
#include <iostream>

using namespace std;

extern "C" {
    void signal_callback(int fd, short event, void *arg);
    int tool_init(void);
    extern orcm_cfgi_API_module_t orcm_cfgi;
    extern orte_proc_info_t orte_process_info;
    extern opal_list_t *orcm_schedulers;
};

extern mocking_flags sst_mocking;


void ut_sst_tool_tests::SetUpTestCase()
{
    cout << "In Setup\n";
}

void ut_sst_tool_tests::TearDownTestCase()
{
    cout << "In Teardown\n";
}

void ut_sst_tool_tests::InitMockingFlags()
{
    // Set boolean flags
    sst_mocking.orte_ess_base_std_prolog = false;
    sst_mocking.opal_pointer_array_init = false;
    sst_mocking.read_config = false;
    sst_mocking.define_system = false;
    sst_mocking.orte_plm_base_set_hnp_name = false;
    sst_mocking.orte_register_params = false;
    sst_mocking.mca_base_framework_open = false;
    // Change function pointers
    orcm_cfgi.read_config = &mock_cfgi_read_config;
    orcm_cfgi.define_system = &mock_cfgi_define_system;
    // Other initializations
    if (NULL == orte_process_info.nodename) {
        orte_process_info.nodename = strdup("Somenode");
    }
    if (NULL == orcm_schedulers) {
        orcm_schedulers = OBJ_NEW(opal_list_t);
    }
}

TEST_F(ut_sst_tool_tests, tool_init_std_prolog_err)
{
    int rc;
    InitMockingFlags();
    sst_mocking.orte_ess_base_std_prolog = true;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_opal_array_init_first)
{
    int rc;
    InitMockingFlags();
    sst_mocking.opal_pointer_array_init = true;
    sst_mocking.opal_pointer_array_init_max = 1;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_opal_array_init_second)
{
    int rc;
    InitMockingFlags();
    sst_mocking.opal_pointer_array_init = true;
    sst_mocking.opal_pointer_array_init_max = 2;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_opal_array_init_third)
{
    int rc;
    InitMockingFlags();
    sst_mocking.opal_pointer_array_init = true;
    sst_mocking.opal_pointer_array_init_max = 3;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_cfgi_read_config)
{
    int rc;
    InitMockingFlags();
    sst_mocking.read_config = true;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}


TEST_F(ut_sst_tool_tests, tool_init_cfgi_define_system)
{
    int rc;
    InitMockingFlags();
    sst_mocking.define_system = true;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_orte_plm_base_set_hnp_name)
{
    int rc;
    InitMockingFlags();
    sst_mocking.orte_plm_base_set_hnp_name = true;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}

TEST_F(ut_sst_tool_tests, tool_init_orte_register_params)
{
    int rc;
    InitMockingFlags();
    sst_mocking.orte_register_params = true;
    rc = orcm_sst_tool_module.init();
    EXPECT_EQ(rc, ORTE_ERR_SILENT);
}
