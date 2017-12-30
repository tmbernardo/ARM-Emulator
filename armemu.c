#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NREGS 16
#define SP 13
#define LR 14 
#define PC 15

struct arm_state {
    unsigned int regs[NREGS];
    unsigned int cpsr;
    unsigned int stack_size;
    unsigned char *stack;
    
    unsigned int inst_count;
    unsigned int comp_count;
    unsigned int mem_count;
    unsigned int b_count;
};

struct arm_state *arm_state_new(unsigned int stack_size, unsigned int *func,
                                unsigned int arg0, unsigned int arg1,
                                unsigned int arg2, unsigned int arg3)
{
    struct arm_state *as;
    int i;

    as = (struct arm_state *) malloc(sizeof(struct arm_state));
    if (as == NULL) {
        printf("malloc() failed, exiting.\n");
        exit(-1);
    }

    as->stack = (unsigned char *) malloc(stack_size);
    if (as->stack == NULL) {
        printf("malloc() failed, exiting.\n");
        exit(-1);
    }
    
    as->stack_size = stack_size;

    /* Initialize all registers to zero. */
    for (i = 0; i < NREGS; i++) {
        as->regs[i] = 0;
    }

    as->regs[PC] = (unsigned int) func;
    as->regs[SP] = (unsigned int) as->stack + as->stack_size;

    as->cpsr = 0;
    as->regs[0] = arg0;
    as->regs[1] = arg1;
    as->regs[2] = arg2;
    as->regs[3] = arg3;
    
    as->inst_count = 0;
    as->comp_count = 0;
    as->mem_count = 0;
    as->b_count = 0;
    
    return as;
}

void arm_state_free(struct arm_state *as)
{
    free(as->stack);
    free(as);
}

// prints out an anaylsis of all instructions ran
void arm_analysis_print(struct arm_state *as) {
    printf("\nAnalysis Report\n");
    printf("Total Instruction Count: %d\n", as->inst_count);
    printf("    Computation Count: %d\n", as->comp_count);
    printf("    Memory Count: %d\n", as->mem_count);
    printf("    Branch Count: %d\n", as->b_count);
    printf("\n");
}

// checks if instruction condition matches CPSR
bool cond_matches_cpsr(struct arm_state *as, unsigned int iw) {
    
    unsigned int cond = (iw >> 28) & 0b1111;
    unsigned int n = (as->cpsr >> 31);
    unsigned int z = (as->cpsr >> 30) & 0b1;
    unsigned int c = (as->cpsr >> 29) & 0b1;
    unsigned int v = (as->cpsr >> 28) & 0b1;

    if (cond == 14) {
        return true;
    } else if (cond == 0 && z == 1) {
        return true;
    } else if (cond == 1 && z == 0) {
        return true;
    } else if (cond == 12 && z == 0 && n == v) {
        return true;
    }

    return false;
}

// is the instruction an immediate
bool iw_is_immediate(unsigned int iw) {
    return ((iw >> 25) & 0b1) == 1;
}

// is it a compare instruction
bool iw_is_cmp_instruction(unsigned int iw) {
    unsigned int op;
    unsigned int opcode;

    op = (iw >> 26) & 0b11;
    opcode = (iw >> 21) & 0b1111;

    return (op == 0) && (opcode == 10);
}
void execute_cmp_instruction(struct arm_state *as, unsigned int iw)
{
    signed int condition;
    unsigned int rn;
    unsigned int rm;

    rn = (iw >> 16) & 0b1111;
    rm = iw & 0b1111;

    if (iw_is_immediate(iw)) {
        condition = (unsigned int) as->regs[rn] - (iw & 0b11111111);
    } else {
        condition = (unsigned int) as->regs[rn] - as->regs[rm];
    }
    
    if (condition == 0) {
        as->cpsr = 0x40000000 | as->cpsr;
    } else if (condition > 0) {
        as->cpsr = 0x3FFFFFFF & as->cpsr;
    } else if (condition < 0) {
        as->cpsr = (0x8FFFFFFF | as->cpsr) & 0xBFFFFFFF;
    }

    as->regs[PC] = as->regs[PC] + 4;
}

// is condition an add instruction
bool iw_is_add_instruction(unsigned int iw)
{
    unsigned int op;
    unsigned int opcode;

    op = (iw >> 26) & 0b11;
    opcode = (iw >> 21) & 0b1111;

    return (op == 0) && (opcode == 4);
}
void execute_add_instruction(struct arm_state *as, unsigned int iw)
{
    unsigned int rd;
    unsigned int rn;
    unsigned int rm;

    rd = (iw >> 12) & 0b1111;
    rn = (iw >> 16) & 0b1111;
    rm = iw & 0b1111;

    if (iw_is_immediate(iw)) {
        as->regs[rd] = as->regs[rn] + (iw & 0b11111111);
    } else {
        as->regs[rd] = as->regs[rn] + as->regs[rm];
    }

    as->regs[PC] = as->regs[PC] + 4;
}

// is it a subtract instruction
bool iw_is_sub_instruction(unsigned int iw)
{
    unsigned int op;
    unsigned int opcode;

    op = (iw >> 26) & 0b11;
    opcode = (iw >> 21) & 0b1111;
 
    return (op == 0) && (opcode == 2);
}
void execute_sub_instruction(struct arm_state *as, unsigned int iw)
{
    unsigned int rd;
    unsigned int rn;
    unsigned int rm;

    rd = (iw >> 12) & 0b1111;
    rn = (iw >> 16) & 0b1111;
    rm = iw & 0b1111;
    
    if (iw_is_immediate(iw)) {
        as->regs[rd] = as->regs[rn] - (iw & 0b11111111);
    } else {
        as->regs[rd] = as->regs[rn] - as->regs[rm];
    }

    as->regs[PC] = as->regs[PC] + 4; 
}

// is it a move instruction
bool iw_is_mov_instruction(unsigned int iw) {
    unsigned int op;
    unsigned int opcode;

    op = (iw >> 26) & 0b11;
    opcode = (iw >> 21) & 0b1111;
 
    return (op == 0) && (opcode == 13 || opcode == 15);
}
void execute_mov_instruction(struct arm_state *as, unsigned int iw){    
    unsigned int rd;
    unsigned int rn;
    unsigned int rm;

    rd = (iw >> 12) & 0b1111;
    rm = iw & 0b1111;
    
    if (iw_is_immediate(iw)) {
        as->regs[rd] = iw & 0b11111111;
        if (((iw >> 21) & 0b1111) == 15) {
            as->regs[rd] = ~as->regs[rd];
        }
    } else {
        as->regs[rd] = as->regs[rm];
    }

    as->regs[PC] = as->regs[PC] + 4; 
}


void execute_ldr_instruction(struct arm_state *as, unsigned int i,
                             unsigned int rn, unsigned int rd,
                             unsigned int rm, unsigned int imm) { 
    if (i == 0) {
        as->regs[rd] = ((int *) as->regs[rn])[imm/4];
    } else {
        as->regs[rd] = ((int *) as->regs[rn])[as->regs[rm]];
    }

    return;
}

void execute_ldrb_instruction(struct arm_state *as, unsigned int i,
                              unsigned int rn, unsigned int rd,
                              unsigned int rm, unsigned int imm) {

    if (i == 0) {
        as->regs[rd] = ((char *) as->regs[rn])[imm/4];
    } else {
        as->regs[rd] = ((char *) as->regs[rn])[as->regs[rm]];
    }
    return;
}

void execute_str_instruction(struct arm_state *as, unsigned int i,
                             unsigned int rn, unsigned int rd,
                             unsigned int rm, unsigned int imm) {
    
    if (i == 0) {
        ((int *)as->regs[rn])[imm/4] = as->regs[rd];
    } else {
        ((int *)as->regs[rn])[as->regs[rm]] = as->regs[rd];
    }
    
    return;
}

// is it an single data transfer instruction
bool iw_is_sdt_instruction(unsigned int iw) { 
    return ((iw >> 26) & 0b11) == 1;
}
void execute_sdt_instruction(struct arm_state *as, unsigned int iw) {
    unsigned int b = (iw >> 22) & 0b1;
    unsigned int l = (iw >> 20) & 0b1;

    unsigned int i = (iw >> 25) & 0b1;
    unsigned int rn = (iw >> 16) & 0b1111;
    unsigned int rd = (iw >> 12) & 0b1111;
    
    unsigned int rm = iw & 0b1111;
    unsigned int imm = iw & 0b11111111;
 
    if (l == 1 && b == 1) {
        execute_ldrb_instruction(as, i, rn, rd, rm, imm);
    } else if (l == 1) {
        execute_ldr_instruction(as, i, rn, rd, rm, imm);
    } else {
        execute_str_instruction(as, i, rn, rd, rm, imm);
    }

    as->regs[PC] = as->regs[PC] + 4; 
    return;
}

// is it a branch instruction
bool iw_is_b_instruction(unsigned int iw) { 
    return ((iw >> 24) & 0b1111) == 0b1010;
}
void execute_b_instruction(struct arm_state *as, unsigned int iw) {
    unsigned int new_pc = iw & 0b111111111111111111111111;
    if ((new_pc >> 23) == 1) {
        new_pc = (new_pc | 0xFF800000) << 2;
    } else {
        new_pc = new_pc << 2;
    }
    as->regs[PC] += new_pc + 8;
    return;
}

// is it a branch and link instruction?
bool iw_is_bl_instruction(unsigned int iw) {
    return ((iw >> 24) & 0b1111) == 0b1011;
}
void execute_bl_instruction(struct arm_state *as, unsigned int iw) {
    as->regs[LR] = as->regs[PC] + 4;
    execute_b_instruction(as, iw);
    return;
}

// is it a branch and exchange instruction
bool iw_is_bx_instruction(unsigned int iw)
{
    return ((iw >> 4) & 0xFFFFFF) == 0b000100101111111111110001;
}
void execute_bx_instruction(struct arm_state *as, unsigned int iw)
{   
    as->regs[PC] = as->regs[LR];
    as->regs[LR] = 0;
}

// executes one ARM instruction
void arm_state_execute_one(struct arm_state *as)
{
    unsigned int iw;
    unsigned int *pc;

    pc = (unsigned int *) as->regs[PC];
    iw = *pc;
    
    // checks if the condition code matches the CPSR
    if (!cond_matches_cpsr(as, iw)) {
        as->regs[PC] = as->regs[PC] + 4;
        return;
    // executes branch and exchange instruction
    } else if (iw_is_bx_instruction(iw)) {
        execute_bx_instruction(as, iw);
        as->b_count++;
        as->inst_count++;
    // executes branch instruction
    } else if (iw_is_b_instruction(iw)) {
        execute_b_instruction(as, iw);
        as->b_count++;
        as->inst_count++;
    // executes branch and link instruction
    } else if (iw_is_bl_instruction(iw)) {
        execute_bl_instruction(as, iw);
        as->b_count++;
        as->inst_count++;
    // executes compare instruction
    }  else if (iw_is_cmp_instruction(iw)) {
        execute_cmp_instruction(as, iw);
        as->comp_count++;
        as->inst_count++;
    // executes subtract instruction
    } else if (iw_is_sub_instruction(iw)) {
        execute_sub_instruction(as, iw);
        as->comp_count++;
        as->inst_count++;
    // executes move instruction
    } else if (iw_is_mov_instruction(iw)) {
        execute_mov_instruction(as, iw);
        as->mem_count++;
        as->inst_count++;
    // exuctes addition instruction
    } else if (iw_is_add_instruction(iw)) {
        execute_add_instruction(as, iw);
        as->comp_count++;
        as->inst_count++;
    // executes single data transfer instruction
    } else if (iw_is_sdt_instruction(iw)) {
        execute_sdt_instruction(as, iw);
        as->mem_count++;
        as->inst_count++;
    } 
}

// continues to execute instructions until the PC is 0
unsigned int arm_state_execute(struct arm_state *as)
{
    while (as->regs[PC] != 0) {
        arm_state_execute_one(as);
    }

    return as->regs[0];
}
