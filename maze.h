typedef struct _tree tree;
typedef tree maze;



typedef struct {
    // index of the corner
    int x,y;
    int width;
    int height;
} rectangle;

// wall
typedef enum {
    HORIZONTAL,
    VERTICAL
} direction;

struct _tree
{
    rectangle rectangle;

    direction wall_direction;
    int wall_pos;
    int door_pos;

    // sub tree
    tree *left;
    tree *right;
};


maze *maze_random (int width, int height);
void maze_svg (maze *maze, char *filename);

