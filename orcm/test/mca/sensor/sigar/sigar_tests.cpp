/*
 * Copyright (c) 2016  Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "sigar_tests.h"

// ORTE
#include "orte/runtime/orte_globals.h"

// ORCM
#include "orcm/mca/sensor/base/sensor_private.h"
#include "orcm/mca/sensor/base/sensor_runtime_metrics.h"
#include "orcm/mca/sensor/sigar/sensor_sigar.h"

// Fixture
using namespace std;

extern "C" {
    ORCM_DECLSPEC extern orcm_sensor_base_t orcm_sensor_base;
    extern void collect_sigar_sample(orcm_sensor_sampler_t *sampler);
    extern int sigar_enable_sampling(const char* sensor_specification);
    extern int sigar_disable_sampling(const char* sensor_specification);
    extern int sigar_reset_sampling(const char* sensor_specification);
};

void ut_sigar_tests::SetUpTestCase()
{
    // Configure/Create OPAL level resources
    opal_dss_register_vars();
    opal_dss_open();
}

void ut_sigar_tests::TearDownTestCase()
{
    // Release OPAL level resources
    opal_dss_close();
}


// Testing the data collection class
TEST_F(ut_sigar_tests, sigar_sensor_sample_tests)
{
    // Setup
    void* object = orcm_sensor_base_runtime_metrics_create("sigar", false, false);
    mca_sensor_sigar_component.runtime_metrics = object;
    orcm_sensor_sampler_t* sampler = (orcm_sensor_sampler_t*)OBJ_NEW(orcm_sensor_sampler_t);

    // Tests
    collect_sigar_sample(sampler);
    EXPECT_EQ(0, (mca_sensor_sigar_component.diagnostics & 0x1));

    orcm_sensor_base_runtime_metrics_set(object, true, "sigar");
    collect_sigar_sample(sampler);
    EXPECT_EQ(1, (mca_sensor_sigar_component.diagnostics & 0x1));

    // Cleanup
    OBJ_RELEASE(sampler);
    orcm_sensor_base_runtime_metrics_destroy(object);
    mca_sensor_sigar_component.runtime_metrics = NULL;
}

TEST_F(ut_sigar_tests, sigar_api_tests)
{
    // Setup
    void* object = orcm_sensor_base_runtime_metrics_create("sigar", false, false);
    mca_sensor_sigar_component.runtime_metrics = object;

    // Tests
    sigar_enable_sampling("sigar");
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));
    sigar_disable_sampling("sigar");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));
    sigar_enable_sampling("sigar");
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));
    sigar_reset_sampling("sigar");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));
    sigar_enable_sampling("not_the_right_datagroup");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));
    sigar_enable_sampling("all");
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, NULL));

    // Cleanup
    orcm_sensor_base_runtime_metrics_destroy(object);
    mca_sensor_sigar_component.runtime_metrics = NULL;
}


TEST_F(ut_sigar_tests, sigar_api_tests_2)
{
    // Setup
    void* object = orcm_sensor_base_runtime_metrics_create("sigar", false, false);
    mca_sensor_sigar_component.runtime_metrics = object;
    orcm_sensor_base_runtime_metrics_track(object, "memory");
    orcm_sensor_base_runtime_metrics_track(object, "cpu_load");
    orcm_sensor_base_runtime_metrics_track(object, "io_ops");
    orcm_sensor_base_runtime_metrics_track(object, "procstat");

    // Tests
    sigar_enable_sampling("sigar:procstat");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, "cpu_load"));
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, "procstat"));
    EXPECT_EQ(1,orcm_sensor_base_runtime_metrics_active_label_count(object));
    sigar_disable_sampling("sigar:procstat");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, "procstat"));
    sigar_enable_sampling("sigar:io_ops");
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, "io_ops"));
    sigar_reset_sampling("sigar:io_ops");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, "io_ops"));
    sigar_enable_sampling("sigar:core no_core");
    EXPECT_FALSE(orcm_sensor_base_runtime_metrics_do_collect(object, "io_ops"));
    sigar_enable_sampling("sigar:all");
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, "memory"));
    EXPECT_TRUE(orcm_sensor_base_runtime_metrics_do_collect(object, "procstat"));
    EXPECT_EQ(4,orcm_sensor_base_runtime_metrics_active_label_count(object));

    // Cleanup
    orcm_sensor_base_runtime_metrics_destroy(object);
    mca_sensor_sigar_component.runtime_metrics = NULL;
}
