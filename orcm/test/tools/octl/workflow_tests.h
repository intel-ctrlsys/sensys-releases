/*
 * Copyright (c) 2016  Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef OCTL_WORKFLOW_TESTS_H
#define OCTL_WORKFLOW_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    // OPAL
    #include "opal/dss/dss.h"
    // ORTE
    #include "orte/mca/rml/rml.h"
    #include "orcm/util/utils.h"
    #include "orcm/tools/octl/common.h"
    #include "orcm/mca/parser/parser.h"
    #include "orcm/mca/parser/base/base.h"
#ifdef __cplusplus
};
#endif // __cplusplus

#include "gtest/gtest.h"
#include "octl_mocking.h"
#include "octl_tests.h"

class ut_octl_workflow_tests: public ut_octl_tests
{
    protected:
        // gtest fixture required methods
        static void SetUpTestCase();
        static void TearDownTestCase();
        static int NegOpenFile(char const *file);
        static int OpenFile(char const *file);
        static int CloseFile(int file_id);
        static void NegRemoveRecvBuffer(orte_process_name_t* peer,
                                        orte_rml_tag_t tag,
                                        bool persistent,
                                        orte_rml_buffer_callback_fn_t cbfunc,
                                        void* cbdata);
        static void NegListRecvBuffer(orte_process_name_t* peer,
                                      orte_rml_tag_t tag,
                                      bool persistent,
                                      orte_rml_buffer_callback_fn_t cbfunc,
                                      void* cbdata);
        static orcm_parser_module_open_fn_t saved_open_fn_t;
        static orcm_parser_module_retrieve_section_fn_t saved_retrieve_section_fnt_t;
        static orcm_parser_module_close_fn_t saved_close_fn_t;
}; // class

#endif // OCTL_WORKFLOW_TESTS_H
