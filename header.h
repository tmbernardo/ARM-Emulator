int sum_array_a(int *array, int n);
int sum_array_c(int *array, int n);

int find_max_c(int *array, int n);
int find_max_a(int *array, int n);

int fib_iter_c(int n);
int fib_iter_a(int n);

int fib_rec_c(int n);
int fib_rec_a(int n);

int find_str_c(char *s, char *sub);
int find_str_a(char *s, char *sub);

struct arm_state *arm_state_new(unsigned int stack_size, unsigned int *func,
                                unsigned int arg0, unsigned int arg1,
                                unsigned int arg2, unsigned int arg3);
void arm_analysis_print(struct arm_state *as);
unsigned int arm_state_execute(struct arm_state *as);
void arm_state_free(struct arm_state *as);
