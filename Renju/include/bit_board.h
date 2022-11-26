#define MOVE_NUM 32768
void init_move_table();
int get_new_move(tree move_set, line* last_buf, int j);
void set_bit_board(int i, int j);
void buf_bit_move(int j, line* buf);
void reset_bit_move(int j, line* buf);
void get_range(int* start_j, int *end_j, int j);

