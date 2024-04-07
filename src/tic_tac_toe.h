enum { DIM = 2 };

typedef struct {
  char symb;
  int isturn;
} player;

typedef struct {
  char spaces[DIM][DIM];
  int dim;
} board;
