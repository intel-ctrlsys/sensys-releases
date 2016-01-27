/*
 * Copyright (c) 2016  Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "file_tests.h"

// ORTE
#include "orte/runtime/orte_globals.h"

// ORCM
#include "orcm/mca/sensor/base/sensor_private.h"
#include "orcm/mca/sensor/base/sensor_runtime_metrics.h"
#include "orcm/mca/sensor/file/sensor_file.h"

#define SAFE_FREE(x) if(NULL != x) free(x); x = NULL;

// Fixture
using namespace std;

extern "C" {
    ORCM_DECLSPEC extern orcm_sensor_base_t orcm_sensor_base;
    extern void collect_file_sample(orcm_sensor_sampler_t *sampler);
    extern orcm_sensor_base_module_t orcm_sensor_file_module;
};

void ut_file_tests::SetUpTestCase()
{
    // Configure/Create OPAL level resources
    opal_dss_register_vars();
    opal_dss_open();
}

void ut_file_tests::TearDownTestCase()
{
    // Release OPAL level resources
    opal_dss_close();
}


// Testing the data collection class
TEST_F(ut_file_tests, file_sensor_sample_tests)
{
    // Setup
    orcm_sensor_sampler_t* sampler = (orcm_sensor_sampler_t*)OBJ_NEW(orcm_sensor_sampler_t);
    mca_sensor_file_component.file = strdup("/bin/ls");
    mca_sensor_file_component.check_size = true;
    mca_sensor_file_component.use_progress_thread = false;
    orcm_sensor_file_module.init();
    void* object = mca_sensor_file_component.runtime_metrics;
    orcm_sensor_file_module.start(0);

    // Tests
    orcm_sensor_file_module.sample(sampler);
    EXPECT_EQ(0, (mca_sensor_file_component.diagnostics & 0x1));

    orcm_sensor_base_runtime_metrics_set(object, true);
    orcm_sensor_file_module.sample(sampler);
    EXPECT_EQ(1, (mca_sensor_file_component.diagnostics & 0x1));

    // Cleanup
    OBJ_RELEASE(sampler);

    orcm_sensor_file_module.stop(0);
    orcm_sensor_file_module.finalize();
    SAFE_FREE(mca_sensor_file_component.file);
}