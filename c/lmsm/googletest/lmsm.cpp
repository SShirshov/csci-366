#include "gtest/gtest.h"
extern "C" {
#include "lmsm.h"
}

TEST(lmsm_machine_suite,test_add_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 0;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 100); // ADD 00
    ASSERT_EQ(the_machine->accumulator->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_add_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 995;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 100); // ADD 00
    ASSERT_EQ(the_machine->accumulator->value, 999);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sub_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 20;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 200); // SUB 00
    ASSERT_EQ(the_machine->accumulator->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sub_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = -995;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 200); // SUB 00
    ASSERT_EQ(the_machine->accumulator->value, -999);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_store_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 300); // STA 00
    ASSERT_EQ(the_machine->memory[0], 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_load_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 0;
    the_machine->memory[20] = 10;
    lmsm_exec_instruction(the_machine, 520); // LDA 20
    ASSERT_EQ(the_machine->accumulator->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_instruction_works){
    lmsm *the_machine = lmsm_create();
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 620); // BRA 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_zero_instruction_works_when_val_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 0;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 720); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_zero_instruction_works_when_val_not_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 720); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 0;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_positive){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_not_positive){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = -10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_output_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 902); // OUT 20
    lmsm_exec_instruction(the_machine, 902); // OUT 20
    ASSERT_STREQ(the_machine->output_buffer, "10 10 ");
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_halt_instruction){
    lmsm *the_machine = lmsm_create();
    ASSERT_EQ(the_machine->status, machine_status::STATUS_READY);
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 000); // HLT
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_load_immediate_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 0;
    lmsm_exec_instruction(the_machine, 499); // LDI 99
    ASSERT_EQ(the_machine->accumulator->value, 99);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_push_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->accumulator->value, 0);
    ASSERT_EQ(the_machine->accumulator->next->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_pop_instruction_removes_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->accumulator->value, 0);
    ASSERT_EQ(the_machine->accumulator->next->value, 10);

    lmsm_exec_instruction(the_machine, 921); // SPOP
    ASSERT_EQ(the_machine->accumulator->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_pop_instruction_enters_error_state_if_nothing_to_pop){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 921); // SPOP
    lmsm_exec_instruction(the_machine, 921); // SPOP
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_dup_instruction_duplicates_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    ASSERT_EQ(the_machine->accumulator->value, 10);
    ASSERT_EQ(the_machine->accumulator->next->value, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sadd_instruction_adds_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 923); // SADD
    ASSERT_EQ(the_machine->accumulator->value, 20);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sadd_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 500;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 923); // SADD
    ASSERT_EQ(the_machine->accumulator->value, 999);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sadd_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 923); // SADD
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_subtracts_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 924); // SSUB
    ASSERT_EQ(the_machine->accumulator->value, 0);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = -500;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 500;
    lmsm_exec_instruction(the_machine, 924); // SSUB
    ASSERT_EQ(the_machine->accumulator->value, -999);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_subtracts_the_values_on_the_stack_in_the_right_order){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 20; // set top value to 20
    lmsm_exec_instruction(the_machine, 924); // SSUB
    ASSERT_EQ(the_machine->accumulator->value, -10);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 924); // SSUB
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smax_instruction_sets_max_val_on_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 20;
    lmsm_exec_instruction(the_machine, 925); // SMAX
    ASSERT_EQ(the_machine->accumulator->value, 20);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smax_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 925); // SMAX
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smin_instruction_sets_min_val_on_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 20;
    lmsm_exec_instruction(the_machine, 926); // SMIN
    ASSERT_EQ(the_machine->accumulator->value, 10);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smin_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 926); // SMIN
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_multiplies_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 20;
    lmsm_exec_instruction(the_machine, 927); // SMUL
    ASSERT_EQ(the_machine->accumulator->value, 200);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 50;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 20;
    lmsm_exec_instruction(the_machine, 927); // SMUL
    ASSERT_EQ(the_machine->accumulator->value, 999);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 927); // SMUL
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sdiv_instruction_divides_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 20;
    lmsm_exec_instruction(the_machine, 922); // SDUP
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 928); // SDIV
    ASSERT_EQ(the_machine->accumulator->value, 2);
    ASSERT_EQ(the_machine->accumulator->next, nullptr); //C++ equivalent of NULL
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sdiv_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator->value = 10;
    lmsm_exec_instruction(the_machine, 928); // SDIV
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_EMPTY_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_call_instruction_pushes_next_address_onto_stack){
    lmsm *the_machine = lmsm_create();

    lmsm_exec_instruction(the_machine, 920); // push a new elt
    the_machine->accumulator->value = 22; // call function at slot 22
    the_machine->program_counter = 3; // next instruction is slot 3

    lmsm_exec_instruction(the_machine, 910);

    ASSERT_EQ(the_machine->accumulator->value, 0); // top of stack should have been consumed
    ASSERT_EQ(the_machine->program_counter, 22); // machine should have jumped to slot 22
    ASSERT_EQ(the_machine->call_stack->value, 3); // the top of the call stack should be 3, where we return to

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_return_instruction_returns_to_address_on_top_of_call_stack){
    lmsm *the_machine = lmsm_create();

    lmsm_exec_instruction(the_machine, 920); // push a new elt
    the_machine->accumulator->value = 22; // call function at slot 22
    the_machine->program_counter = 3; // next instruction is slot 3

    lmsm_exec_instruction(the_machine, 910); // CALL

    ASSERT_EQ(the_machine->accumulator->value, 0); // top of stack should have been consumed
    ASSERT_EQ(the_machine->program_counter, 22); // machine should have jumped to slot 22
    ASSERT_EQ(the_machine->call_stack->value, 3); // the top of the call stack should be 3, where we return to

    lmsm_exec_instruction(the_machine, 911); // RETURN

    ASSERT_EQ(the_machine->program_counter, 3); // machine should have jumped to slot 22
    ASSERT_EQ(the_machine->call_stack->value, 0); // the top of the call stack should be 0

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,step_steps_to_the_next_instruction_and_executes_current){

    lmsm *the_machine = lmsm_create();

    the_machine->accumulator->value = 10;
    the_machine->program_counter = 0;
    the_machine->memory[0] = 902; // OUT
    the_machine->memory[1] = 000; // HLT
    lmsm_step(the_machine); // should execute the 902 instruction and move to the next position

    ASSERT_EQ(the_machine->program_counter, 1); // should have bumped the pc
    ASSERT_EQ(the_machine->current_instruction, 902); // should have loaded the instruction from mem[0]
    ASSERT_STREQ(the_machine->output_buffer, "10 "); // should have executed the instruction (OUT)

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,step_does_not_step_to_the_next_instruction_and_execute_if_machine_is_halted){

    lmsm *the_machine = lmsm_create();

    the_machine->accumulator->value = 10;
    the_machine->program_counter = 0;
    the_machine->memory[0] = 902; // OUT
    the_machine->memory[1] = 000; // HLT

    the_machine->status = STATUS_HALTED; // halt the machine
    lmsm_step(the_machine); // should execute the 902 instruction and move to the next position

    ASSERT_EQ(the_machine->program_counter, 0); // should not have bumped the pc since halted
    ASSERT_EQ(the_machine->current_instruction, 0); // should not have loaded the instruction from mem[0]
    ASSERT_STREQ(the_machine->output_buffer, ""); // should not have executed the instruction (OUT)

    lmsm_delete(the_machine);
}

