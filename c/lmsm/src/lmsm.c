#include "lmsm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//======================================================
//  Instruction Implementation
//======================================================

void lmsm_i_call(lmsm *our_little_machine) {

    //update stack
    lmsm_stack *current_call_stack = our_little_machine->call_stack;
    lmsm_stack *new_call_stack = malloc(sizeof(lmsm_stack));
    new_call_stack->next = current_call_stack;
    new_call_stack->value = our_little_machine->program_counter;
    our_little_machine->call_stack = new_call_stack;
    //update pc
    our_little_machine->program_counter = our_little_machine->accumulator->value;
    //update accumulator //possibly not memory efficient

    //our_little_machine->accumulator->value = our_little_machine->accumulator->next->value;
    //our_little_machine->accumulator->next = our_little_machine->accumulator->next->next;

    our_little_machine->accumulator = our_little_machine->accumulator->next;

}

void lmsm_i_return(lmsm *our_little_machine) {
    //update pc
    our_little_machine->program_counter = our_little_machine->call_stack->value;
    //update call stack
    our_little_machine->call_stack = our_little_machine->call_stack->next;

}

void lmsm_i_push(lmsm *our_little_machine) {
    lmsm_stack *current_accumulator = our_little_machine->accumulator;
    lmsm_stack *new_accumulator = malloc(sizeof(lmsm_stack));
    new_accumulator->value = 0;
    new_accumulator->next = current_accumulator;
    our_little_machine->accumulator = new_accumulator;
}

void lmsm_i_pop(lmsm *our_little_machine) {
    if (our_little_machine->accumulator->next == NULL){
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
        return;
    }
    //hold current accumulator
    lmsm_stack *current_accumulator = our_little_machine->accumulator;
    //create new accumulator with value and next pointer

    lmsm_stack *new_accumulator = malloc(sizeof(lmsm_stack));

    new_accumulator->value = current_accumulator->next->value;
    new_accumulator->next = current_accumulator->next->next;
    our_little_machine->accumulator = new_accumulator;
    //DEBUG
    // printf("%d, %d",our_little_machine->accumulator->value, our_little_machine->accumulator->next);
    //could also do instead
    //our_little_machine->accumulator = our_little_machine->accumulator->next;
    //-------------

    free(current_accumulator);
}

void lmsm_i_dup(lmsm *our_little_machine) {
    lmsm_stack *current_accumulator = our_little_machine->accumulator;
    lmsm_stack *new_accumulator = malloc(sizeof(lmsm_stack));
    new_accumulator->value = current_accumulator->value;
    new_accumulator->next = current_accumulator;
    our_little_machine->accumulator = new_accumulator;
}

int check_stack(lmsm *our_little_machine) {
    lmsm_stack *current = our_little_machine->accumulator;
    if (current == NULL) {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
        return 0;
    }
    lmsm_stack *next = current->next;
    if (next == NULL) {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
        return 0;
    }
    return 1;
}

void lmsm_i_sadd(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    new->value = current->value + next->value;

    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_ssub(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *top = our_little_machine->accumulator;
    lmsm_stack *second = top->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    new->value = second->value - top->value;

    new->next = second->next;

    our_little_machine->accumulator = new;

    free(top);
    free(second);
}

void lmsm_i_smax(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *top = our_little_machine->accumulator;
    lmsm_stack *second = top->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    if(second->value > top->value) {
        new->value = second->value;
    }
    else{
        new->value = top->value;
    }

    new->next = second->next;

    our_little_machine->accumulator = new;

    free(top);
    free(second);
}

void lmsm_i_smin(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *top = our_little_machine->accumulator;
    lmsm_stack *second = top->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    if(second->value < top->value) {
        new->value = second->value;
    }
    else{
        new->value = top->value;
    }

    new->next = second->next;

    our_little_machine->accumulator = new;

    free(top);
    free(second);
}

void lmsm_i_smul(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *top = our_little_machine->accumulator;
    lmsm_stack *second = top->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    new->value = second->value * top->value;

    new->next = second->next;

    our_little_machine->accumulator = new;

    free(top);
    free(second);
}

void lmsm_i_sdiv(lmsm *our_little_machine) {
    if (!check_stack(our_little_machine)) {
        return;
    }
    lmsm_stack *top = our_little_machine->accumulator;
    lmsm_stack *second = top->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    new->value = second->value / top->value;

    new->next = second->next;

    our_little_machine->accumulator = new;

    free(top);
    free(second);
}

void append(char* s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void lmsm_i_out(lmsm *our_little_machine) {
    //create char list temp to hold accumulator value
    char accum_value [10];
    //copy over value in accumulator plus space to accum_value
    sprintf(accum_value, "%d ", our_little_machine->accumulator->value);
    //append chars in our temp array into output_buffer
    for (int i = 0; i < strlen(accum_value); ++i) {
        char c = accum_value[i];
        append(our_little_machine->output_buffer, c);
    }
}

void lmsm_i_inp(lmsm *our_little_machine) {

}

void lmsm_i_load(lmsm *our_little_machine, int location) {
    our_little_machine->accumulator->value = our_little_machine->memory[location];
}

void lmsm_i_add(lmsm *our_little_machine, int location) {
    /*
    int c = our_little_machine->accumulator->value + our_little_machine->memory[location];
    if (c <= 999) {
        our_little_machine->accumulator->value += our_little_machine->memory[location];
    }
    else{
        our_little_machine->accumulator->value = 999;
    }
     */
    our_little_machine->accumulator->value += our_little_machine->memory[location];
    //test DEBUG
    //printf("%d\n",c);
    //printf("%d\n",our_little_machine->accumulator->value);
}

void lmsm_i_sub(lmsm *our_little_machine, int location) {
    int c = our_little_machine->accumulator->value - our_little_machine->memory[location];
    //DEBUG
    //printf("%d\n",c);

    our_little_machine->accumulator->value -= our_little_machine->memory[location];
}

void lmsm_i_load_immediate(lmsm *our_little_machine, int value) {
    our_little_machine->accumulator->value = value;
}

void lmsm_i_store(lmsm *our_little_machine, int location) {
    our_little_machine->memory[location] = our_little_machine->accumulator->value;
}

void lmsm_i_halt(lmsm *our_little_machine) {
    our_little_machine->status = STATUS_HALTED;
}

void lmsm_i_branch_unconditional(lmsm *our_little_machine, int location) {
    our_little_machine->memory[location] = our_little_machine->accumulator->value;
    our_little_machine->program_counter = location;
}

void lmsm_i_branch_if_zero(lmsm *our_little_machine, int location) {
    int c = our_little_machine->accumulator->value;
    if (c == 0){
        our_little_machine->memory[location] = our_little_machine->accumulator->value;
        our_little_machine->program_counter = location;
    }
}

void lmsm_i_branch_if_positive(lmsm *our_little_machine, int location) {
    int c = our_little_machine->accumulator->value;
    if (c >= 0){
        our_little_machine->memory[location] = our_little_machine->accumulator->value;
        our_little_machine->program_counter = location;
    }
}

void lmsm_cap_accumulator_value(lmsm *our_little_machine){
    if(our_little_machine->accumulator->value < -999 ){
        our_little_machine->accumulator->value = -999;
    }
    else if(our_little_machine->accumulator->value > 999){
        our_little_machine->accumulator->value = 999;
    }
}

void lmsm_step(lmsm *our_little_machine) {
    if(our_little_machine->status != STATUS_HALTED) {
        int c = our_little_machine->program_counter;
        lmsm_exec_instruction(our_little_machine, our_little_machine->memory[c]);
        our_little_machine->current_instruction = our_little_machine->memory[c];
        c += 1;
        our_little_machine->program_counter = c;
    }

}

//======================================================
//  LMSM Implementation
//======================================================

void lmsm_exec_instruction(lmsm *our_little_machine, int instruction) {

    // TODO - dispatch the rest of the instruction set and implement
    //        the instructions above

    if (instruction == 0) {
        lmsm_i_halt(our_little_machine);
    } else if (100 <= instruction && instruction <= 199) {
        lmsm_i_add(our_little_machine, instruction - 100);
    } else if (200 <= instruction && instruction <= 299) {
        lmsm_i_sub(our_little_machine, instruction - 200);
    } else if (300 <= instruction && instruction <= 399) {
        lmsm_i_store(our_little_machine, instruction - 300);
    } else if (400 <= instruction && instruction <= 499) {
        lmsm_i_load_immediate(our_little_machine, instruction - 400);
    } else if (500 <= instruction && instruction <= 599) {
        lmsm_i_load(our_little_machine, instruction - 500);
    } else if (600 <= instruction && instruction <= 699) {
        lmsm_i_branch_unconditional(our_little_machine, instruction - 600);
    } else if (700 <= instruction && instruction <= 799) {
        lmsm_i_branch_if_zero(our_little_machine, instruction - 700);
    } else if (800 <= instruction && instruction <= 899) {
        lmsm_i_branch_if_positive(our_little_machine, instruction - 800);
    } else if (instruction == 901) {
        lmsm_i_inp(our_little_machine);
    } else if (instruction == 902) {
        lmsm_i_out(our_little_machine);
    } else if (instruction == 910) {
        lmsm_i_call(our_little_machine);
    } else if (instruction == 911) {
        lmsm_i_return(our_little_machine);
    } else if (instruction == 920) {
        lmsm_i_push(our_little_machine);
    } else if (instruction == 921) {
        lmsm_i_pop(our_little_machine);
    } else if (instruction == 922) {
        lmsm_i_dup(our_little_machine);
    } else if (instruction == 923) {
        lmsm_i_sadd(our_little_machine);
    } else if (instruction == 924) {
        lmsm_i_ssub(our_little_machine);
    } else if (instruction == 925) {
        lmsm_i_smax(our_little_machine);
    } else if (instruction == 926) {
        lmsm_i_smin(our_little_machine);
    } else if (instruction == 927) {
        lmsm_i_smul(our_little_machine);
    } else if (instruction == 928) {
        lmsm_i_sdiv(our_little_machine);
    }else {
        our_little_machine->error_code = ERROR_UNKNOWN_INSTRUCTION;
        our_little_machine->status = STATUS_HALTED;
    }
    lmsm_cap_accumulator_value(our_little_machine);
}

void lmsm_load(lmsm *our_little_machine, int *program, int length) {
    for (int i = 0; i < length; ++i) {
        our_little_machine->memory[i] = program[i];
    }
}

void lmsm_delete_stack(lmsm_stack *current) {
    if (current->next) {
        lmsm_delete_stack(current->next);
    }
    free(current);
}

void lmsm_init(lmsm *the_machine) {
    the_machine->accumulator->value = 0;
    the_machine->accumulator->next = NULL;
    the_machine->status = STATUS_READY;
    the_machine->error_code = ERROR_NONE;
    the_machine->program_counter = 0;
    the_machine->current_instruction = 0;
    the_machine->call_stack->value = 0;
    the_machine->call_stack->next = NULL;
    memset(the_machine->output_buffer, 0, sizeof(char) * 1000);
    memset(the_machine->memory, 0, sizeof(int) * 100);
}

void lmsm_reset(lmsm *our_little_machine) {
    lmsm_delete_stack(our_little_machine->accumulator);
    our_little_machine->accumulator = malloc(sizeof(lmsm_stack));
    lmsm_delete_stack(our_little_machine->call_stack);
    our_little_machine->call_stack = malloc(sizeof(lmsm_stack));
    lmsm_init(our_little_machine);
}

void lmsm_run(lmsm *our_little_machine) {
    our_little_machine->status = STATUS_RUNNING;
    while (our_little_machine->status != STATUS_HALTED) {
        lmsm_step(our_little_machine);
    }
}

lmsm *lmsm_create() {
    lmsm *the_machine = malloc(sizeof(lmsm));
    the_machine->accumulator = malloc(sizeof(lmsm_stack));
    the_machine->call_stack = malloc(sizeof(lmsm_stack));
    lmsm_init(the_machine);
    return the_machine;
}

void lmsm_delete(lmsm *the_machine) {
    lmsm_delete_stack(the_machine->accumulator);
    lmsm_delete_stack(the_machine->call_stack);
    free(the_machine);
}